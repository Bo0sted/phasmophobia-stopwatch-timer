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
const QList<int> UioHotkeyManager::DefaultRestoreStopwatchHotkey = QList<int>{VC_3};

UioHotkeyManager::UioHotkeyManager(MainWindow *mwr, UioEventLoop *loop)
    : QObject(mwr),
    mw{mwr},
    eventLoop{loop},
    ToggleStopwatchHotkey{FetchToggleStopwatchHotkey()},
    ResetStopwatchHotkey{FetchResetStopwatchHotkey()},
    RestoreStopwatchHotkey{FetchRestoreStopwatchHotkey()},
    hotkeyReassignMode(false),
    newAssignedHotkey(-1),
    activeModifiers{},
    newModifiers{},
    totalElapsedSecondsBeforeReset{}
{
    connect(eventLoop, &UioEventLoop::keyReleased, this, &UioHotkeyManager::onKeyReleased);
    connect(eventLoop, &UioEventLoop::keyPressed, this, &UioHotkeyManager::onKeyPressed);
    connect(eventLoop, &UioEventLoop::modifierPressed, this, &UioHotkeyManager::onModifierPressed);
    connect(eventLoop, &UioEventLoop::modifierReleased, this, &UioHotkeyManager::onModifierReleased);
}

QList<int> UioHotkeyManager::FetchToggleStopwatchHotkey() {
    auto *qsm = &mw->qsm;
    QVariant hotkey = qsm->getValue(qsm->ToggleKey);
    QList<int> hotkeyList;
    for (const QVariant &v : hotkey.toList())
        hotkeyList.append(v.toInt());
    return (hotkey == false ? QList<int>{DefaultToggleStopwatchHotkey} : hotkeyList.toList());
}

QList<int> UioHotkeyManager::FetchResetStopwatchHotkey() {
    auto *qsm = &mw->qsm;
    QVariant hotkey = qsm->getValue(qsm->ResetKey);
    QList<int> hotkeyList;
    for (const QVariant &v : hotkey.toList())
        hotkeyList.append(v.toInt());
    return (hotkey == false ? QList<int>{DefaultResetStopwatchHotkey} : hotkeyList.toList());
}

QList<int> UioHotkeyManager::FetchRestoreStopwatchHotkey()
{
    auto *qsm = &mw->qsm;
    QVariant hotkey = qsm->getValue(qsm->RestoreKey);
    QList<int> hotkeyList;
    for (const QVariant &v : hotkey.toList())
        hotkeyList.append(v.toInt());
    return (hotkey == false ? QList<int>{DefaultRestoreStopwatchHotkey} : hotkeyList.toList());
}

void UioHotkeyManager::DeleteHotkey(AvailableHotkeys ah) {
    switch (ah) {
    case ToggleKey: ToggleStopwatchHotkey = DefaultToggleStopwatchHotkey; break;
    case ResetKey:  ResetStopwatchHotkey  = DefaultResetStopwatchHotkey;  break;
    case RestoreKey:  RestoreStopwatchHotkey  = DefaultRestoreStopwatchHotkey;  break;
    }
}

void UioHotkeyManager::FetchAndAssignHotkey(AvailableHotkeys ah) {
    DeleteHotkey(ah);
    switch (ah) {
    case ToggleKey: ToggleStopwatchHotkey = FetchToggleStopwatchHotkey(); break;
    case ResetKey:  ResetStopwatchHotkey  = FetchResetStopwatchHotkey();  break;
    case RestoreKey:  RestoreStopwatchHotkey  = FetchRestoreStopwatchHotkey();  break;
    }
}

void UioHotkeyManager::AssignHotkey(AvailableHotkeys ah, QList<int> hotkey) {
    DeleteHotkey(ah);
    switch (ah) {
    case ToggleKey: ToggleStopwatchHotkey = hotkey; break;
    case ResetKey:  ResetStopwatchHotkey  = hotkey; break;
    case RestoreKey:  RestoreStopwatchHotkey  = hotkey; break;
    }
}

