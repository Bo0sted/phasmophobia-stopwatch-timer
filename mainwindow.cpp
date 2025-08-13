#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qsettingsmanager.h"
#include "stopwatchinteractiveeditor.h"
#include "stylesheetgenerator.h"
#include "systemtimemodule.h"
#include "uioeventloop.h"
#include <uiokey.h>

#include <QHotkey>
#include <QMouseEvent>
#include <QtConcurrent>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <array>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    qsm{},
    swm{this},
    // qhm{this},
    um{this},
    uioel{this},
    uiohm{this, &uioel},
    stopwatchFontColor(qsm.FetchStopwatchFontColor()),
    pausedStopwatchFontColor(qsm.FetchPausedStopwatchFontColor()),
    resetStopwatchFontColor(qsm.FetchResetStopwatchFontColor()),
    stopwatchBackgroundColor(qsm.FetchStopwatchBackgroundColor()),
    stopwatchBorderColor(qsm.FetchStopwatchBorderColor()),
    stopwatchBorderThickness(qsm.FetchStopwatchBorderWidth()),
    stopwatchGradientOneColor(qsm.FetchGradientOneFontColor()),
    stopwatchGradientTwoColor(qsm.FetchGradientTwoFontColor()),
    gradientEnabled(qsm.FetchIsGradientEnabled()),
    backgroundEnabled(qsm.FetchIsBackgroundEnabled()),
    Uptime(QDateTime::currentMSecsSinceEpoch()),
    uuid(qsm.FetchUUID()),
    lastPingUnix(qsm.FetchLastPingUnix()),
    lastUpdateCheckUnix(qsm.FetchLastUpdateCheckUnix()),
    latestVersion{"<br><u>Recent update check performed already.</u>"},
    stopwatchRainbowMode{(qsm.FetchStopwatchRainbowModeIndex())},
    stopwatchFormatMode(qsm.FetchStopwatchFormatMode()),
    enabled(qsm.FetchIsStopwatchEnabled())

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
    ui->StopwatchLabel->setStyleSheet(StylesheetGenerator::NewModuleOutputStylesheet(stopwatchFontColor, stopwatchBackgroundColor));
    // qDebug() << "Applying color to StopwatchLabel:" << color.name()
    //          << "Valid?" << color.isValid();
}

void MainWindow::RefreshStopwatchState(bool shouldReset)
{
    if (swm.pauseStopwatch == true && !shouldReset) {
        ui->StopwatchLabel->setStyleSheet(StylesheetGenerator::NewModuleOutputStylesheet(pausedStopwatchFontColor, stopwatchBackgroundColor, stopwatchBorderColor));
    }
    // This statement should go above the last one because IF the user has a custom reset/initial state set, then itll be set here
    else if (swm.pauseStopwatch == true && shouldReset){
        ui->StopwatchLabel->setStyleSheet(StylesheetGenerator::NewModuleOutputStylesheet(resetStopwatchFontColor, stopwatchBackgroundColor, stopwatchBorderColor));
    }
    else
        ui->StopwatchLabel->setStyleSheet(StylesheetGenerator::NewModuleOutputStylesheet(stopwatchFontColor, stopwatchBackgroundColor, stopwatchBorderColor));

    if (!backgroundEnabled) {
        ui->StopwatchLabel->setAttribute(Qt::WA_NoSystemBackground);
        ui->StopwatchLabel->setAutoFillBackground(false);
        ui->StopwatchLabel->setStyleSheet(QString("%1%2").arg(ui->StopwatchLabel->styleSheet()).arg("background-color: rgba(0, 0, 0, 0);"));
    }
    else {
        ui->StopwatchLabel->setAttribute(Qt::WA_NoSystemBackground, false);
    }

    ui->StopwatchLabel->setUseGradient(gradientEnabled);

    if (gradientEnabled)
        ui->StopwatchLabel->setGradientColors(stopwatchGradientOneColor, stopwatchGradientTwoColor);

    ui->StopwatchLabel->update();
    ui->StopwatchLabel->repaint();
    ui->StopwatchLabel->show();

}

