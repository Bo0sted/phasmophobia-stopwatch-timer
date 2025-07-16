#ifndef QHOTKEYMANAGER_H
#define QHOTKEYMANAGER_H

#include <QString>
#include <QHotkey>

class MainWindow;

class QHotkeyManager
{
public:
    enum AvailableHotkeys {
        ToggleKey,
        ResetKey,
        BringToForeground
    };

    QHotkeyManager(MainWindow *mwr);
    static const QString DefaultToggleStopwatchHotkey;
    static const QString DefaultResetStopwatchHotkey;
    static const QString DefaultBringToForegroundHotkey;


    QString FetchToggleStopwatchHotkey();
    QString FetchResetStopwatchHotkey();
    QString FetchBringToForegroundHotkey();
    void DeleteHotkey(AvailableHotkeys ah);
    void FetchAndAssignHotkey(AvailableHotkeys ah);
    void AssignHotkey(AvailableHotkeys ah, QString hotkey);
    bool IsHotkeyAvailable(QString hotkey, bool shouldAlertUser=false);
    void UpdateHotkeySignalBlock(bool shouldBlockSignal=true);
    void SetHotkeyBlocked(AvailableHotkeys hotkey, bool block);
private:
    MainWindow *mw;
    QString ToggleStopwatchHotkey;
    QString ResetStopwatchHotkey;
    QString BringToForegroundStopwatchHotkey;

    QHotkey *toggleStopwatchHotkey;
    QHotkey *resetStopwatchHotkey;
    QHotkey *bringToForegroundStopwatchHotkey;
    void ToggleStopwatch();
    void ResetStopwatch();
    void BringToForegroundStopwatch();
    QHotkey* GetQHotkeyFromAvailableHotkeysEnum(AvailableHotkeys ah);
};

#endif // QHOTKEYMANAGER_H
