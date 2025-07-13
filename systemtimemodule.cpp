#include "systemtimemodule.h"
#include "ui_systemtimemodule.h"
#include "mainwindow.h"
#include "stylesheetgenerator.h"

#include <QtConcurrent>
#include <QTime>
#include <QFont>

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
    // ui->SystemTimeLabel->setStyleSheet(QString(
    //                             " %1").arg(
    //                             StylesheetGenerator::DefaultHeaderTextDejaVu(25, "#EE2C2C", "black")
    //                                            ));
    ReflectStopwatchFont();
}

void SystemTimeModule::closeEvent(QCloseEvent *event)
{
    QString spos = QString("%1,%2").arg(this->pos().x()).arg(this->pos().y());
    mw->qsm.setValue(QSettingsManager::LastSystemClockPosition, spos);
    event->accept();
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

void SystemTimeModule::ResizeClockToFitWindow()
{

    ui->SystemTimeLabel->adjustSize();

    QSize centralSize = ui->SystemTimeLabel->sizeHint();

    // Calculate frame size difference (window decorations)
    int frameWidth = this->frameGeometry().width() - this->geometry().width();
    int frameHeight = this->frameGeometry().height() - this->geometry().height();

    // Resize main window explicitly
    this->resize(centralSize.width() + frameWidth,
                 centralSize.height() + frameHeight);
}

void SystemTimeModule::ReflectStopwatchFont()
{
    ui->SystemTimeLabel->setStyleSheet(mw->GetActiveStopwatchStyleSheet());
    ui->SystemTimeLabel->setFont(QFont(mw->GetCurrentFont().family(), mw->GetCurrentStopwatchFontSize()));
}

void SystemTimeModule::RefreshClockThread()
{
    qDebug() << "System Time Module initialized";
        while (!isDeconstructing) {
            QThread::msleep(1000);
            QString currentTimeString = QTime::currentTime().toString("h:mm ap");
            emit signalRefreshClock(currentTimeString);
        }
}

void SystemTimeModule::updateClock(const QString &time)
{
    ui->SystemTimeLabel->setText(time);
    ResizeClockToFitWindow();
}
