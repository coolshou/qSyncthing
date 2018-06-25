#include "mainwindow.h"
#include <QApplication>

#include "const.h"
#include "version.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(APPNAME);
    app.setApplicationVersion(APPVERSION);
    app.setOrganizationDomain(APPORG);

    MainWindow w;
    w.show();
    //app.aboutToQuit(w.syncthing_stop());
    return app.exec();
}