void MainWindow::UpdateStopwatchPausedColor(QColor color)
{
    pausedStopwatchFontColor = color;
}

void MainWindow::UpdateStopwatchResetColor(QColor color)
{
    resetStopwatchFontColor = color;
}

void MainWindow::UpdateStopwatchBackground(QColor color)
{
    stopwatchBackgroundColor = color;
}

void MainWindow::UpdateStopwatchBorderColor(QColor color)
{
    stopwatchBorderColor = color;
}

void MainWindow::UpdateStopwatchBorderThickness(QString thickness)
{
    if (!thickness.endsWith("px"))
            thickness = "0px";
    stopwatchBorderThickness = thickness;
}

void MainWindow::SetGradientEnabled(bool enabled)
{
    gradientEnabled = enabled;
}

void MainWindow::SetBackgroundEnabled(bool enabled)
{
    backgroundEnabled = enabled;
}

bool MainWindow::IsBackgroundEnabled() {
    return backgroundEnabled;
}

bool MainWindow::IsGradientEnabled()
{
    return gradientEnabled;
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
    this->close();
}

QString MainWindow::FetchUUID()
{
    return uuid;
}

int MainWindow::FetchLastPingUnix()
{
    return lastPingUnix;
}

int MainWindow::FetchLastUpdateCheckUnix()
{
    return lastUpdateCheckUnix;
}

void MainWindow::SetLastPingUnix(int ts)
{
    lastPingUnix = ts;
    qsm.setValue(QSettingsManager::Logging::LastPingUnix, lastPingUnix);
}

void MainWindow::SetLastUpdateCheckUnix(int ts)
{
    lastUpdateCheckUnix = ts;
    qsm.setValue(QSettingsManager::Logging::LastUpdateCheckUnix, lastUpdateCheckUnix);
}

QString MainWindow::FetchLatestVersion()
{
    return latestVersion;
}

void MainWindow::SetLatestVersion(QString version)
{
    latestVersion = version;
}

bool MainWindow::CheckIfStopwatchEnabled()
{
    return enabled;
}

void MainWindow::SetEnabled(bool shouldEnable)
{
    enabled = shouldEnable;

    if (enabled && !this->isVisible())
        this->show();
    else if (!enabled && !this->isVisible())
        this->hide();
}

int MainWindow::GetRainbowMode()
{
    return stopwatchRainbowMode;
}


void MainWindow::SetRainbowMode(int index )
{
    stopwatchRainbowMode = index;

    switch (stopwatchRainbowMode) {
    case 0: {
        swm.rainbowModeOn = false;
        swm.rainbowModeBackgroundOn = false;
        ui->StopwatchLabel->setStyleSheet(StylesheetGenerator::NewStopwatchStylesheet(stopwatchFontColor.name(), stopwatchBackgroundColor.name()));
        emit updateClockRainbowColor("");
        RefreshStopwatchState(false);
        break;
    }
    case 1: {
        swm.rainbowModeOn = true;
        swm.rainbowModeBackgroundOn = false;
        break;
    }
    case 2: {
        swm.rainbowModeOn = false;
        swm.rainbowModeBackgroundOn = true;
        break;
    }
    }

}

StopwatchManager::FormatModes MainWindow::GetFormatMode()
{
    return stopwatchFormatMode;
}

void MainWindow::SetFormatMode(StopwatchManager::FormatModes format)
{
    stopwatchFormatMode = format;
    RefreshStopwatchState(false);

}

QColor MainWindow::GetBackground()
{
    return stopwatchBackgroundColor;
}

