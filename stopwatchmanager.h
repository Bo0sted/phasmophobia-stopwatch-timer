#ifndef STOPWATCHMANAGER_H
#define STOPWATCHMANAGER_H

#include <QFuture>

// Forward declaration
class MainWindow;

class StopwatchManager : public QObject
{
    Q_OBJECT
public:
    bool pauseStopwatch;

    StopwatchManager(MainWindow *mwr);
    ~StopwatchManager();
    void ResetStopwatch();
    void StopwatchThread();
    int elapsedSeconds;
signals:
    void updateElapsedTime(const int &time); // Signal to update elapsed time
private:
    MainWindow *mw;
    bool isDeconstructing;
    QFuture<void> stopwatchThread;
};

#endif // STOPWATCHMANAGER_H
