#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>


#include "qsettingsmanager.h"
#include "stopwatchmanager.h"
#include "qhotkeymanager.h"
#include "systemtimemodule.h"

// Forward declaration
class StopwatchManager;
class HotkeyManager;
class SystemTimeModule;

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

    MainWindow(QWidget *parent = nullptr);
    void SetStopwatchValue(QString text);
    ~MainWindow();
    QString FetchStopwatchFontColorAsHex();
    void UpdateStopwatchFont(QString fontName, int fontSize);
    void UpdateStopwatchColor(QColor color);
    QFont GetCurrentFont();
    QString GetActiveStopwatchStyleSheet();
    int GetCurrentStopwatchFontSize();
    const qint64 Uptime;

public slots:
    void updateElapsedTime(const int &time);
signals:
    void toggleModuleSignal(bool shouldEnable);
private:
    Ui::MainWindow *ui;
    QPointF oldPosition;
    QFuture<void> stopwatchUpdateRespondThread;
    void ResizeWindowToFitStopwatch();
    QString FormatTime(int totalSeconds);
    QColor stopwatchFontColor;
protected:
    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;


};
#endif // MAINWINDOW_H
