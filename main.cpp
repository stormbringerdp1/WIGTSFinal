#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//--
    QApplication::setStyle("windowsvista"); // "windows" // "fusion" // "macintosh" // "gtk" //  "windowsxp"
//--
    MainWindow w;     w.Init();
    w.show();

    return a.exec();
}
