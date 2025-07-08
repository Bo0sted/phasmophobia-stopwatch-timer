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
    pauseStopwatch = true;
    elapsedSeconds = 0;
    mw->SetStopwatchValue("0:00");

}

void StopwatchManager::StopwatchThread()
{
    qDebug() << "Stopwatch Thread initialized";
    while (!isDeconstructing) {
        while (!pauseStopwatch && !isDeconstructing) {
            QThread::msleep(1000);
            if (pauseStopwatch)
                break;
            elapsedSeconds += 1;
            emit updateElapsedTime(elapsedSeconds);
    }}
}
