#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qsettingsmanager.h"
#include "stopwatchinteractiveeditor.h"
#include "stylesheetgenerator.h"
#include "systemtimemodule.h"
#include "stopwatchinteractiveeditor.h"

#include <QHotkey>
#include <QMouseEvent>
#include <QtConcurrent>
#include <QDebug>
#include <QDir>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    qsm{},
    swm{this},
    qhm{this},
    stopwatchFontColor(qsm.FetchStopwatchFontColor()),
    Uptime(QDateTime::currentMSecsSinceEpoch())
{
    ui->setupUi(this);
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

void MainWindow::UpdateStopwatchColor(QColor color)
{
    stopwatchFontColor = color;
    ui->StopwatchLabel->setStyleSheet(StylesheetGenerator::NewModuleOutputStylesheet(stopwatchFontColor));
    // qDebug() << "Applying color to StopwatchLabel:" << color.name()
    //          << "Valid?" << color.isValid();
}

QFont MainWindow::GetCurrentFont()
{
    return ui->StopwatchLabel->font();
}

QString MainWindow::GetActiveStopwatchStyleSheet()
{
    return ui->StopwatchLabel->styleSheet();
}

int MainWindow::GetCurrentStopwatchFontSize()
{
    return ui->StopwatchLabel->font().pointSize();
}

void MainWindow::BeginShutdown()
{
    QApplication::quit();
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
    if (event->button() == Qt::MouseButton::RightButton) {
        sie->setVisible(true);
        qhm.UpdateHotkeySignalBlock(true);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QString spos = QString("%1,%2").arg(this->pos().x()).arg(this->pos().y());
    qsm.setValue(QSettingsManager::LastStopwatchPosition, spos);
    stm->close();
    sie->close();
    event->accept();
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

}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Polish) {
        UpdateStopwatchFont(qsm.FetchStopwatchFont(),GetCurrentFont().pointSize());
        SetStopwatchValue(QString("Stopwatch ready... Press %1 to run or right click this window to configure").arg(qhm.FetchToggleStopwatchHotkey()));
        UpdateStopwatchFont(qsm.FetchStopwatchFont(),GetCurrentFont().pointSize());

        ui->StopwatchLabel->setStyleSheet(QString("QLabel { background-color : black; color : %1; }").arg(stopwatchFontColor.name()));
        {
            QPair<float, float> lastKnown = qsm.FetchStopwatchLastPosition();
            this->move(lastKnown.first,lastKnown.second);
            // This forces Qt to update the window. Otherwise move() wont have any effect
            this->raise();
        }

        stm = new SystemTimeModule(nullptr, this);
        stm->setAttribute(Qt::WA_DeleteOnClose);
        stm->setAttribute(Qt::WA_TranslucentBackground);
        stm->setWindowFlags(stm->windowFlags()
                            | Qt::FramelessWindowHint
                            | Qt::WindowStaysOnTopHint
                            );
        QPair<float, float> lastKnown = qsm.FetchSystemClockLastPosition();
        stm->show();
        stm->move(lastKnown.first,lastKnown.second);

        sie = new StopwatchInteractiveEditor(nullptr, this);
        sie->setAttribute(Qt::WA_DeleteOnClose);
        sie->setWindowFlags(sie->windowFlags()
                            | Qt::FramelessWindowHint
                            );
        sie->show();

        connect(this, &MainWindow::toggleModuleSignal, stm, &SystemTimeModule::setLoadModule);
        if (!qsm.CheckIfClockEnabled())
            emit toggleModuleSignal(false);
        else
            emit toggleModuleSignal(true);

        connect(this, &MainWindow::toggleEditorSignal, sie, &StopwatchInteractiveEditor::setEditorOpen);
        emit toggleEditorSignal(false);

        connect(sie, &StopwatchInteractiveEditor::toggleModuleSignal, stm, &SystemTimeModule::setLoadModule);
        connect(&swm, &StopwatchManager::updateElapsedTime, this, &MainWindow::updateElapsedTime);
    }
    return QWidget::event(event);
}

void MainWindow::updateElapsedTime(const int &time) {
    ui->StopwatchLabel->setText(FormatTime(time));
    ResizeWindowToFitStopwatch();
}
