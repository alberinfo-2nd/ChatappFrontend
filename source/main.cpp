#include "../include/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // show mainWindow maximized
    w.showMaximized();
    return QCoreApplication::exec();
}
