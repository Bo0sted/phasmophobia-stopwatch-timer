#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>
#include <array>

// Forward declaration
class StopwatchManager;
// class HotkeyManager;
class SystemTimeModule;
class StopwatchInteractiveEditor;
class UpdateManager;
class UioEventLoop;
class UioHotkeyManager;

#include "qsettingsmanager.h"
#include "stopwatchmanager.h"
#include "systemtimemodule.h"
#include "stopwatchinteractiveeditor.h"
#include "updatemanager.h"
#include "uioeventloop.h"
#include "uiohotkeymanager.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QSettingsManager qsm;
    StopwatchManager swm;
    // QHotkeyManager qhm;
    SystemTimeModule *stm;
    StopwatchInteractiveEditor *sie;
    UpdateManager um;
private: UioEventLoop uioel;
public:
    UioHotkeyManager uiohm;
    const qint64 Uptime;

    MainWindow(QWidget *parent = nullptr);
    void SetStopwatchValue(QString text);
    ~MainWindow();
    QString FetchStopwatchFontColorAsHex();
    void UpdateStopwatchFont(QString fontName, int fontSize);
    void UpdateStopwatchColor(QColor color);
    void RefreshStopwatchState(bool shouldReset);
    void UpdateStopwatchPausedColor(QColor color);
    void UpdateStopwatchResetColor(QColor color);
    void UpdateStopwatchBackground(QColor color);
    void UpdateStopwatchBorderColor(QColor color);
    void UpdateStopwatchBorderThickness(QString thickness);
    void SetGradientEnabled(bool enabled);
    void SetBackgroundEnabled(bool enabled);
    bool IsBackgroundEnabled();
    bool IsGradientEnabled();
    QFont GetCurrentFont();
    QString GetActiveStopwatchStyleSheet();
    int GetCurrentStopwatchFontSize();
    void BeginShutdown();
    QString FetchUUID();
    int FetchLastPingUnix();
    int FetchLastUpdateCheckUnix();
    void SetLastPingUnix(int ts);
    void SetLastUpdateCheckUnix(int ts);
    QString FetchLatestVersion();
    void SetLatestVersion(QString version);
    bool CheckIfStopwatchEnabled();
    void SetEnabled(bool enabled);

    int GetRainbowMode();
    void SetRainbowMode(int index);
    void ResizeWindowToFitStopwatch();

    std::array<QColor, 2> GetGradientColors();

    StopwatchManager::FormatModes GetFormatMode();
    void SetFormatMode(StopwatchManager::FormatModes fm);
    QColor GetBackground();

    void SetGradients(QColor gradientOne, QColor gradientTwo);

public slots:
    void updateElapsedTime(const int &time);
    void updateRainbowColor(const QColor& color);
    void updateRainbowBackgroundColor(const QColor& color);
    void setLoadStopwatch(bool enabled);
signals:
    void toggleModuleSignal(bool shouldEnable);
    void toggleEditorSignal(bool shouldOpen);
    void updateClockRainbowColor(QColor color);
private:
    Ui::MainWindow *ui;
    QPointF oldPosition;
    QFuture<void> stopwatchUpdateRespondThread;
    QString FormatTime(int totalSeconds);

    QColor stopwatchFontColor;
    QColor pausedStopwatchFontColor;
    QColor resetStopwatchFontColor;
    QColor stopwatchBackgroundColor;
    QColor stopwatchBorderColor;
    QColor stopwatchBorderThickness;
    QColor stopwatchGradientOneColor;
    QColor stopwatchGradientTwoColor;
    bool gradientEnabled;
    bool backgroundEnabled;
    QString uuid;
    int lastPingUnix;
    int lastUpdateCheckUnix;
    QString latestVersion;
    int stopwatchRainbowMode;
    StopwatchManager::FormatModes stopwatchFormatMode;
    bool enabled;
protected:
    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    bool event(QEvent *event) override;


};
#endif // MAINWINDOW_H
