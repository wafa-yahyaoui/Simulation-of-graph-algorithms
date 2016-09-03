#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem
{
public:
Edge(Node *sourceNode, Node *destNode, double weight=0);
Node *sourceNode() const;
Node *destNode() const;
double edge_cost() const; //  an accessor to the edge cost
Edge* edge_pointer(); // an accessor to the pointer "this"
void setState(int i){state=i;}; // to be used in algorithmes
void adjust();
enum { Type = UserType + 2 };
int type() const Q_DECL_OVERRIDE { return Type; }


protected:
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

private:
    Node *source, *dest;
    double cost; // a node has a cost
    int state; // to be used in algorithms
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

#endif // EDGE_H
