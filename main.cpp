#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");

    MainWindow w;
    w.setWindowTitle ("GRIFON Graphical User Interface");
    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
