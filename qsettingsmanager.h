#ifndef QSETTINGSMANAGER_H
#define QSETTINGSMANAGER_H

#include "stopwatchmanager.h"

#include <QSettings>
#include <QList>


class QSettingsManager
{
public:
    enum SettingsForHotkeyGroupIndex {
        ResetKey,
        RestoreKey,
        ToggleKey,
    };
    enum SettingsForStopwatchGroupIndex {
        IsStopwatchEnabled,
        LastStopwatchPosition,
        StopwatchBackgroundColor,
        StopwatchFont,
        StopwatchFontColor,
        StopwatchFontSize,
        StopwatchPausedFontColor,
        StopwatchResetFontColor,
        StopwatchBorderColor,
        StopwatchBorderThickness,
        StopwatchBackgroundEnabled,
        StopwatchRainbowModeIndex,
        StopwatchFormatModeIndex,
        StopwatchGradientOneColor,
        StopwatchGradientTwoColor,
        StopwatchGradientEnabled,

    };
    enum SettingsForClockGroupIndex {
        ClockFont,
        ClockFontSize,
        ClockFontColor,
        ClockBackgroundColor,
        IsClockEnabled,
        LastClockPosition,
        IsSyncedFontEnabled,

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
    void setValue(enum SettingsForHotkeyGroupIndex e, QVariantList value);
    void setValue(enum SettingsForHotkeyGroupIndex e, QString value);
    void setValue(enum SettingsForClockGroupIndex e, QString value);
    void setValue(enum Logging e, QVariant value);
    const QVariant getValue(enum SettingsForHotkeyGroupIndex e);
    const QVariant getValue(enum SettingsForStopwatchGroupIndex e, bool shouldDefaultBeInteger=false);
    const QVariant getValue(enum SettingsForClockGroupIndex e, bool shouldDefaultBeInteger=false);
    const QVariant getValue(enum Logging e);
    const QVariant getValue(QString groupName, QString valueName);

    bool FetchIsStopwatchEnabled();
    bool FetchIsGradientEnabled();
    QString FetchStopwatchFontColor();
    QString FetchPausedStopwatchFontColor();
    QString FetchResetStopwatchFontColor();
    QString FetchGradientOneFontColor();
    QString FetchGradientTwoFontColor();
    QString FetchStopwatchBackground();
    QString FetchStopwatchFont();
    int FetchStopwatchFontSize();
    QString FetchClockFont();
    int FetchClockFontSize();
    QString FetchClockFontColor();
    QString FetchStopwatchBackgroundColor();
    QString FetchStopwatchBorderColor();
    QString FetchStopwatchBorderWidth();
    int FetchStopwatchRainbowModeIndex();
    enum StopwatchManager::FormatModes FetchStopwatchFormatMode();
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
    bool FetchIsClockFontSynced();
private:
    QSettings settings;
};

#endif // QSETTINGSMANAGER_H
