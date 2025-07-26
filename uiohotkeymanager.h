#ifndef UIOHOTKEYMANAGER_H
#define UIOHOTKEYMANAGER_H

#include <QObject>
#include <QString>

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

    static const QString DefaultToggleStopwatchHotkey;
    static const QString DefaultResetStopwatchHotkey;

    QString FetchToggleStopwatchHotkey();
    QString FetchResetStopwatchHotkey();

    void DeleteHotkey(AvailableHotkeys ah);
    void FetchAndAssignHotkey(AvailableHotkeys ah);
    void AssignHotkey(AvailableHotkeys ah, QString hotkey);
    bool IsHotkeyAvailable(QString hotkey, bool shouldAlertUser = false);

    void UpdateHotkeySignalBlock(bool shouldBlockSignal = true);
    void SetHotkeyBlocked(AvailableHotkeys hotkey, bool block);

private slots:
    void onKeyPressed(int keycode);

private:
    MainWindow *mw;
    UioEventLoop *eventLoop;

    QString ToggleStopwatchHotkey;
    QString ResetStopwatchHotkey;

    int toggleKeycode;
    int resetKeycode;

    void ToggleStopwatch();
    void ResetStopwatch();
    void BringToForegroundStopwatch();
};

#endif // UIOHOTKEYMANAGER_H
