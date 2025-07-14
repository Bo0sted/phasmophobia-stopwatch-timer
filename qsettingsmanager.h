#ifndef QSETTINGSMANAGER_H
#define QSETTINGSMANAGER_H

#include <QSettings>
#include <QList>

class QSettingsManager
{
public:
    enum SettingsForHotkeyGroupIndex {
        ToggleKey,
        ResetKey,
        BringToForeground
    };
    enum SettingsForStopwatchGroupIndex {
        Font,
        FontColor,
        BackgroundColor,
        LastWindowPosition,
        LastSystemClockPosition,
        ClockFont,
        IsClockEnabled
    };

    const QList<QString> SettingsForHotkeyGroup;
    const QList<QString> SettingsForStopwatchGroup;

    QSettingsManager();
    void setValue(QString groupName, QString valueName, QString value);
    void setValue(enum SettingsForStopwatchGroupIndex e, QString value);
    void setValue(enum SettingsForHotkeyGroupIndex e, QString value);
    const QVariant getValue(enum SettingsForHotkeyGroupIndex e);
    const QVariant getValue(enum SettingsForStopwatchGroupIndex e, bool shouldDefaultBeInteger=false);
    const QVariant getValue(QString groupName, QString valueName);

    QString FetchStopwatchFontColor();
    QString FetchStopwatchFont();
    QString FetchClockFont();
    QPair<float,float> FetchStopwatchLastPosition();
    QPair<float,float> FetchSystemClockLastPosition();
    bool CheckIfClockEnabled();
private:
    QSettings settings;
};

#endif // QSETTINGSMANAGER_H
