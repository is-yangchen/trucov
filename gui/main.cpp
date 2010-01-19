#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap("monk.png");
    QSplashScreen *splash = new QSplashScreen( pixmap );
    MainWindow w;
    w.show();
    splash->show();
    splash->showMessage("Wait! Program Loading......");
    qApp->processEvents();
    sleep(3);
    delete splash;


    //splash->finish(&w);
    return a.exec();
}
