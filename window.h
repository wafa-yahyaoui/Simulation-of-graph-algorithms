#ifndef WINDOW_H
#define WINDOW_H
#include "graphwidget.h"
#include"window_delete_edge.h"
#include <QtWidgets>
#include <QApplication>
#include<QList>
#include<QString>
#include<QDialog>
#include <QFormLayout>
class window : public QMainWindow
{

   public:
    window();
// generer les slot
public slots:
    void add_edge_slot();
   void open_window_add_node();
   void open_window_dijkstra_algorithm();
   void open_window_add_edge();
   void open_window_delete_edge();
private:
    //QDialog *Dialog_add_edge; // fenetre add edge should be an attribute
    window_delete_edge fenetre_delete_edge;// fenetre add edge should be an attribute
    GraphWidget *graph;
    QList<QString> existing_name_nodes;
    QList<double> cost_edges;


};

#endif // WINDOW_H
