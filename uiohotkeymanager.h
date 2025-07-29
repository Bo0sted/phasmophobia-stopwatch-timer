#ifndef UIOHOTKEYMANAGER_H
#define UIOHOTKEYMANAGER_H

#include <QObject>
#include <QString>
#include "uiohook.h"
#include "uiokey.h"

class MainWindow;
class UioEventLoop;


class UioHotkeyManager : public QObject {
    Q_OBJECT
public:
    enum AvailableHotkeys {
        ToggleKey,
        ResetKey,
        RestoreKey
    };

    explicit UioHotkeyManager(MainWindow *mwr, UioEventLoop *loop);

    static const QList<int> DefaultToggleStopwatchHotkey;
    static const QList<int> DefaultResetStopwatchHotkey;
    static const QList<int> DefaultRestoreStopwatchHotkey;

    QList<int> FetchToggleStopwatchHotkey();
    QList<int> FetchResetStopwatchHotkey();
    QList<int> FetchRestoreStopwatchHotkey();

    void DeleteHotkey(AvailableHotkeys ah);
    void FetchAndAssignHotkey(AvailableHotkeys ah);
    void AssignHotkey(AvailableHotkeys ah, QList<int> hotkey);
    bool IsHotkeyAvailable(QList<int> hotkey, AvailableHotkeys target, bool shouldAlertUser = false);
    bool IsHotkeyMatch(int keycode, const QList<int> &targetHotkey);
    QString IntToUioKeyName(QList<int> hotkey);
    void SetHotkeyReassignMode(bool enabled);
    bool GetHotkeyAssignMode();
    int GetHotkeyToReasign();
    QList<int> GetNewModifiers();
    void ClearHotkeyAssignState();
    QList<int> GetHotkeyAssignBuffer();
    QString GetDisplayFromQListOfKeycodes(QList<int> keycodes);
    AvailableHotkeys GetHotkeyForCurrentTab();

    // 🔹 Translation helpers
    int translateLinuxRawcodeToKeycode(int rawcode);
    int translateKeycodeToLinuxRawcode(int keycode);
    int autoTranslateKeycode(const Hotkey &hk);

    void UpdateHotkeySignalBlock(bool shouldBlockSignal = true);

signals:
    void refreshHotkeyDisplays();

private slots:
    void onKeyPressed(int keycode, int rawcode);
    void onKeyReleased(int keycode, int rawcode);
    void onModifierPressed(int keycode, int rawcode);
    void onModifierReleased(int keycode, int rawcode);

private:
    MainWindow *mw;
    UioEventLoop *eventLoop;
    bool hotkeyReassignMode;
    int newAssignedHotkey;
    QList<int> newModifiers;
    QList<int> activeModifiers;

    QList<int> ToggleStopwatchHotkey;
    QList<int> ResetStopwatchHotkey;
    QList<int> RestoreStopwatchHotkey;
    int totalElapsedSecondsBeforeReset;

    void ToggleStopwatch();
    void ResetStopwatch();
    void RestoreStopwatch();
    void BringToForegroundStopwatch();
};

#endif // UIOHOTKEYMANAGER_H
