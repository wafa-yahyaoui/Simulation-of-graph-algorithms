#ifndef WINDOW_DELETE_EDGE_H
#define WINDOW_DELETE_EDGE_H
#include <QtWidgets>
#include <QApplication>
#include <QFormLayout>
class window_delete_edge : public QDialog
{
public:
    window_delete_edge();
private:
    QVBoxLayout *layoutPrincipal;
    QFormLayout *formLayout_delete_edge;
    QLineEdit *origine;
    QLineEdit *destination;
    QPushButton *bouton_delete;
};
#endif // WINDOW_DELETE_EDGE_H
