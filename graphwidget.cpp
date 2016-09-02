#include "graphwidget.h"
#include "edge.h"
#include "node.h"

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
    setWindowTitle(tr("Elastic Nodes"));

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
}


// a method that returns the pointer to the node using its name
    Node* GraphWidget::find_pointer(QString nom)
    {
        QList<Node*>::iterator it=graph_algo.begin();
        while((*it)->node_name()!=nom)
        {
        it++;
        }
        return (*it)->node_pointer();
    }

// adding an edge
void GraphWidget::add_edge(QString name_source, QString name_destination, double weight)
{
    Edge *edge = new Edge(find_pointer(name_source), find_pointer(name_destination),weight);
    scene_att->addItem(edge);
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
//1) Dijikstra algorithme
void GraphWidget::dijkstra (QString name_first_node,QString name_second_node)
{
    Node* first_node_pointer = find_pointer(name_first_node);
    Node* second_node_pointer = find_pointer(name_second_node);
    Node* node_shortest_path_algo=first_node_pointer;
    QList<Node*> S_bar;
    // initialisation
    QList<Edge*>::iterator it_edge;
    QList<Edge*> first_node_successor_edges= first_node_pointer->accessor_successor_edges();
    first_node_pointer->set_dijkstra_cost(0);
    for(it_edge=first_node_successor_edges.begin();it_edge!=first_node_successor_edges.end();it_edge++);
         {
         Node* node= (*it_edge)->destNode();
         double edge_cost = (*it_edge)->edge_cost();
         node->set_dijkstra_cost(edge_cost);
//****************** testing **************************

         Node* noeud_a_colorier = node;
          noeud_a_colorier->set_color(QColor(0,0,255,127));
//*******************************************************

         }

   /*for(it=graph_algo.begin() ; it != graph_algo.end();it++)
    {


        if (*it== first_node_pointer)
        {
            S_bar.push_back(*it);
            (*it)->set_dijkstra_cost(0);
        }
        else if (first_node_pointer->accessor_successor_nodes().contains(*it ))
        {


           for(it_edge=(first_node_pointer->edges()).begin();it_edge != (first_node_pointer->edges()).end();it_edge ++)
            {
              if ( *it==(*itedge)->destNode())
                {
                   (*it)->set_dijkstra_cost((*it_edge)->edge_cost());
                }
          }
        }

        else
        {
           S_bar.push_back(*it);
            (*it)->set_dijkstra_cost(10000000000000);
        }
    }*/

 /*   while(!S_bar.empty())
    {
        //recherche du minimum
        double min=1000000000000;

        for (QList <Node*>::iterator it=S_bar.begin();it != S_bar.end();it++) // recherche du minimum
        {
            if ((*it)->return_dijkstra_cost() <=min)
            {
                min=(*it)->return_dijkstra_cost();
                min_pos=it;
            }
        }

        (*min_pos)-> set_node_shortest_path(node_shortest_path_algo);
        S_bar.erase(min_pos);
        if (S_bar.isEmpty())// sotie de la la boucle si S bar est vide
        {
            break;
        }
        // 3eme etape du l'algo , on va definir un itérateur pour edges puisk on a besoi de parcouir les arcs
        for(QList<Node*>::iterator it=((*min_pos)->accessor_successor_nodes()).begin();it !=((*min_pos)->accessor_successor_nodes()).end();it++)
        {
            if (S_bar.contains(*it))
            {
                for(QList<Edge*>::iterator it_edge=(*min_pos)->edges().begin();it_edge != (*min_pos)->edges().end();it_edge ++)
                {
                    if ( (*it==(*it_edge)->destNode()) && (((*min_pos)->return_dijkstra_cost() + (*it_edge)->edge_cost())> ((*it)->return_dijkstra_cost()) ) )
                    {
                        (*it)->set_dijkstra_cost((*min_pos)->return_dijkstra_cost() + (*it_edge)->edge_cost());
                    }
                }


        }


        }
        }

   Node* noeud_a_colorier = second_node_pointer;
// Colorier le plus court chemin
    do
   {

       noeud_a_colorier->set_color(QColor(0,0,255,127));
        noeud_a_colorier->set_color(QColor(0,0,255,127));
   noeud_a_colorier=noeud_a_colorier->accessor_node_shortest_path();
   }
   while (noeud_a_colorier != first_node_pointer);
*/

}
