#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMouseEvent>

#include "qsettingsmanager.h"
#include "stopwatchmanager.h"
#include "qhotkeymanager.h"

// Forward declaration
class StopwatchManager;
class HotkeyManager;

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

    MainWindow(QWidget *parent = nullptr);
    void SetStopwatchValue(QString text);
    ~MainWindow();
public slots:
    void updateElapsedTime(const int &time);
private:
    Ui::MainWindow *ui;
    QPointF oldPosition;
    QFuture<void> stopwatchUpdateRespondThread;

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void ResizeWindowToFitStopwatch();
    QString FormatTime(int totalSeconds);
};
#endif // MAINWINDOW_H
