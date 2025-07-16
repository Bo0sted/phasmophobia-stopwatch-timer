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

    qDebug() << "Toggle hotkey registered:" << toggleStopwatchHotkey->isRegistered();
    qDebug() << "Reset hotkey registered:" << resetStopwatchHotkey->isRegistered();
    qDebug() << "Foreground hotkey registered:" << bringToForegroundStopwatchHotkey->isRegistered();
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

void QHotkeyManager::DeleteHotkey(AvailableHotkeys ah)
{
    switch(ah) {
    case ToggleKey: {
        if (toggleStopwatchHotkey->isRegistered()) {
            toggleStopwatchHotkey->disconnect();
            delete toggleStopwatchHotkey;
        }
        ToggleStopwatchHotkey = DefaultToggleStopwatchHotkey;
        return;
    }
    case ResetKey: {
        if (resetStopwatchHotkey->isRegistered()) {
            resetStopwatchHotkey->disconnect();
            delete resetStopwatchHotkey;
        }
        ResetStopwatchHotkey = DefaultResetStopwatchHotkey;
        return;
    }
    case BringToForeground: {
        if (bringToForegroundStopwatchHotkey->isRegistered()) {
            bringToForegroundStopwatchHotkey->disconnect();
            delete bringToForegroundStopwatchHotkey;
        }
        BringToForegroundStopwatchHotkey = DefaultBringToForegroundHotkey;
        return;
    }
    }
}

void QHotkeyManager::FetchAndAssignHotkey(AvailableHotkeys ah)
{
    DeleteHotkey(ah);

    switch(ah) {
        case ToggleKey: {
            ToggleStopwatchHotkey = FetchToggleStopwatchHotkey();
            toggleStopwatchHotkey = new QHotkey(QKeySequence(ToggleStopwatchHotkey), true, qApp);
            QObject::connect(toggleStopwatchHotkey, &QHotkey::activated, qApp, [&](){
                ToggleStopwatch();
            });
            return;
    }
        case ResetKey: {
            ResetStopwatchHotkey = FetchResetStopwatchHotkey();
            resetStopwatchHotkey = new QHotkey(QKeySequence(ResetStopwatchHotkey), true, qApp);
            QObject::connect(resetStopwatchHotkey, &QHotkey::activated, qApp, [&](){
                ResetStopwatch();
            });
            return;
    }
        case BringToForeground: {
            BringToForegroundStopwatchHotkey = FetchBringToForegroundHotkey();
            bringToForegroundStopwatchHotkey = new QHotkey(QKeySequence(BringToForegroundStopwatchHotkey), true, qApp);
            QObject::connect(bringToForegroundStopwatchHotkey, &QHotkey::activated, qApp, [&](){
                BringToForegroundStopwatch();
            });
            return;
    }
    }
}

void QHotkeyManager::AssignHotkey(AvailableHotkeys ah, QString hotkey)
{
    DeleteHotkey(ah);

    switch(ah) {
        case ToggleKey: {
            ToggleStopwatchHotkey = hotkey;
            toggleStopwatchHotkey = new QHotkey(QKeySequence(ToggleStopwatchHotkey), true, qApp);
            QObject::connect(toggleStopwatchHotkey, &QHotkey::activated, qApp, [&](){
                ToggleStopwatch();
            });
            return;
    }
        case ResetKey: {
            ResetStopwatchHotkey = hotkey;
            resetStopwatchHotkey = new QHotkey(QKeySequence(ResetStopwatchHotkey), true, qApp);
            QObject::connect(resetStopwatchHotkey, &QHotkey::activated, qApp, [&](){
                ResetStopwatch();
            });
            return;
    }
        case BringToForeground: {
            BringToForegroundStopwatchHotkey = hotkey;
            bringToForegroundStopwatchHotkey = new QHotkey(QKeySequence(BringToForegroundStopwatchHotkey), true, qApp);
            QObject::connect(bringToForegroundStopwatchHotkey, &QHotkey::activated, qApp, [&](){
                BringToForegroundStopwatch();
            });
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
    SetHotkeyBlocked(ToggleKey,shouldBlockSignal);
    SetHotkeyBlocked(ResetKey,shouldBlockSignal);
    SetHotkeyBlocked(BringToForeground,shouldBlockSignal);

}

void QHotkeyManager::SetHotkeyBlocked(AvailableHotkeys hotkey, bool block)
{
    GetQHotkeyFromAvailableHotkeysEnum(hotkey)->blockSignals(block);
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

QHotkey* QHotkeyManager::GetQHotkeyFromAvailableHotkeysEnum(AvailableHotkeys ah)
{
    switch(ah) {
    case ToggleKey: {
        return toggleStopwatchHotkey;
    }
    case ResetKey: {
        return resetStopwatchHotkey;
    }
    case BringToForeground: {
        return bringToForegroundStopwatchHotkey;
    }
}
}