bool listsEqual(QList<int> a, QList<int> b) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

bool UioHotkeyManager::IsHotkeyAvailable(QList<int> hotkey, AvailableHotkeys target, bool shouldAlertUser) {
    // Ignore if reassigning to itself
    QList<int> currentHotkey;
    switch (target) {
    case ToggleKey:  currentHotkey = ToggleStopwatchHotkey; break;
    case ResetKey:   currentHotkey = ResetStopwatchHotkey; break;
    case RestoreKey: currentHotkey = RestoreStopwatchHotkey; break;
    }
    if (listsEqual(hotkey, currentHotkey))
        return true;

    // Check against other hotkeys
    if ((target != ToggleKey   && listsEqual(hotkey, ToggleStopwatchHotkey)) ||
        (target != ResetKey    && listsEqual(hotkey, ResetStopwatchHotkey)) ||
        (target != RestoreKey  && listsEqual(hotkey, RestoreStopwatchHotkey)))
    {
        if (shouldAlertUser) {
            QString msg = QString("Sorry, %1 is already assigned.")
            .arg(GetDisplayFromQListOfKeycodes(hotkey));
            QMessageBox::information(nullptr, "Hotkey Error", msg);
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

QList<int> UioHotkeyManager::GetNewModifiers()
{
    return newModifiers;
}

void UioHotkeyManager::ClearHotkeyAssignState() {
    newModifiers.clear();
    newAssignedHotkey = -1;
    emit refreshHotkeyDisplays();
}

QList<int> UioHotkeyManager::GetHotkeyAssignBuffer() {
    QList<int> buffer = newModifiers;
    if (newAssignedHotkey != -1)
        buffer.append(newAssignedHotkey);
    return buffer;
}

QString UioHotkeyManager::GetDisplayFromQListOfKeycodes(QList<int> keycodes) {
    QStringList parts;
    for (int code : keycodes) parts << KeycodeToQString(code);
    return parts.join(" + ");
}

UioHotkeyManager::AvailableHotkeys UioHotkeyManager::GetHotkeyForCurrentTab()
{
    int tabIndex = mw->sie->GetActiveTabFromHotkeyGroup(); // however you get it
    switch (tabIndex) {
    case 0: return ToggleKey;
    case 1: return ResetKey;
    case 2: return RestoreKey;
    default: return ToggleKey; // fallback
    }
}

void UioHotkeyManager::UpdateHotkeySignalBlock(bool shouldBlockSignal) {
    return;
    eventLoop->blockSignals(shouldBlockSignal);
}

bool UioHotkeyManager::IsHotkeyMatch(int keycode, const QList<int> &targetHotkey) {
    if (targetHotkey.isEmpty()) return false;
    int mainKey = targetHotkey.last();
    if (keycode != mainKey) return false;

    QList<int> requiredModifiers = targetHotkey.mid(0, targetHotkey.size() - 1);
    QList<int> sortedRequired = requiredModifiers;
    QList<int> sortedActive = activeModifiers;
    std::sort(sortedRequired.begin(), sortedRequired.end());
    std::sort(sortedActive.begin(), sortedActive.end());
    return sortedRequired == sortedActive;
}

// 🔹 Modified to auto-translate using rawcode
void UioHotkeyManager::onKeyPressed(int keycode, int rawcode) {
    if (hotkeyReassignMode) {
        newAssignedHotkey = autoTranslateKeycode(Hotkey{keycode, rawcode});
        emit refreshHotkeyDisplays();
        return;
    }
}

void UioHotkeyManager::onKeyReleased(int keycode, int rawcode) {
    int translatedKeycode = autoTranslateKeycode(Hotkey{keycode, rawcode});

    if (hotkeyReassignMode) {
        newAssignedHotkey = translatedKeycode;
        AvailableHotkeys targetHotkey = GetHotkeyForCurrentTab();
        AssignHotkey(targetHotkey, GetHotkeyAssignBuffer());
        SetHotkeyReassignMode(false);
        emit refreshHotkeyDisplays();
        return;
    }

    if (IsHotkeyMatch(translatedKeycode, ToggleStopwatchHotkey)) {
        ToggleStopwatch();
    } else if (IsHotkeyMatch(translatedKeycode, ResetStopwatchHotkey)) {
        totalElapsedSecondsBeforeReset = mw->swm.elapsedSeconds;
        ResetStopwatch();
    }
    else if (IsHotkeyMatch(translatedKeycode, RestoreStopwatchHotkey)) {
        RestoreStopwatch();
    }
}

void UioHotkeyManager::onModifierPressed(int keycode, int rawcode) {
    if (hotkeyReassignMode) {
        if (!newModifiers.contains(keycode))
            newModifiers.append(keycode);
        newAssignedHotkey = -1;
    }
    if (activeModifiers.contains(keycode))
        activeModifiers.clear();
    else
        activeModifiers.append(keycode);
    emit refreshHotkeyDisplays();
}

void UioHotkeyManager::onModifierReleased(int keycode, int rawcode) {
    if (hotkeyReassignMode) {
        newModifiers.removeOne(keycode);
        emit refreshHotkeyDisplays();
    }
    activeModifiers.removeOne(keycode);
}

void UioHotkeyManager::ToggleStopwatch() {
    mw->swm.pauseStopwatch = !mw->swm.pauseStopwatch;
    mw->RefreshStopwatchState(false);
}

void UioHotkeyManager::ResetStopwatch()
{
    mw->swm.ResetStopwatch();
}

void UioHotkeyManager::RestoreStopwatch()
{
    mw->swm.elapsedSeconds = totalElapsedSecondsBeforeReset;
}
void UioHotkeyManager::BringToForegroundStopwatch()
{
    mw->show();
    mw->activateWindow();
    mw->raise();
}

int UioHotkeyManager::autoTranslateKeycode(const Hotkey &hotkey) {
    int translated = translateLinuxRawcodeToKeycode(hotkey.rawcode);

    // Only trust translation for special keys
    if (translated != VC_UNDEFINED &&
        (translated >= VC_INSERT && translated <= VC_RIGHT)) {
        return translated;
    }

    // Otherwise, fallback to normal keycode
    return hotkey.keycode;
}

int UioHotkeyManager::translateLinuxRawcodeToKeycode(int rawcode) {
    switch (rawcode) {
    case 65505: return VC_SHIFT_L;
    case 65507: return VC_CONTROL_L;
    case 65515: return VC_META_L;
    case 65513: return VC_ALT_L;
    case 65506: return VC_SHIFT_R;
    case 65508: return VC_CONTROL_R;
    case 65516: return VC_META_R;
    case 65514: return VC_ALT_R;
    case 65379: return VC_INSERT;
    case 65535: return VC_DELETE;
    case 65365: return VC_PAGE_UP;
    case 65366: return VC_PAGE_DOWN;
    case 65362: return VC_UP;
    case 65364: return VC_DOWN;
    case 65361: return VC_LEFT;
    case 65363: return VC_RIGHT;

    case 45: return VC_INSERT;
    case 46: return VC_DELETE;
    case 36: return VC_HOME;
    case 35: return VC_END;
    case 33: return VC_PAGE_UP;
    case 34: return VC_PAGE_DOWN;
    // Arrows on Windows
    case 38: return VC_UP;
    case 40: return VC_DOWN;
    case 37: return VC_LEFT;
    case 39: return VC_RIGHT;
    default: return VC_UNDEFINED;
    }
}

int UioHotkeyManager::translateKeycodeToLinuxRawcode(int keycode) {
    switch (keycode) {
    case VC_SHIFT_L: return 65505;
    case VC_CONTROL_L: return 65507;
    case VC_META_L: return 65515;
    case VC_ALT_L: return 65513;
    case VC_SHIFT_R: return 65506;
    case VC_CONTROL_R: return 65508;
    case VC_META_R: return 65516;
    case VC_ALT_R: return 65514;
    default: return -1;
    }
}
