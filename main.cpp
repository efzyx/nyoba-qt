#include "login.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    MainWindow mw;
//    mw.show();
    Login w;
    w.setModal(true);
//    w.hide();
    w.show();
    return a.exec();
}