void MainWindow::SetGradients(QColor gradientOne, QColor gradientTwo)
{
    stopwatchGradientOneColor = gradientOne;
    stopwatchGradientTwoColor = gradientTwo;

    if (gradientEnabled)
        RefreshStopwatchState(false);

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
        uiohm.UpdateHotkeySignalBlock(true);
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

std::array<QColor, 2> MainWindow::GetGradientColors()
{
    return {stopwatchGradientOneColor, stopwatchGradientTwoColor};
}

QString MainWindow::FormatTime(int totalSeconds)
{
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    if (stopwatchFormatMode == StopwatchManager::TotalSecondsOnly)
        return QString("%1")
            .arg(totalSeconds);

    if (stopwatchFormatMode == StopwatchManager::TotalMinutesOnly)
        return QString::number(static_cast<double>(totalSeconds) / 60.0, 'f', 1);

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

    QTimer::singleShot(0, this, [this]() {
        setLoadStopwatch(enabled);
    });
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Polish) {
        if (qsm.ShouldPostPing())
            um.PostAnonymousUsageLog();

        if (qsm.ShouldCheckUpdate())
            um.CheckForUpdateAndPromptUser();
        else
            um.SetUpdateCheckSuccessful(true);

        UpdateStopwatchFont(qsm.FetchStopwatchFont(),qsm.FetchStopwatchFontSize());
        //SetStopwatchValue(QString("Stopwatch ready... Press %1 to run or right click this window to configure").arg(uiohm.GetDisplayFromQListOfKeycodes(uiohm.FetchToggleStopwatchHotkey())));
        SetStopwatchValue("0:00");

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
                            | Qt::WindowStaysOnTopHint
                            );
        sie->show();
        sie->RefreshOpenState();


        connect(&uiohm, &UioHotkeyManager::refreshHotkeyDisplays,
                sie, &StopwatchInteractiveEditor::refreshHotkeyDisplays);

        connect(this, &MainWindow::toggleModuleSignal, stm, &SystemTimeModule::setLoadModule);
        if (!qsm.CheckIfClockEnabled())
            emit toggleModuleSignal(false);
        else
            emit toggleModuleSignal(true);

        connect(this, &MainWindow::toggleEditorSignal, sie, &StopwatchInteractiveEditor::setEditorOpen);

        connect(sie, &StopwatchInteractiveEditor::toggleModuleSignal, stm, &SystemTimeModule::setLoadModule);
        connect(sie, &StopwatchInteractiveEditor::toggleStopwatchSignal, this, &MainWindow::setLoadStopwatch);
        connect(this, &MainWindow::updateClockRainbowColor, stm, &SystemTimeModule::refreshColorState);
        connect(&swm, &StopwatchManager::updateClockRainbowColor, stm, &SystemTimeModule::refreshColorState);
        connect(&swm, &StopwatchManager::updateElapsedTime, this, &MainWindow::updateElapsedTime);
        connect(&swm,&StopwatchManager::updateRainbowColor, this, &MainWindow::updateRainbowColor);
        connect(&swm,&StopwatchManager::updateRainbowBackgroundColor, this, &MainWindow::updateRainbowBackgroundColor);

        RefreshStopwatchState(true);
    }
    return QWidget::event(event);
}

void MainWindow::updateElapsedTime(const int &time) {
    ui->StopwatchLabel->setText(FormatTime(time));
    ResizeWindowToFitStopwatch();
}

void MainWindow::updateRainbowColor(const QColor& color) {
    ui->StopwatchLabel->setStyleSheet(StylesheetGenerator::NewStopwatchStylesheet(color.name(),stopwatchBackgroundColor.name()));
}

void MainWindow::updateRainbowBackgroundColor(const QColor &color)
{
    ui->StopwatchLabel->setStyleSheet(StylesheetGenerator::NewStopwatchStylesheet(stopwatchFontColor.name(), color.name()));
}

void MainWindow::setLoadStopwatch(bool shouldEnable)
{
    enabled = shouldEnable;
    if (enabled && !this->isVisible())
        this->setVisible(true);
    else if (!enabled && this->isVisible())
        this->setVisible(false);
}
