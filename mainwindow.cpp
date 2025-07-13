#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qsettingsmanager.h"
#include "stopwatchinteractiveeditor.h"
#include "stylesheetgenerator.h"
#include "systemtimemodule.h"

#include <QHotkey>
#include <QMouseEvent>
#include <QtConcurrent>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    qsm{},
    swm{this},
    qhm{this}
{
    ui->setupUi(this);

    ui->StopwatchLabel->setStyleSheet(QString("QLabel { background-color : black; color : %1; }").arg(StylesheetGenerator::DefaultFontHexColor));
    connect(&swm, &StopwatchManager::updateElapsedTime, this, &MainWindow::updateElapsedTime);
    UpdateStopwatchFont(qsm.FetchStopwatchFont(),GetCurrentFont().pointSize());
    QDir dir;
    SetStopwatchValue(QString("Welcome, %1. Stopwatch ready...").arg(dir.home().dirName()));
}

void MainWindow::SetStopwatchValue(QString text)
{
    ui->StopwatchLabel->setText(text);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::FetchStopwatchFontColorAsHex()
{
    return ui->StopwatchLabel->palette().color(QPalette::WindowText).name();
}

void MainWindow::UpdateStopwatchFont(QString fontName, int fontSize)
{
    ui->StopwatchLabel->setFont(QFont(fontName, fontSize));
}

QFont MainWindow::GetCurrentFont()
{
    return ui->StopwatchLabel->font();
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::MouseButton::RightButton)) {
    const QPointF delta = event->globalPosition() - oldPosition;
    move(x()+delta.x(), y()+delta.y());
    oldPosition = event->globalPosition();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::MouseButton::RightButton)
        oldPosition = event->globalPosition();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::RightButton && StopwatchInteractiveEditor::instances < 1) {
        auto *sie = new StopwatchInteractiveEditor(nullptr, this);
        sie->setAttribute(Qt::WA_DeleteOnClose);
        sie->setWindowFlags(sie->windowFlags()
                            | Qt::FramelessWindowHint
                             );
        sie->show();

        auto *stmodule = new SystemTimeModule(nullptr, this);
        stmodule->setAttribute(Qt::WA_DeleteOnClose);
        stmodule->setAttribute(Qt::WA_TranslucentBackground);
        stmodule->setWindowFlags(stmodule->windowFlags()
                            | Qt::FramelessWindowHint
                            | Qt::WindowStaysOnTopHint
                            );
        stmodule->show();
    }
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

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    UpdateStopwatchFont(qsm.FetchStopwatchFont(),GetCurrentFont().pointSize());
}

void MainWindow::updateElapsedTime(const int &time) {
    ui->StopwatchLabel->setText(FormatTime(time));
    ResizeWindowToFitStopwatch();
}
