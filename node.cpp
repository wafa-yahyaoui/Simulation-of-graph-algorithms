#include "edge.h"
#include "node.h"
#include "graphwidget.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

Node::Node(GraphWidget *graphWidget, QString nom)
    : graph(graphWidget)
{
    name = nom;
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);


}
// a method to add successor nodes
void Node::add_successor_node(Node* node)
{
    successor_nodes.append(node);
}
// an accessor to the pointer on the node
Node* Node::node_pointer()
{
    return this;
}


void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}


void Node::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos();
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}

bool Node::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
   // painter->setBrush(Qt::darkGray);
    //painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 15);
    gradient.setCenter(3, 3);
    gradient.setFocalPoint(3, 3);
    if (option->state & QStyle::State_Sunken) {
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    }
    else if (active==1)
    {
        gradient.setColorAt(1, QColor(Qt::green));
        gradient.setColorAt(0, QColor(Qt::darkGreen));
    }
    else if (active==0)
    {
        gradient.setColorAt(0, Qt::gray);
        gradient.setColorAt(1, Qt::darkGray);
    }
    else
    {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
// accesseur pour le nom du noeud

QString Node::node_name() const
{
    return name;
}
// accesseur pour successor nodes
   QList<Node*> Node:: accessor_successor_nodes()
   {
       return successor_nodes;
   }

 //accessor to successor_edges list
 QList<Edge *> Node::accessor_successor_edges() const
 {
     return successor_edges;
 }
 // add successor edge to use in algorithms
 void Node::add_successor_edge(Edge* arc)
 {
    successor_edges.append(arc);
 }
