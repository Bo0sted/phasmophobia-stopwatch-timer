#include "systemtimemodule.h"
#include "ui_systemtimemodule.h"
#include "mainwindow.h"
#include "stylesheetgenerator.h".h"

#include <QtConcurrent>
#include <QTime>

SystemTimeModule::SystemTimeModule(QWidget *parent, MainWindow *mwr)
    : QWidget(parent)
    , ui(new Ui::SystemTimeModule)
    , mw(mwr),
    isDeconstructing{false},
    refreshClockThread(QtConcurrent::run(&SystemTimeModule::RefreshClockThread, this))
{
    ui->setupUi(this);
    connect(this, &SystemTimeModule::signalRefreshClock, this, &SystemTimeModule::updateClock);
}

SystemTimeModule::~SystemTimeModule()
{
    isDeconstructing = true;
    refreshClockThread.cancel();
    refreshClockThread.waitForFinished();
    delete ui;
}

void SystemTimeModule::showEvent(QShowEvent *event)
{
    ui->SystemTimeLabel->setStyleSheet(QString(
                                " %1").arg(
                                StylesheetGenerator::DefaultHeaderTextDejaVu(25, "#EE2C2C", "black")
                                               ));
}

void SystemTimeModule::closeEvent(QCloseEvent *event)
{

}

void SystemTimeModule::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::MouseButton::RightButton)) {
        const QPointF delta = event->globalPosition() - oldPosition;
        move(x()+delta.x(), y()+delta.y());
        oldPosition = event->globalPosition();
    }
}

void SystemTimeModule::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::MouseButton::RightButton)
        oldPosition = event->globalPosition();
}

void SystemTimeModule::RefreshClockThread()
{
    qDebug() << "System Time Module initialized";
        while (!isDeconstructing) {
            QThread::msleep(1000);
            QString currentTimeString = QTime::currentTime().toString();
            emit signalRefreshClock(currentTimeString);
        }
}

void SystemTimeModule::updateClock(const QString &time)
{
    ui->SystemTimeLabel->setText(time);
}
