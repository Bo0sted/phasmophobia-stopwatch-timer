#include "uiohotkeymanager.h"
#include "mainwindow.h"
#include "qsettingsmanager.h"
#include "uioeventloop.h"

#include <QMessageBox>
#include <QDebug>

const QString UioHotkeyManager::DefaultToggleStopwatchHotkey = "1";
const QString UioHotkeyManager::DefaultResetStopwatchHotkey = "2";

int stringToKeycode(const QString &key) {
    if (key == "1") return VC_1;
    if (key == "2") return VC_2;
    if (key == "A") return VC_A;
    if (key == "B") return VC_B;
    // Add more mappings as needed
    return 0; // Invalid
}

UioHotkeyManager::UioHotkeyManager(MainWindow *mwr, UioEventLoop *loop)
    : QObject(mwr),
    mw{mwr},
    eventLoop{loop},
    ToggleStopwatchHotkey{FetchToggleStopwatchHotkey()},
    ResetStopwatchHotkey{FetchResetStopwatchHotkey()}
{
    connect(eventLoop, &UioEventLoop::keyPressed,
            this, &UioHotkeyManager::onKeyPressed);

    toggleKeycode = stringToKeycode(ToggleStopwatchHotkey);
    resetKeycode  = stringToKeycode(ResetStopwatchHotkey);
}

QString UioHotkeyManager::FetchToggleStopwatchHotkey() {
    auto *qsm = &mw->qsm;
    auto hotkey = qsm->getValue(qsm->ToggleKey);
    return hotkey == false ? DefaultToggleStopwatchHotkey : hotkey.toString();
}

QString UioHotkeyManager::FetchResetStopwatchHotkey() {
    auto *qsm = &mw->qsm;
    auto hotkey = qsm->getValue(qsm->ResetKey);
    return hotkey == false ? DefaultResetStopwatchHotkey : hotkey.toString();
}

void UioHotkeyManager::DeleteHotkey(AvailableHotkeys ah) {
    switch (ah) {
    case ToggleKey:
        ToggleStopwatchHotkey = DefaultToggleStopwatchHotkey;
        toggleKeycode = ToggleStopwatchHotkey.toInt();
        break;
    case ResetKey:
        ResetStopwatchHotkey = DefaultResetStopwatchHotkey;
        resetKeycode = ResetStopwatchHotkey.toInt();
        break;
    }
}

void UioHotkeyManager::FetchAndAssignHotkey(AvailableHotkeys ah) {
    DeleteHotkey(ah);

    switch (ah) {
    case ToggleKey:
        ToggleStopwatchHotkey = FetchToggleStopwatchHotkey();
        toggleKeycode = ToggleStopwatchHotkey.toInt();
        break;
    case ResetKey:
        ResetStopwatchHotkey = FetchResetStopwatchHotkey();
        resetKeycode = ResetStopwatchHotkey.toInt();
        break;
    }
}

void UioHotkeyManager::AssignHotkey(AvailableHotkeys ah, QString hotkey) {
    DeleteHotkey(ah);

    switch (ah) {
    case ToggleKey:
        ToggleStopwatchHotkey = hotkey;
        toggleKeycode = hotkey.toInt();
        break;
    case ResetKey:
        ResetStopwatchHotkey = hotkey;
        resetKeycode = hotkey.toInt();
        break;
    }
}

bool UioHotkeyManager::IsHotkeyAvailable(QString hotkey, bool shouldAlertUser) {
    if (hotkey.isEmpty()) {
        if (shouldAlertUser) {
            QMessageBox::information(nullptr, "Hotkey Error",
                                     "No hotkey was inputted. Please try again.");
        }
        return false;
    }
    if (hotkey == ToggleStopwatchHotkey || hotkey == ResetStopwatchHotkey) {
        if (shouldAlertUser) {
            QString message = QString("Sorry, %1 is already assigned.").arg(hotkey);
            QMessageBox::information(nullptr, "Hotkey Error", message);
        }
        return false;
    }
    return true;
}

void UioHotkeyManager::UpdateHotkeySignalBlock(bool) {
    // Not needed with uiohook
}

void UioHotkeyManager::SetHotkeyBlocked(AvailableHotkeys, bool) {
    // Not needed with uiohook
}

void UioHotkeyManager::onKeyPressed(int keycode) {
    if (keycode == toggleKeycode) {
        ToggleStopwatch();
    } else if (keycode == resetKeycode) {
        ResetStopwatch();
    }
}

void UioHotkeyManager::ToggleStopwatch() {
    mw->swm.pauseStopwatch = !mw->swm.pauseStopwatch;
    mw->RefreshStopwatchState(false);
}

void UioHotkeyManager::ResetStopwatch() {
    mw->swm.ResetStopwatch();
}

void UioHotkeyManager::BringToForegroundStopwatch() {
    mw->show();
    mw->activateWindow();
    mw->raise();
}
