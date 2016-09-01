#include "window_delete_edge.h"
window_delete_edge::window_delete_edge():QDialog()

{
layoutPrincipal = new QVBoxLayout;
formLayout_delete_edge = new QFormLayout;
origine = new QLineEdit;
destination = new QLineEdit;
formLayout_delete_edge->addRow(tr("Origine:"), origine);
formLayout_delete_edge->addRow(tr("Destination:"), destination);
bouton_delete = new QPushButton("Delete edge");
layoutPrincipal->addLayout(formLayout_delete_edge);
layoutPrincipal->addWidget(bouton_delete); // Ajout du bouton
setLayout(layoutPrincipal);
}
