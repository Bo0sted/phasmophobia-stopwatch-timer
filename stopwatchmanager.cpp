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
    elapsedSeconds = 0;
    emit updateElapsedTime(elapsedSeconds);
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
            // Smooth oscillation values for each RGB channel
            qreal r = 0.5 + 0.5 * std::sin(t);
            qreal g = 0.5 + 0.5 * std::sin(t + 2.094); // +120° phase shift
            qreal b = 0.5 + 0.5 * std::sin(t + 4.188); // +240° phase shift

            QColor color;
            color.setRgbF(r, g, b);

            if (rainbowModeBackgroundOn)
                emit updateRainbowBackgroundColor(color);
            else if (rainbowModeOn)
                emit updateRainbowColor(color);

            hue += 0.002; // keeps hue variable updated (if used elsewhere)
            if (hue > 1.0) hue -= 1.0;

            t += 0.05; // advances the RGB animation smoothly

            QThread::msleep(45);
        }
    }
}

