#include <QApplication>
#include <QTime>
#include "window.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    window mainWindow;
     mainWindow.show();
    return app.exec();
}
