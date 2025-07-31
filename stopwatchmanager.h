#ifndef STOPWATCHMANAGER_H
#define STOPWATCHMANAGER_H

#include <QFuture>
#include <atomic>

// Forward declaration
class MainWindow;

class StopwatchManager : public QObject
{
    Q_OBJECT
public:
    enum FormatModes {
        HourMinuteSecond,
        TotalMinutesOnly,
        TotalSecondsOnly

    };

    std::atomic<bool> pauseStopwatch;
    std::atomic<bool> rainbowModeOn;
    std::atomic<bool> rainbowModeBackgroundOn;

    StopwatchManager(MainWindow *mwr);
    ~StopwatchManager();
    void ResetStopwatch();
    std::atomic<int> elapsedSeconds;
signals:
    void updateElapsedTime(const int &time); // Signal to update elapsed time
    void updateRainbowColor(const QColor& color);
    void updateRainbowBackgroundColor(const QColor& color);
private:
    MainWindow *mw;
    std::atomic<bool> isDeconstructing;
    QFuture<void> stopwatchThread;
    QFuture<void> rainbowModeThread;

    void StopwatchThread();
    void RainbowModeThread();
};

#endif // STOPWATCHMANAGER_H
