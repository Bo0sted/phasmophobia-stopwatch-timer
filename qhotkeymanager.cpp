#include "mainwindow.h"
#include "qhotkeymanager.h"
#include "qsettingsmanager.h"
class MainWindow;

#include <QHotkey>

const QString QHotkeyManager::DefaultToggleStopwatchHotkey = "1";
const QString QHotkeyManager::DefaultResetStopwatchHotkey = "2";

QHotkeyManager::QHotkeyManager(MainWindow *mwr)
    :mw{mwr},
    ToggleStopwatchHotkey{FetchToggleStopwatchHotkey()},
    ResetStopwatchHotkey{FetchResetStopwatchHotkey()}
{
    toggleStopwatchHotkey = new QHotkey(QKeySequence(ToggleStopwatchHotkey), true, qApp);
    resetStopwatchHotkey = new QHotkey(QKeySequence(ResetStopwatchHotkey), true, qApp);

    QObject::connect(toggleStopwatchHotkey, &QHotkey::activated, qApp, [&](){
        ToggleStopwatch();
    });

    QObject::connect(resetStopwatchHotkey, &QHotkey::activated, qApp, [&](){
        ResetStopwatch();
    });
}

QString QHotkeyManager::FetchToggleStopwatchHotkey()
{
    QSettingsManager *qsm = &mw->qsm;
    auto hotkey = qsm->getValue(qsm->ToggleKey);

    if (hotkey == false) return DefaultToggleStopwatchHotkey;
    else return hotkey.toString();
}

QString QHotkeyManager::FetchResetStopwatchHotkey()
{
    QSettingsManager *qsm = &mw->qsm;
    auto hotkey = qsm->getValue(qsm->ResetKey);

    if (hotkey == false) return DefaultResetStopwatchHotkey;
    else return hotkey.toString();
}

void QHotkeyManager::ToggleStopwatch()
{
    mw->swm.pauseStopwatch = !mw->swm.pauseStopwatch;
}

void QHotkeyManager::ResetStopwatch()
{
    mw->swm.ResetStopwatch();
}


