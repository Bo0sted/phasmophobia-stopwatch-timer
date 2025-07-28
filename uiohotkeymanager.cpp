#include "uiohotkeymanager.h"
#include "mainwindow.h"
#include "qsettingsmanager.h"
#include "uioeventloop.h"

#include <uiohook.h>
#include "uiokey.h"

#include <QMessageBox>
#include <QDebug>

const QList<int> UioHotkeyManager::DefaultToggleStopwatchHotkey = QList<int>{VC_1};
const QList<int> UioHotkeyManager::DefaultResetStopwatchHotkey = QList<int>{VC_2};

UioHotkeyManager::UioHotkeyManager(MainWindow *mwr, UioEventLoop *loop)
    : QObject(mwr),
    mw{mwr},
    eventLoop{loop},
    ToggleStopwatchHotkey{FetchToggleStopwatchHotkey()},
    ResetStopwatchHotkey{FetchResetStopwatchHotkey()},
    hotkeyReassignMode(false),
    newAssignedHotkey(-1),
    activeModifiers{}
{
    connect(eventLoop, &UioEventLoop::keyReleased,
            this, &UioHotkeyManager::onKeyReleased);
    connect(eventLoop, &UioEventLoop::keyPressed,
            this, &UioHotkeyManager::onKeyPressed);
}

QList<int> UioHotkeyManager::FetchToggleStopwatchHotkey() {
    auto *qsm = &mw->qsm;
    QVariant hotkey = qsm->getValue(qsm->ToggleKey);
    QList<int> hotkeyList;
    for (const QVariant &v : hotkey.toList())
        hotkeyList.append(v.toInt());
    return (hotkey == false ? QList<int>{DefaultToggleStopwatchHotkey}: hotkeyList.toList());
}

QList<int> UioHotkeyManager::FetchResetStopwatchHotkey() {
    auto *qsm = &mw->qsm;
    QVariant hotkey = qsm->getValue(qsm->ResetKey);
    QList<int> hotkeyList;
    for (const QVariant &v : hotkey.toList())
        hotkeyList.append(v.toInt());
    return (hotkey == false ? QList<int>{DefaultResetStopwatchHotkey}: hotkeyList.toList());
}

void UioHotkeyManager::DeleteHotkey(AvailableHotkeys ah) {
    switch (ah) {
    case ToggleKey:
        ToggleStopwatchHotkey = DefaultToggleStopwatchHotkey;
        break;
    case ResetKey:
        ResetStopwatchHotkey = DefaultResetStopwatchHotkey;
        break;
    }
}

void UioHotkeyManager::FetchAndAssignHotkey(AvailableHotkeys ah) {
    DeleteHotkey(ah);

    switch (ah) {
    case ToggleKey:
        ToggleStopwatchHotkey = FetchToggleStopwatchHotkey();
        break;
    case ResetKey:
        ResetStopwatchHotkey = FetchResetStopwatchHotkey();
        break;
    }
}

void UioHotkeyManager::AssignHotkey(AvailableHotkeys ah, QList<int> hotkey) {
    DeleteHotkey(ah);

    switch (ah) {
    case ToggleKey:
        ToggleStopwatchHotkey = hotkey;
        break;
    case ResetKey:
        ResetStopwatchHotkey = hotkey;
        break;
    }
}

bool UioHotkeyManager::IsHotkeyAvailable(QList<int> hotkey, bool shouldAlertUser) {
    if (hotkey == ToggleStopwatchHotkey || hotkey == ResetStopwatchHotkey) {
        if (shouldAlertUser) {
            QString message = QString("Sorry, %1 is already assigned.").arg(GetDisplayFromQListOfKeycodes(hotkey));
            QMessageBox::information(nullptr, "Hotkey Error", message);
        }
        return false;
    }
    return true;
}

void UioHotkeyManager::SetHotkeyReassignMode(bool enabled)
{
    hotkeyReassignMode = enabled;
}

bool UioHotkeyManager::GetHotkeyAssignMode()
{
    return hotkeyReassignMode;
}

int UioHotkeyManager::GetHotkeyToReasign()
{
    return newAssignedHotkey;
}

QList<int> UioHotkeyManager::GetActiveModifiers()
{
    return activeModifiers;
}

void UioHotkeyManager::ClearHotkeyAssignState()
{
    activeModifiers.clear();
    newAssignedHotkey = -1;
}

QList<int> UioHotkeyManager::GetHotkeyAssignBuffer()
{
    QList<int> buffer = activeModifiers;
    if (newAssignedHotkey != -1)
        buffer.append(newAssignedHotkey);
    return buffer;
}

QString UioHotkeyManager::GetDisplayFromQListOfKeycodes(QList<int> keycodes)
{
    QStringList parts;
    for (int code : keycodes) {
        parts << KeycodeToQString(code);
    }
    return parts.join(" + ");
}

void UioHotkeyManager::UpdateHotkeySignalBlock(bool shouldBlockSignal) {
    return;
    eventLoop->blockSignals(shouldBlockSignal);
}

bool UioHotkeyManager::IsHotkeyMatch(int keycode, const QList<int> &targetHotkey) {
    if (targetHotkey.isEmpty())
        return false;

    // The last element is the main key
    int mainKey = targetHotkey.last();

    if (keycode != mainKey)
        return false;

    // Compare modifiers (everything except last)
    QList<int> requiredModifiers = targetHotkey.mid(0, targetHotkey.size() - 1);

    // Sort both lists to avoid order issues
    QList<int> sortedRequired = requiredModifiers;
    QList<int> sortedActive = activeModifiers;
    std::sort(sortedRequired.begin(), sortedRequired.end());
    std::sort(sortedActive.begin(), sortedActive.end());

    return sortedRequired == sortedActive;
}


void UioHotkeyManager::onKeyPressed(int keycode) {
    // Always track modifiers
    if (IsKeycodeModifierKey(keycode, keycode)) {
        // If the modifier already exists, reset everything
        if (activeModifiers.contains(keycode)) {
            activeModifiers.clear();
            newAssignedHotkey = -1;
            // emit refreshHotkeyDisplays();
            return;
        }

        // Otherwise, add it
        activeModifiers.append(keycode);
         emit refreshHotkeyDisplays();
        return;
    }

    // Hotkey reassign mode logic
    if (hotkeyReassignMode) {
        newAssignedHotkey = keycode;
        //emit refreshHotkeyDisplays();
        return;
    }
}


void UioHotkeyManager::onKeyReleased(int keycode) {
    // Always track modifier release
    if (IsKeycodeModifierKey(keycode, keycode) && !hotkeyReassignMode) {
        activeModifiers.removeOne(keycode);
    }
    if (hotkeyReassignMode) {
        emit refreshHotkeyDisplays();
        return;
    }

    if (hotkeyReassignMode) {
        newAssignedHotkey = keycode;
        SetHotkeyReassignMode(false);
        emit refreshHotkeyDisplays();
        return;
    }

    // Hotkey actions now take activeModifiers into account
    if (IsHotkeyMatch(keycode, ToggleStopwatchHotkey)) {
        ToggleStopwatch();
    } else if (IsHotkeyMatch(keycode, ResetStopwatchHotkey)) {
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
