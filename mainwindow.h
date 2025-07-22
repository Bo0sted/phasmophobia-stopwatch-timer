#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>

// Forward declaration
class StopwatchManager;
class HotkeyManager;
class SystemTimeModule;
class StopwatchInteractiveEditor;
class UpdateManager;

#include "qsettingsmanager.h"
#include "stopwatchmanager.h"
#include "qhotkeymanager.h"
#include "systemtimemodule.h"
#include "stopwatchinteractiveeditor.h"
#include "updatemanager.h"


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
    QHotkeyManager qhm;
    SystemTimeModule *stm;
    StopwatchInteractiveEditor *sie;
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
    void SetBackgroundEnabled(bool enabled);
    bool IsBackgroundEnabled();
    QFont GetCurrentFont();
    QString GetActiveStopwatchStyleSheet();
    int GetCurrentStopwatchFontSize();
    void BeginShutdown();
    QString FetchUUID();
    int FetchLastPingUnix();
    int FetchLastUpdateCheckUnix();
    void SetLastPingUnix(int ts);
    void SetLastUpdateCheckUnix(int ts);

public slots:
    void updateElapsedTime(const int &time);
signals:
    void toggleModuleSignal(bool shouldEnable);
    void toggleEditorSignal(bool shouldOpen);
private:
    Ui::MainWindow *ui;
    QPointF oldPosition;
    QFuture<void> stopwatchUpdateRespondThread;
    void ResizeWindowToFitStopwatch();
    QString FormatTime(int totalSeconds);
    QColor stopwatchFontColor;
    QColor pausedStopwatchFontColor;
    QColor resetStopwatchFontColor;
    QColor stopwatchBackgroundColor;
    QColor stopwatchBorderColor;
    QColor stopwatchBorderThickness;
    bool backgroundEnabled;
    QString uuid;
    int lastPingUnix;
    int lastUpdateCheckUnix;
    UpdateManager um;
protected:
    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    bool event(QEvent *event) override;


};
#endif // MAINWINDOW_H
