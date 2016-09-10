#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include"wait.h"
#include <math.h>
#include <QKeyEvent>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    // addin the scene to private attributors
    scene_att->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene_att->setSceneRect(-200, -200, 400, 400);
    setScene(scene_att);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Graph Simulation"));

}
// Adding a  node
void GraphWidget::add_node(QString name)
{
// add node to the scene and also to graph_algo
    Node *node = new Node(this,name);
    graph_algo.append(node);
     QGraphicsTextItem *nodeText= new QGraphicsTextItem(node->node_name(),node); // adding label
    nodeText->setHtml(QString("<div style='background:rgba(255, 255, 255, 100%);'><b>" + QString(node->node_name()) + QString("</b></div>") ));
scene_att->addItem(node);
node->setPos(0,0);
update();
}
// Deleting a node
void GraphWidget::delete_node(QString name_node_to_delete)
{

    Node* p = find_pointer(name_node_to_delete);
   graph_algo.removeOne(p);
    scene_att->removeItem(p);
    delete p;
update();
}

// adding an edge
void GraphWidget::add_edge(QString name_source, QString name_destination, double weight)
{
    Edge *edge = new Edge(find_pointer(name_source), find_pointer(name_destination),weight);
    scene_att->addItem(edge);
    graph_edges.append(edge);
    update();
}

// a methode that delete an edge given the source node and destination node (after verification )
void GraphWidget::delete_edge(QString source_edge_to_delete ,QString destination_edge_to_delete)
{
    Edge* pointer_edge_delete=find_edge_pointer(source_edge_to_delete,destination_edge_to_delete);
graph_edges.removeOne(pointer_edge_delete);
find_pointer(source_edge_to_delete)->delete_successor_node(find_pointer(destination_edge_to_delete));
find_pointer(source_edge_to_delete)->dellEdge(pointer_edge_delete);
find_pointer(destination_edge_to_delete)->dellEdge(pointer_edge_delete);
find_pointer(source_edge_to_delete)->delete_successor_edge(pointer_edge_delete);
scene_att->removeItem(pointer_edge_delete);
delete pointer_edge_delete;
pointer_edge_delete=NULL;
update();

}

// a method that returns the pointer to the node using its name destination and name source (after verificat)
    Node* GraphWidget::find_pointer(QString nom)
    {
        QList<Node*>::iterator it=graph_algo.begin();
        while((*it)->node_name()!=nom)
        {
        it++;
        }
        return (*it)->node_pointer();
    }

// a method that returns the pointer to the edge using the name of destination and source
Edge* GraphWidget::find_edge_pointer(QString name_source, QString name_destination)
{
    Node* pointer_name_source = find_pointer(name_source);
    Node* pointer_name_destination = find_pointer(name_destination);
    Edge* pointer_to_edge;
    for(QList<Edge*>::iterator it = graph_edges.begin();it!= graph_edges.end();++it)
    {
        if ((*it)->destNode()==pointer_name_destination && (*it)->sourceNode()==pointer_name_source)
        {
            pointer_to_edge=(*it)->edge_pointer();
            break;
        }
    }
    return pointer_to_edge;

}

// a method that verify the existence of the edge using the name of destination and source
bool GraphWidget::verify_edge_existence(QString name_source, QString name_destination)
{
    Node* pointer_name_source = find_pointer(name_source);
    Node* pointer_name_destination = find_pointer(name_destination);
    bool b=false;
    for(QList<Edge*>::iterator it = graph_edges.begin();it!= graph_edges.end();++it)
    {
        if ((*it)->destNode()==pointer_name_destination && (*it)->sourceNode()==pointer_name_source)
        {
            b=true;
            break;
        }
    }
    return b;

}

// a method that reset all the graph states to 0 and explored to false ===> reset colors
   void GraphWidget::reset()
   {
       for (QList<Node*>::iterator it=graph_algo.begin();it!=graph_algo.end();++it)
       {
           (*it)->setState(-1);
           (*it)->setExplored(false);
           (*it)->update();


       }
       for (QList<Edge*>::iterator it=graph_edges.begin();it!=graph_edges.end();++it)
       {
           (*it)->setState(0);
       }
   }
   //a method that verifies if a node is still connected
   bool GraphWidget::connected_node(QString node)
   {
       bool b =false;
       for(QList<Edge*>::iterator it= graph_edges.begin();it!=graph_edges.end();++it)
       {
           if (((*it)->destNode()->node_name().compare(node)==0) ||((*it)->sourceNode()->node_name().compare(node)==0))
           {
               b=true;
               break;
           }
       }
       return b ;
   }


void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
       node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

