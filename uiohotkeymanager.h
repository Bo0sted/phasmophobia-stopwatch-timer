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
        ResetKey
    };

    explicit UioHotkeyManager(MainWindow *mwr, UioEventLoop *loop);

    static const QList<int> DefaultToggleStopwatchHotkey;
    static const QList<int> DefaultResetStopwatchHotkey;

    QList<int> FetchToggleStopwatchHotkey();
    QList<int> FetchResetStopwatchHotkey();

    void DeleteHotkey(AvailableHotkeys ah);
    void FetchAndAssignHotkey(AvailableHotkeys ah);
    void AssignHotkey(AvailableHotkeys ah, QList<int> hotkey);
    bool IsHotkeyAvailable(QList<int> hotkey, bool shouldAlertUser = false);
    bool IsHotkeyMatch(int keycode, const QList<int> &targetHotkey);
    QString IntToUioKeyName(QList<int> hotkey);
    void SetHotkeyReassignMode(bool enabled);
    bool GetHotkeyAssignMode();
    int GetHotkeyToReasign();
    QList<int> GetNewModifiers();
    void ClearHotkeyAssignState();
    QList<int> GetHotkeyAssignBuffer();
    QString GetDisplayFromQListOfKeycodes(QList<int> keycodes);

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

    void ToggleStopwatch();
    void ResetStopwatch();
    void BringToForegroundStopwatch();
};

#endif // UIOHOTKEYMANAGER_H
