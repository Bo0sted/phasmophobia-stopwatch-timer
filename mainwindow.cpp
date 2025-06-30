#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qsettingsmanager.h"

#include <QHotkey>
#include <QMouseEvent>
#include <QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    qsm{},
    swm{this},
    qhm{this}
{
    ui->setupUi(this);
    ui->StopwatchLabel->setStyleSheet("QLabel { background-color : black; color : blue; }");
    connect(&swm, &StopwatchManager::updateElapsedTime, this, &MainWindow::updateElapsedTime);

    SetStopwatchValue(QString("Press %1 to begin Stopwatch\n"\
                              "Don't forget pressing %2 will reset "\
                              "your stopwatch").arg(qhm.FetchToggleStopwatchHotkey(), qhm.FetchResetStopwatchHotkey()));
}

void MainWindow::SetStopwatchValue(QString text)
{
    ui->StopwatchLabel->setText(text);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    const QPointF delta = event->globalPosition() - oldPosition;
    move(x()+delta.x(), y()+delta.y());
    oldPosition = event->globalPosition();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    oldPosition = event->globalPosition();
}

void MainWindow::ResizeWindowToFitStopwatch()
{
    ui->StopwatchLabel->adjustSize();
    ui->centralwidget->adjustSize();

    QSize centralSize = ui->centralwidget->sizeHint();

    // Calculate frame size difference (window decorations)
    int frameWidth = this->frameGeometry().width() - this->geometry().width();
    int frameHeight = this->frameGeometry().height() - this->geometry().height();

    // Resize main window explicitly
    this->resize(centralSize.width() + frameWidth,
                 centralSize.height() + frameHeight);
}

QString MainWindow::FormatTime(int totalSeconds)
{
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    if (hours > 0) {
        // Format as H:MM:SS
        return QString("%1:%2:%3")
            .arg(hours)
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    } else {
        // Format as M:SS
        return QString("%1:%2")
            .arg(minutes)
            .arg(seconds, 2, 10, QChar('0'));
    }
}

void MainWindow::updateElapsedTime(const int &time) {
    ui->StopwatchLabel->setText(FormatTime(time));
    ResizeWindowToFitStopwatch();
}
