#include "mainwindow.h"

#include <QApplication>
#include <QHotkey>
#include <QtDebug>
#include <QSettings>


int main(int argc, char *argv[])
{
    // This line of code will force your system compositor to use X11 or a compatibility layer like Xwayland
    // This obviously does not apply to Win/Mac
    #ifdef Q_OS_UNIX
        qputenv("QT_QPA_PLATFORM", "xcb");
    #endif

    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowFlag(Qt::WindowStaysOnTopHint, true);
    w.setWindowFlag(Qt::FramelessWindowHint, true);
    w.setAttribute(Qt::WA_TranslucentBackground, true);

    w.show();
    return a.exec();
}
