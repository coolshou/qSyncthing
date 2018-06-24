#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //a.aboutToQuit(w.syncthing_stop());
    return a.exec();
}
