#include "stopwatchmanager.h"
#include "mainwindow.h"

#include <QDebug>
#include <QtConcurrent>

StopwatchManager::StopwatchManager(MainWindow *mwr)
    : pauseStopwatch{true},
    isDeconstructing{false},
    elapsedSeconds{0},
    stopwatchThread(QtConcurrent::run(&StopwatchManager::StopwatchThread, this))
{
    mw = mwr;
}

StopwatchManager::~StopwatchManager()
{
    isDeconstructing = true;
    stopwatchThread.cancel();
    stopwatchThread.waitForFinished();
}

void StopwatchManager::ResetStopwatch()
{
    pauseStopwatch = true;
    mw->SetStopwatchValue("0:00");
    elapsedSeconds = 0;
    mw->RefreshStopwatchColor(true);

}

void StopwatchManager::StopwatchThread()
{
    qDebug() << "Stopwatch Thread initialized";
    while (!isDeconstructing) {
        while (!pauseStopwatch && !isDeconstructing) {
            if (pauseStopwatch)
                break;
            elapsedSeconds += 1;
            emit updateElapsedTime(elapsedSeconds);
            QThread::msleep(1000);
    }}
}
