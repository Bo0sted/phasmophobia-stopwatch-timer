#ifndef QHOTKEYMANAGER_H
#define QHOTKEYMANAGER_H

#include <QString>
#include <QHotkey>

class MainWindow;

class QHotkeyManager
{
public:
    QHotkeyManager(MainWindow *mwr);
    static const QString DefaultToggleStopwatchHotkey;
    static const QString DefaultResetStopwatchHotkey;


    QString FetchToggleStopwatchHotkey();
    QString FetchResetStopwatchHotkey();
private:
    MainWindow *mw;
    QString ToggleStopwatchHotkey;
    QString ResetStopwatchHotkey;

    QHotkey *toggleStopwatchHotkey;
    QHotkey *resetStopwatchHotkey;
    void ToggleStopwatch();
    void ResetStopwatch();
};

#endif // QHOTKEYMANAGER_H
