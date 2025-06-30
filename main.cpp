#include "mainwindow.h"

#include <QApplication>
#include <QHotkey>
#include <QtDebug>
#include <QSettings>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowFlag(Qt::WindowStaysOnTopHint, true);
    w.setWindowFlag(Qt::FramelessWindowHint, true);
    w.setAttribute(Qt::WA_TranslucentBackground, true);

    w.show();
    return a.exec();
}
