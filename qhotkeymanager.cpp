#include "mainwindow.h"
#include "qhotkeymanager.h"
#include "qsettingsmanager.h"
class MainWindow;

#include <QHotkey>
#include <QMessageBox>

const QString QHotkeyManager::DefaultToggleStopwatchHotkey = "1";
const QString QHotkeyManager::DefaultResetStopwatchHotkey = "2";
const QString QHotkeyManager::DefaultBringToForegroundHotkey = "3";

QHotkeyManager::QHotkeyManager(MainWindow *mwr)
    :mw{mwr},
    ToggleStopwatchHotkey{FetchToggleStopwatchHotkey()},
    ResetStopwatchHotkey{FetchResetStopwatchHotkey()},
    BringToForegroundStopwatchHotkey{FetchBringToForegroundHotkey()}
{
    toggleStopwatchHotkey = new QHotkey(QKeySequence(ToggleStopwatchHotkey), true, qApp);
    resetStopwatchHotkey = new QHotkey(QKeySequence(ResetStopwatchHotkey), true, qApp);
    bringToForegroundStopwatchHotkey = new QHotkey(QKeySequence(BringToForegroundStopwatchHotkey), true,qApp);

    QObject::connect(toggleStopwatchHotkey, &QHotkey::activated, qApp, [&](){
        ToggleStopwatch();
    });

    QObject::connect(resetStopwatchHotkey, &QHotkey::activated, qApp, [&](){
        ResetStopwatch();
    });

    QObject::connect(bringToForegroundStopwatchHotkey, &QHotkey::activated, qApp, [&](){
        BringToForegroundStopwatch();
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

QString QHotkeyManager::FetchBringToForegroundHotkey()
{
    QSettingsManager *qsm = &mw->qsm;
    auto hotkey = qsm->getValue(qsm->BringToForeground);

    if (hotkey == false) return DefaultBringToForegroundHotkey;
    else return hotkey.toString();
}

bool QHotkeyManager::ReassignHotkey(AvailableHotkeys ah, QString hotkey)
{
    switch(ah) {
    case ToggleKey: {
        ToggleStopwatchHotkey = hotkey;
        return true;
    }
    case ResetKey: {
        ResetStopwatchHotkey = hotkey;
        return true;
    }
    case BringToForeground: {
        BringToForegroundStopwatchHotkey = hotkey;
        return true;
    }
    }
}

void QHotkeyManager::ReassignHotkey(AvailableHotkeys ah, bool shouldUnassign)
{
    switch(ah) {
    case ToggleKey: {
        if (shouldUnassign) {
            toggleStopwatchHotkey->disconnect();
            delete toggleStopwatchHotkey;
        }
        else {
            toggleStopwatchHotkey = new QHotkey(QKeySequence(ToggleStopwatchHotkey), true, qApp);
            QObject::connect(toggleStopwatchHotkey, &QHotkey::activated, qApp, [&](){
                ToggleStopwatch();
            });
        }
        return;
    }
    case ResetKey: {
        if (shouldUnassign) {
            resetStopwatchHotkey->disconnect();
            delete resetStopwatchHotkey;
        }
        else {
            resetStopwatchHotkey = new QHotkey(QKeySequence(ResetStopwatchHotkey), true, qApp);
            QObject::connect(resetStopwatchHotkey, &QHotkey::activated, qApp, [&](){
                ResetStopwatch();
            });
        }
        return;
    }
    case BringToForeground: {
        if (shouldUnassign) {
            bringToForegroundStopwatchHotkey->disconnect();
            delete bringToForegroundStopwatchHotkey;
        }
        else {
            bringToForegroundStopwatchHotkey = new QHotkey(QKeySequence(BringToForegroundStopwatchHotkey), true, qApp);
            QObject::connect(bringToForegroundStopwatchHotkey, &QHotkey::activated, qApp, [&](){
                BringToForegroundStopwatch();
            });
        }
        return;
    }
    }
}

bool QHotkeyManager::IsHotkeyAvailable(QString hotkey, bool shouldAlertUser)
{
    if (hotkey == "") {
        if (shouldAlertUser) {
            QMessageBox msgBox;
            msgBox.setText(QString("No hotkey was inputted in the box. Please try again."));
            msgBox.exec();
        }
        return false;
    }
    if (hotkey == ToggleStopwatchHotkey) {
        if (shouldAlertUser) {
            QMessageBox msgBox;
            msgBox.setText(QString("Sorry, %1 is currently assigned to %2").arg(hotkey, mw->qsm.SettingsForHotkeyGroup[mw->qsm.ToggleKey]));
            msgBox.exec();
        }
        return false;
    }
    if (hotkey == ResetStopwatchHotkey) {
        if (shouldAlertUser) {
            QMessageBox msgBox;
            msgBox.setText(QString("Sorry, %1 is currently assigned to %2").arg(hotkey, mw->qsm.SettingsForHotkeyGroup[mw->qsm.ResetKey]));
            msgBox.exec();
        }
        return false;
    }
    if (hotkey == BringToForegroundStopwatchHotkey) {
        if (shouldAlertUser) {
            QMessageBox msgBox;
            msgBox.setText(QString("Sorry, %1 is currently assigned to %2").arg(hotkey, mw->qsm.SettingsForHotkeyGroup[mw->qsm.BringToForeground]));
            msgBox.exec();
        }
        return false;
    }

    return true;
}

void QHotkeyManager::UpdateHotkeySignalBlock(bool shouldBlockSignal)
{
    if (shouldBlockSignal) {
        ReassignHotkey(ToggleKey);
        ReassignHotkey(ResetKey);
        ReassignHotkey(BringToForeground);
    }
    else {
        ReassignHotkey(ToggleKey, false);
        ReassignHotkey(ResetKey, false);
        ReassignHotkey(BringToForeground, false);
    }
}

void QHotkeyManager::ToggleStopwatch()
{
    mw->swm.pauseStopwatch = !mw->swm.pauseStopwatch;
}

void QHotkeyManager::ResetStopwatch()
{
    mw->swm.ResetStopwatch();
}

void QHotkeyManager::BringToForegroundStopwatch()
{
    mw->show();
    mw->activateWindow();
    mw->raise();
}


