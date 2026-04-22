#include "../include/mainwindow.h"

#include <sodium.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (sodium_init() < 0) {
        qDebug() << "libsodium failed to initialize";
        return 1;
    }

    MainWindow w;
    // show mainWindow maximized
    w.showMaximized();
    return QCoreApplication::exec();
}
