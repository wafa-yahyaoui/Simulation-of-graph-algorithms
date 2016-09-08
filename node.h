#ifndef NODE_H
#define NODE_H
#include <QGraphicsItem>
#include <QList>
#include<QGraphicsTextItem>
#include<QString>

class Edge;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget,QString nom="ADD NAME"); // un noeud doit avoir un nom
    void addEdge(Edge *edge); // dessin +algo
    void dellEdge(Edge *edge); // dessin +algo
    QList<Edge *> edges() const;
    void

    enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE { return Type; }

   void calculateForces();
    bool advance();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    // ajouter un accesseur pour le nom
    QString node_name() const;
    // an accessor to the pointer on the node
    Node* node_pointer();
// methode to add a successor node
    void add_successor_node(Node* node);
 // method to delete a successor node
    void delete_successor_node(Node* node );
 // accesseur pour successor nodes
    QList<Node*> accessor_successor_nodes();
    // accesseur to successor edges
    QList<Edge *> accessor_successor_edges() const;
    // add successor edge to use in algorithms
    void add_successor_edge(Edge* arc);
    void setState(int s){active=s;}; // to be used in dijkstra algorithme
    void setExplored(bool state){explored=state;}; // to be used in bfs; dfs algorithmes
    bool isExplored(){return explored;};// to be used in bfs;dfs algorithmes


protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QString name; // un noeud sera caractérisé par un nom
    QList<Node*> successor_nodes; // chaque noeud aura une liste qui contient ses suivants
    QList<Edge *> edgeList; // liste de tous  les edges
    QPointF newPos;
    GraphWidget *graph;
    int active; // a utilisé dans algo dijkstra
    QList<Edge *> successor_edges; // à utiliser dans algo dijkstra
    bool explored; // to be used in bfs; dfs algorithms

};

#endif // NODE_H