#ifndef QT_NO_WHEELEVENT
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
#endif

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::white);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::white);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::white);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);

    painter->setPen(Qt::black);

}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items()) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
//++++++++++++++++++++ LES ALGORITHMES ++++++++++++++++++++++++++++
QString GraphWidget::minDistance(QMap<QString,double> dist,QMap<QString,bool> sptSet)
{
    int min = INT_MAX;
    QString name_min;

for (QMap<QString,double>::iterator it=dist.begin();it != dist.end();++it)
{
    if( (it.value()< min)&& (!sptSet[it.key()]) )
    {
        min=it.value();
        name_min=it.key();
    }
}
return name_min;
}
// 1) Dijkstra algorithm ==============================
void GraphWidget::dijkstra (QString name_first_node,QString name_second_node)
{
    reset(); // retour du graph à son état initial
    // generation des  map qui relie les noms des noeuds à des int
    QMap <QString,QString> parent;
    QMap <QString,double>dist;
    QMap <QString,bool> sptSet;

    for (QList<Node*>::iterator it = graph_algo.begin();it!=graph_algo.end();++it)
    {
     QString nom =(*it)->node_name();
     dist.insert(nom,INT_MAX);
     sptSet.insert(nom,false);
    }

    dist[name_first_node] = 0;
    parent[name_first_node] = "NO PARENT";




    for (QMap<QString,double>::iterator it=dist.begin();it != dist.end();++it)
    {
        QString name_dij_node = minDistance(dist, sptSet);

        sptSet[name_dij_node] = true;

        if (name_second_node.compare(name_dij_node) == 0) break;
        for (QMap<QString,double>::iterator itr=dist.begin();itr != dist.end();++itr)
        {
            QString name_node = itr.key();
         Edge* pointer_edge_dij_node_to_name_node=find_edge_pointer(name_dij_node,name_node);
         double cost_edge_dij_node_to_name_node=pointer_edge_dij_node_to_name_node->edge_cost();
            if (!sptSet[name_node] && dist[name_dij_node] != INT_MAX && dist[name_dij_node]+cost_edge_dij_node_to_name_node < dist[name_node])
            {
                dist[name_node]= dist[name_dij_node] + cost_edge_dij_node_to_name_node;
                parent[name_node] = name_dij_node;
            }
        }
    }

    QString node_name = name_second_node;
    QList<Node *> Shortest_Path;
    Shortest_Path<<find_pointer(name_second_node);
    while (node_name.compare(name_first_node) != 0)
    {
        node_name = parent[node_name];
        Shortest_Path<<find_pointer(node_name);
    }

    for(int i=Shortest_Path.size()-1; i>0;i--)
    {
        QList<Edge*> edges_list = Shortest_Path[i]->accessor_successor_edges();
        Edge *next=NULL;
        Shortest_Path[i]->setState(1);
        Shortest_Path[i]->update();
        wait(1000);
        foreach(Edge *e, edges_list)
        {
            if(e->destNode()==Shortest_Path[i-1]) next =e;
            e->setState(-1);
            e->update();
            wait(1000);
        }
        foreach(Edge *e, edges_list)
        {
            if(e->destNode()==Shortest_Path[i-1]) next =e;
            e->setState(0);
            e->update();
        }
        wait(1000);
        next->setState(1);
        next->update();
        wait(1000);
    }
    Shortest_Path[0]->setState(1);
    Shortest_Path[0]->update();
    wait(1000);
}
//1) Parcours largeur
void GraphWidget::bfs(QString name_start_node)
{
    reset(); // retour du graph à son etat intial
    QQueue<Node *> Q;
    Node *pointer_start_node = find_pointer(name_start_node);
    Q.enqueue(pointer_start_node);
    pointer_start_node->setExplored(true);
    while (!Q.isEmpty())
    {
        Node *node_exploration_done = Q.dequeue();
        node_exploration_done->setState(1);
        if(node_exploration_done->node_name()!=name_start_node)
        {
            pointer_start_node->setState(0);
            pointer_start_node->update();
        }
        node_exploration_done->update();
        foreach(Edge* e,node_exploration_done->accessor_successor_edges())
        {
            if (!e->destNode()->isExplored())
            {
                Q.enqueue(e->destNode());
                e->destNode()->setExplored(true);
            }
        }
        name_start_node=node_exploration_done->node_name();
        pointer_start_node=find_pointer(name_start_node);
         pointer_start_node->setExplored(true);
        wait(2000);
    }
    pointer_start_node->setState(0);
    pointer_start_node->update();
}

// 2) Parcours Pronfondeur
void GraphWidget::dfs(QString name_start_node)
{
    reset();
    QStack<Node *> s;
    Node *pointer_start_node= find_pointer(name_start_node);
    s.push(pointer_start_node);
    pointer_start_node->setExplored(true);
    while (!s.isEmpty())
    {
        Node *node_exploration_done = s.pop();
        node_exploration_done->setState(1);
        if(node_exploration_done->node_name()!= name_start_node)
        {
            pointer_start_node->setState(0);
            pointer_start_node->update();
        }
        node_exploration_done->update();
        for(int i=node_exploration_done->accessor_successor_edges().size()-1; i>=0;i--)
        {
            if (!node_exploration_done->accessor_successor_edges()[i]->destNode()->isExplored())
            {
                s.push(node_exploration_done->accessor_successor_edges()[i]->destNode());
                node_exploration_done->accessor_successor_edges()[i]->destNode()->setExplored(true);
            }
        }
        name_start_node=node_exploration_done->node_name();
        pointer_start_node=find_pointer(name_start_node);
        wait(2000);
    }
    pointer_start_node->setState(0);
    pointer_start_node->update();
}
