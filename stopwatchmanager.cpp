#include "stopwatchmanager.h"
#include "mainwindow.h"

#include <QDebug>
#include <QtConcurrent>

StopwatchManager::StopwatchManager(MainWindow *mwr)
    : pauseStopwatch{true},
    rainbowModeOn{false},
    rainbowModeBackgroundOn{false},
    isDeconstructing{false},
    elapsedSeconds{0},
    stopwatchThread(QtConcurrent::run(&StopwatchManager::StopwatchThread, this)),
    rainbowModeThread(QtConcurrent::run(&StopwatchManager::RainbowModeThread, this))
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
    mw->RefreshStopwatchState(true);

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

void StopwatchManager::RainbowModeThread()
{
    qreal hue = 0.0;
    qreal t = 0.0;
    while (!isDeconstructing) {
        while ((rainbowModeOn == true || rainbowModeBackgroundOn == true) && !isDeconstructing) {
            qreal glowValue = 0.2 * std::sin(t) + 0.8;
            QColor color;
            color.setHsvF(hue, 1.0,1.0); //glowValue);  // Hue in range [0, 1]
                    //qDebug() << "[Thread] Emitting:" << color.name();

            if (rainbowModeBackgroundOn)
                emit updateRainbowBackgroundColor(color);
            else if (rainbowModeOn)
                emit updateRainbowColor(color);

            hue += 0.002;  // Smooth step
            if (hue > 1.0) hue -= 1.0;
            //t += 0.05;

            QThread::msleep(25);  // Small delay for smooth update
        }
    }
        //qDebug() << "[Thread] Exiting";
}
