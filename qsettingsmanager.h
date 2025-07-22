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
        StopwatchBorderColor,
        StopwatchBorderThickness,
        StopwatchBackgroundEnabled

    };
    enum SettingsForClockGroupIndex {
        ClockFont,
        ClockFontColor,
        ClockBackgroundColor,
        IsClockEnabled,
        LastClockPosition,

    };

    enum Logging {
        UUID,
        LastPingUnix,
        LastUpdateCheckUnix,
    };


    const QList<QString> SettingsForHotkeyGroup;
    const QList<QString> SettingsForStopwatchGroup;
    const QList<QString> SettingsForClockGroup;
    const QList<QString> SettingsForLoggingGroup;

    QSettingsManager();
    void setValue(QString groupName, QString valueName, QString value);
    void setValue(enum SettingsForStopwatchGroupIndex e, QString value);
    void setValue(enum SettingsForHotkeyGroupIndex e, QString value);
    void setValue(enum SettingsForClockGroupIndex e, QString value);
    void setValue(enum Logging e, QVariant value);
    const QVariant getValue(enum SettingsForHotkeyGroupIndex e);
    const QVariant getValue(enum SettingsForStopwatchGroupIndex e, bool shouldDefaultBeInteger=false);
    const QVariant getValue(enum SettingsForClockGroupIndex e, bool shouldDefaultBeInteger=false);
    const QVariant getValue(enum Logging e);
    const QVariant getValue(QString groupName, QString valueName);

    QString FetchStopwatchFontColor();
    QString FetchPausedStopwatchFontColor();
    QString FetchResetStopwatchFontColor();
    QString FetchStopwatchBackground();
    QString FetchStopwatchFont();
    QString FetchClockFont();
    QString FetchClockFontColor();
    QString FetchStopwatchBackgroundColor();
    QString FetchStopwatchBorderColor();
    QString FetchStopwatchBorderWidth();
    bool FetchIsBackgroundEnabled();
    QPair<float,float> FetchStopwatchLastPosition();
    QPair<float,float> FetchSystemClockLastPosition();
    bool CheckIfClockEnabled();
    QString FetchUUID();
    QString GenerateUUID();
    int FetchLastPingUnix();
    int FetchLastUpdateCheckUnix();
    bool ShouldPostPing();
    bool ShouldCheckUpdate();
    qint64 MinutesToSeconds(int minutes);
private:
    QSettings settings;
};

#endif // QSETTINGSMANAGER_H
