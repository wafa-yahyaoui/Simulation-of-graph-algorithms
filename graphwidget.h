#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H
#include <QGraphicsView>
#include <QString>
#include <QList>
#include <iterator>
#include <QMap>
#include <climits>
#include <QQueue>
#include<QStack>

class Node;
class Edge;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);
     QGraphicsScene *scene_att = new QGraphicsScene(this);
       //Adding node node
    void add_node(QString name);
    // adding an edge
    void add_edge(QString name_souce,QString name_destination,double weight=0);
// a method that rturns the pointer to the node using its name
    Node* find_pointer(QString nom);
// a method that returns the pointer to the edge using the name of destination and source
    Edge* find_edge_pointer(QString name_source, QString name_destination);
    void itemMoved();
 // a method that reset all the graph states to 0 ===> reset colors
    void reset();
 // a method that verify the existence of the edge using the name of destination and source
 bool verify_edge_existence(QString name_source, QString name_destination);
 // a methode that delete an edge given the source node and destination node
 void delete_edge(QString source_edge_to_delete ,QString destination_edge_to_delete);
//+++++++++++++++ LES ALGORITHMES ++++++++++++++++++
    // 1) Dijikstra algorithmes
    void dijkstra (QString name_first_node,QString name_second_node);
    //2) Parcours largeur
    void bfs(QString name_start_node);
    //3) Parcours profondeur
    void dfs(QString name_start_node);
//++++++++++++++++++++++++++++++++++++++++++++++++++++
public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

    void scaleView(qreal scaleFactor);

private:
    int timerId;
    Node *centerNode;
    // adding other  implementations of the graph to be used in algorthms
    QList<Node*> graph_algo;
    QList<Edge*> graph_edges; // containes all the graph edges
    //===== attributes to be used in simulation
    QString minDistance(QMap<QString,double> dist,QMap<QString,bool> sptSet);
};

#endif // GRAPHWIDGET_H
