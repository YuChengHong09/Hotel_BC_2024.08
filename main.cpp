#include "mainwindow.h"
#include"logon.h"
#include <QApplication>
#include <QStyleFactory>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));//设置界面风格
    logon logonUI;
    logonUI.show();
    return a.exec();
}
