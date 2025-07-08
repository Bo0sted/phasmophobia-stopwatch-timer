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
    bool ReassignHotkey(AvailableHotkeys ah, QString hotkey);
    void ReassignHotkey(AvailableHotkeys ah, bool shouldUnassign=true);
    bool IsHotkeyAvailable(QString hotkey, bool shouldAlertUser=false);
    void UpdateHotkeySignalBlock(bool shouldBlockSignal=true);
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
};

#endif // QHOTKEYMANAGER_H
