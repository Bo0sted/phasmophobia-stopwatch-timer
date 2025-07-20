#ifndef QSETTINGSMANAGER_H
#define QSETTINGSMANAGER_H

#include <QSettings>
#include <QList>

class QSettingsManager
{
public:
    enum SettingsForHotkeyGroupIndex {
        ResetKey,
        ToggleKey,
    };
    enum SettingsForStopwatchGroupIndex {
        LastStopwatchPosition,
        StopwatchBackgroundColor,
        StopwatchFont,
        StopwatchFontColor,
        StopwatchPausedFontColor,
        StopwatchResetFontColor,
    };
    enum SettingsForClockGroupIndex {
        ClockFont,
        ClockFontColor,
        ClockBackgroundColor,
        IsClockEnabled,
        LastClockPosition,

    };


    const QList<QString> SettingsForHotkeyGroup;
    const QList<QString> SettingsForStopwatchGroup;
    const QList<QString> SettingsForClockGroup;

    QSettingsManager();
    void setValue(QString groupName, QString valueName, QString value);
    void setValue(enum SettingsForStopwatchGroupIndex e, QString value);
    void setValue(enum SettingsForHotkeyGroupIndex e, QString value);
    void setValue(enum SettingsForClockGroupIndex e, QString value);
    const QVariant getValue(enum SettingsForHotkeyGroupIndex e);
    const QVariant getValue(enum SettingsForStopwatchGroupIndex e, bool shouldDefaultBeInteger=false);
    const QVariant getValue(enum SettingsForClockGroupIndex e, bool shouldDefaultBeInteger=false);
    const QVariant getValue(QString groupName, QString valueName);

    QString FetchStopwatchFontColor();
    QString FetchPausedStopwatchFontColor();
    QString FetchResetStopwatchFontColor();
    QString FetchStopwatchFont();
    QString FetchClockFont();
    QString FetchClockFontColor();
    QPair<float,float> FetchStopwatchLastPosition();
    QPair<float,float> FetchSystemClockLastPosition();
    bool CheckIfClockEnabled();
private:
    QSettings settings;
};

#endif // QSETTINGSMANAGER_H
