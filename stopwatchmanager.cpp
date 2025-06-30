#include "stopwatchmanager.h"
#include "mainwindow.h"

#include <QDebug>
#include <QtConcurrent>

StopwatchManager::StopwatchManager(MainWindow *mwr)
    : pauseStopwatch{true},
    isDeconstructing{false},
    elapsedSeconds{},
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
    if (pauseStopwatch) {
        elapsedSeconds = 0;
        mw->SetStopwatchValue("0:00");
    }
    else {
        pauseStopwatch = true;
        elapsedSeconds = 0;
        pauseStopwatch = false;
    }

}

void StopwatchManager::StopwatchThread()
{
    qDebug() << "Stopwatch Thread initialized";
    while (!isDeconstructing) {
            QThread::msleep(500);
        while (!pauseStopwatch && !isDeconstructing) {
            QThread::msleep(1000);
            elapsedSeconds += 1;
            emit updateElapsedTime(elapsedSeconds);
    }}
}
