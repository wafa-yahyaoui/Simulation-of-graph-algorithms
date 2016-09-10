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
QAction *delete_an_edge= new QAction("Delete edge", this);
                menu->addAction(delete_an_edge);
QAction *delete_a_node= new QAction("Delete node", this);
                                menu->addAction(delete_a_node);
QAction *shortest_path= new QAction("Shortest path : Dijksta algorithm", this);
            menu->addAction(shortest_path);
QAction *BFS= new QAction("Breadth First Search", this);
                menu->addAction(BFS);
QAction *DFS= new QAction("Deapth First Search", this);
                                menu->addAction(DFS);
QAction *quit_application= new QAction("Quit", this);
                menu->addAction(quit_application);

//adding shrtcuts =============
quit_application->setShortcut(QKeySequence("Ctrl+Q"));
add_a_node->setShortcut(QKeySequence("Ctrl+N"));
add_an_edge->setShortcut(QKeySequence("Ctrl+E"));
delete_an_edge->setShortcut(QKeySequence("Ctrl+L"));
delete_a_node->setShortcut(QKeySequence("Ctrl+O"));
shortest_path->setShortcut(QKeySequence("Ctrl+D"));
BFS->setShortcut(QKeySequence("Ctrl+B"));
DFS->setShortcut(QKeySequence("Ctrl+F"));


// Connecter les signaux et les slots
connect(quit_application, SIGNAL(triggered()), qApp, SLOT(quit()));
connect(add_a_node, &QAction::triggered, this, &window::open_window_add_node);
connect(BFS, &QAction::triggered, this, &window::open_window_bfs_algorithm);
connect(DFS, &QAction::triggered, this, &window::open_window_dfs_algorithm);
connect(add_an_edge, &QAction::triggered, this, &window::open_window_add_edge);
connect(shortest_path, &QAction::triggered, this, &window::open_window_dijkstra_algorithm);
connect(delete_an_edge, &QAction::triggered, this, &window::open_window_delete_edge);
connect(delete_a_node, &QAction::triggered, this, &window::open_window_delete_node);
}
//==============generer les slots===================
//*********************** add a node *********************
void window::open_window_add_node()
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

//*********************** BFS algorithme ********************
void window::open_window_bfs_algorithm()
{
    bool ok = false;
  QString origine = QInputDialog::getText(this, "BFS Algorithm", "type the origine node's name",QLineEdit::Normal, QString(),&ok);
  if (ok && !origine.isEmpty() && existing_name_nodes.contains(origine))
   {
      graph->bfs(origine);
    }
  else if(ok && !origine.isEmpty() && !existing_name_nodes.contains(origine))
  {
      QMessageBox::critical(this, "Origine not found", "The node you typed does not exist, try again !");
  }
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

//*********************** DFS***********************
void window::open_window_dfs_algorithm()
{
    bool ok = false;
  QString origine = QInputDialog::getText(this, "DFS Algorithm", "type the origine node's name",QLineEdit::Normal, QString(),&ok);
  if (ok && !origine.isEmpty() && existing_name_nodes.contains(origine))
   {
      graph->dfs(origine);
    }
  else if(ok && !origine.isEmpty() && !existing_name_nodes.contains(origine))
  {
      QMessageBox::critical(this, "Origine not found", "The node you typed does not exist, try again !");
  }
}

//********************** delete edge ****************
void window::open_window_delete_edge()
{
    QString origine_edge_to_delete;
    QString destination_edge_to_delete;
    bool ok = false;
  origine_edge_to_delete = QInputDialog::getText(this, "DELETE EDGE", "Type the name of origine node",QLineEdit::Normal, QString(),&ok);
  if (ok && !origine_edge_to_delete.isEmpty() && existing_name_nodes.contains(origine_edge_to_delete))
   {
      ok=false;
      destination_edge_to_delete = QInputDialog::getText(this, "DELETE EDGE", "Type the name of destination node",QLineEdit::Normal, QString(),&ok);
      if (ok && !destination_edge_to_delete.isEmpty() &&existing_name_nodes.contains(destination_edge_to_delete) && (origine_edge_to_delete.compare(destination_edge_to_delete) != 0))
      {
          if(graph->verify_edge_existence(origine_edge_to_delete,destination_edge_to_delete))
          {
             graph->delete_edge(origine_edge_to_delete,destination_edge_to_delete);
      }
          else
          {
               QMessageBox::critical(this, "No Edge ", "No Edge between chosen nodes !");
          }
}


      else if (ok && !destination_edge_to_delete.isEmpty() && !existing_name_nodes.contains(destination_edge_to_delete))
      {
           QMessageBox::critical(this, "Node not found", "Destination node does not exist !");
      }
      else if (ok && !destination_edge_to_delete.isEmpty() && (origine_edge_to_delete.compare(destination_edge_to_delete) == 0) )
      {
           QMessageBox::critical(this, "Same node used ", "destination node and origine node should not be the same !");
      }


    }
  else if (ok && !origine_edge_to_delete.isEmpty() && !existing_name_nodes.contains(origine_edge_to_delete))
  {
       QMessageBox::critical(this, "Node not found", "Origine node does not exist !");
  }

}

//********************** delete node ****************
void window::open_window_delete_node()
{
    bool ok = false;
    QString node_to_delete = QInputDialog::getText(this, "Delete node", "type the name of node to delete, you have to remove all incoming and outgoing edges first ",QLineEdit::Normal, QString(),&ok);
  if (ok && !node_to_delete.isEmpty() &&  existing_name_nodes.contains(node_to_delete))
   {
      if (!graph->connected_node(node_to_delete))
      {
          existing_name_nodes.removeOne(node_to_delete);
          graph->delete_node(node_to_delete);

      }
    else
      {
          QMessageBox::critical(this, "Remove edges", "Node still have outgoing or incoming edges ");
      }

    }
  else if(ok && !node_to_delete.isEmpty() && !existing_name_nodes.contains(node_to_delete))
  {
      QMessageBox::critical(this, "Node not found", "Node not found , try again !");
  }
}
