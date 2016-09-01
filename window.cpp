#include "window.h"
window::window():QMainWindow()
{
    // ===== Show the graph as central widget =======
    graph = new GraphWidget;
    setCentralWidget(graph);
//===adding menu========
QMenu *menu = menuBar()->addMenu("Choose an action"); // to add the menu choose action
// ==== Adding actions to the menu ========
QAction *add_a_node= new QAction("Add node", this);
    menu->addAction(add_a_node);
QAction *add_an_edge= new QAction("Add edge", this);
        menu->addAction(add_an_edge);
QAction *shortest_path= new QAction("Shortest path : Dijksta algorithm", this);
            menu->addAction(shortest_path);
QAction *delete_a_node= new QAction("Delete node", this);
                menu->addAction(delete_a_node);

QAction *quit_application= new QAction("Quit", this);
                menu->addAction(quit_application);

//adding shrtcuts =============
quit_application->setShortcut(QKeySequence("Ctrl+Q"));
add_a_node->setShortcut(QKeySequence("Ctrl+N"));
add_an_edge->setShortcut(QKeySequence("Ctrl+E"));
shortest_path->setShortcut(QKeySequence("Ctrl+D"));
delete_a_node->setShortcut(QKeySequence("Ctrl+C"));


// Connecter les signaux et les slots
connect(quit_application, SIGNAL(triggered()), qApp, SLOT(quit()));
connect(add_a_node, &QAction::triggered, this, &window::open_window_add_node);
//connect(shortest_path, &QAction::triggered, this, &window::open_window_dijkstra_algorithm);
connect(add_an_edge, &QAction::triggered, this, &window::open_window_add_edge);
connect(shortest_path, &QAction::triggered, this, &window::open_window_dijkstra_algorithm);
}
//==============generer les slots===================
void window::open_window_add_node()//windo to  Add new node
{
    bool ok = false;
  QString new_node = QInputDialog::getText(this, "Add new node", "type new node's name",QLineEdit::Normal, QString(),&ok);
  if (ok && !new_node.isEmpty() && ! existing_name_nodes.contains(new_node))
   {
    existing_name_nodes.append(new_node);
      graph->add_node(new_node);
    }
  else if(ok && !new_node.isEmpty() && existing_name_nodes.contains(new_node))
  {
      QMessageBox::critical(this, "Existing name of node", "You used this name before try another name !");
  }
}
// ******************* Dijkstra algorithm ***********************
void window::open_window_dijkstra_algorithm()

    {
        QString origine_node;
        QString destination_node;
        bool positifs_costs =true;
        for(QList<double>::iterator it=cost_edges.begin();it!= cost_edges.end() && positifs_costs;it++)
        {
            if (*it < 0)
            {
                positifs_costs=false;
            }
        }
        if (!positifs_costs)
        {
           QMessageBox::critical(this, "Negatif Cost", "You can not use Dijkstra Algorithm on a graph containing edges with negatif costes !");
        }
        else
        {
        bool ok = false;
      origine_node = QInputDialog::getText(this, "DIJKTRA ALGORITHM", "Type the name of origine node",QLineEdit::Normal, QString(),&ok);
      if (ok && !origine_node.isEmpty() && existing_name_nodes.contains(origine_node))
       {
          ok=false;
          destination_node = QInputDialog::getText(this, "DIJKSTRA ALGORITHM", "Type the name of destination node",QLineEdit::Normal, QString(),&ok);
          if (ok && !destination_node.isEmpty() &&existing_name_nodes.contains(destination_node) && (origine_node.compare(destination_node) != 0))
          {

                       graph->dijkstra(origine_node,destination_node);

              }
     else if(ok && !destination_node.isEmpty() && !existing_name_nodes.contains(destination_node))
          {
               QMessageBox::critical(this, "Node not found", "Destination node does not exist !");
          }
          else if (ok && !destination_node.isEmpty() && (origine_node.compare(destination_node) == 0) )
          {
               QMessageBox::critical(this, "Same node used ", "destination node and origine node should not be the same !");
          }

        }
      else if (ok && !origine_node.isEmpty() && !existing_name_nodes.contains(origine_node))
      {
           QMessageBox::critical(this, "Node not found", "Origine node does not exist !");
      }
        }

    }


void window::open_window_delete_edge()
{
    fenetre_delete_edge.exec();
}

// ****************** Add edge  ****************

void window::open_window_add_edge()
{
    QString origine_node;
    QString destination_node;
    QString edge_cost_string;
    double edge_cost;
    bool convert=false;
    bool ok = false;
  origine_node = QInputDialog::getText(this, "ADD EDGE", "Type the name of origine node",QLineEdit::Normal, QString(),&ok);
  if (ok && !origine_node.isEmpty() && existing_name_nodes.contains(origine_node))
   {
      ok=false;
      destination_node = QInputDialog::getText(this, "ADD EDGE", "Type the name of destination node",QLineEdit::Normal, QString(),&ok);
      if (ok && !destination_node.isEmpty() &&existing_name_nodes.contains(destination_node) && (origine_node.compare(destination_node) != 0))
      {
          ok=false;
           edge_cost_string = QInputDialog::getText(this, "ADD EDGE", "Type the cost of edge",QLineEdit::Normal, "00.00",&ok);
           edge_cost=edge_cost_string.toDouble(&convert);
               if (ok && !edge_cost_string.isEmpty() && convert)
          {
                   graph->add_edge(origine_node,destination_node,edge_cost);
                   cost_edges.append(edge_cost);

          }
              else if (ok && !edge_cost_string.isEmpty() && !convert)
               {
                   QMessageBox::critical(this, "Cost not a number ", "Enter a valid number please !");
               }


      }
      else if (ok && !destination_node.isEmpty() && !existing_name_nodes.contains(destination_node))
      {
           QMessageBox::critical(this, "Node not found", "Destination node does not exist !");
      }
      else if (ok && !destination_node.isEmpty() && (origine_node.compare(destination_node) == 0) )
      {
           QMessageBox::critical(this, "Same node used ", "destination node and origine node should not be the same !");
      }



    }
  else if (ok && !origine_node.isEmpty() && !existing_name_nodes.contains(origine_node))
  {
       QMessageBox::critical(this, "Node not found", "Origine node does not exist !");
  }

}

