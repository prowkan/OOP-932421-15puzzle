#include <QApplication>

#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    MainWindow mainwindow;
    mainwindow.show();
    return application.exec();
}