#ifndef WINDOW_H
#define WINDOW_H
#include "graphwidget.h"
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
   void open_window_bfs_algorithm();
   void open_window_dfs_algorithm();
private:
    GraphWidget *graph;
    QList<QString> existing_name_nodes;
    QList<double> cost_edges;


};

#endif // WINDOW_H
