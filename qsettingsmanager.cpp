#include "qsettingsmanager.h"
#include "stylesheetgenerator.h"

#include <QSettings>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QUuid>
#include <QDateTime>
#include <QDebug>

QSettingsManager::QSettingsManager()
    : settings(QString("%1/%2").arg(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),"StopwatchSettings.ini"), QSettings::Format::IniFormat)
    , SettingsForHotkeyGroup{
        "Hotkeys/Reset",
        "Hotkeys/ToggleOnOff"
          }
    , SettingsForStopwatchGroup{
        "Stopwatch/LastStopwatchPosition",
        "Stopwatch/StopwatchBackgroundColor",
        "Stopwatch/StopwatchFont",
        "Stopwatch/StopwatchFontColor",
        "Stopwatch/StopwatchPausedFontColor",
        "Stopwatch/StopwatchResetFontColor",
        "Stopwatch/StopwatchBorderColor",
        "Stopwatch/StopwatchBorderThickness",
        "Stopwatch/StopwatchBackgroundEnabled"
      }
    , SettingsForClockGroup{
        "System-Clock-Module/ClockFont",
        "System-Clock-Module/ClockFontColor",
        "System-Clock-Module/ClockBackgroundColor",
        "System-Clock-Module/IsClockEnabled",
        "System-Clock-Module/LastClockPosition"}
    , SettingsForLoggingGroup{
        "Network/UUID",
        "Network/LastPingUnix",
        "Network/LastUpdateCheckUnix"}

{
    setValue("Application", "Version", QString("%1").arg(QCoreApplication::applicationVersion()));
}

void QSettingsManager::setValue(QString groupName, QString valueName, QString value)
{
    settings.setValue(QString("%1/%2").arg(groupName, valueName), value);
}

void QSettingsManager::setValue(SettingsForStopwatchGroupIndex e, QString value)
{
    settings.setValue(QString("%1").arg(SettingsForStopwatchGroup[e]), value);
}

void QSettingsManager::setValue(SettingsForHotkeyGroupIndex e, QString value)
{
    settings.setValue(QString("%1").arg(SettingsForHotkeyGroup[e]), value);
}

void QSettingsManager::setValue(SettingsForClockGroupIndex e, QString value)
{
    settings.setValue(QString("%1").arg(SettingsForClockGroup[e]), value);
}

void QSettingsManager::setValue(Logging e, QVariant value)
{
    settings.setValue(QString("%1").arg(SettingsForLoggingGroup[e]), value);
}

const QVariant QSettingsManager::getValue(SettingsForClockGroupIndex e, bool shouldDefaultBeInteger)
{
    return settings.value(SettingsForClockGroup[e], false);
}

const QVariant QSettingsManager::getValue(enum Logging e)
{
    return settings.value(SettingsForLoggingGroup[e], false);
}

const QVariant QSettingsManager::getValue(enum SettingsForHotkeyGroupIndex e)
{
    return settings.value(SettingsForHotkeyGroup[e], false);
}

const QVariant QSettingsManager::getValue(enum SettingsForStopwatchGroupIndex e, bool shouldDefaultBeInteger)
{
    if (shouldDefaultBeInteger)
        return settings.value(SettingsForStopwatchGroup[e], -1);
    return settings.value(SettingsForStopwatchGroup[e], false);
}

const QVariant QSettingsManager::getValue(QString groupName, QString valueName)
{
    return settings.value(QString("%1/%2").arg(groupName, valueName),false);
}

QString QSettingsManager::FetchStopwatchFontColor()
{
    QVariant val = getValue(StopwatchFontColor);

    if (val == false) return StylesheetGenerator::DefaultFontHexColor;
    else return val.toString();
}

QString QSettingsManager::FetchPausedStopwatchFontColor()
{
    QVariant val = getValue(StopwatchPausedFontColor);

    if (val == false) return StylesheetGenerator::DefaultPausedFontHexColor;
    else return val.toString();
}

QString QSettingsManager::FetchResetStopwatchFontColor()
{
    QVariant val = getValue(StopwatchResetFontColor);

    if (val == false) return StylesheetGenerator::DefaultPausedFontHexColor;
    else return val.toString();
}

QString QSettingsManager::FetchStopwatchBackgroundColor()
{
    QVariant val = getValue(StopwatchBackgroundColor);

    if (val == false) return StylesheetGenerator::DefaultStopwatchBackground;
    else return val.toString();
}

QString QSettingsManager::FetchStopwatchBorderColor()
{
    QVariant val = getValue(StopwatchBorderColor);

    if (val == false) return StylesheetGenerator::DefaultStopwatchBorderColor;
    else return val.toString();
}

QString QSettingsManager::FetchStopwatchBorderWidth()
{
    QVariant val = getValue(StopwatchBorderThickness);

    if (val == false) return StylesheetGenerator::DefaultStopwatchBorderWidth;
    else return val.toString();
}

bool QSettingsManager::FetchIsBackgroundEnabled()
{
    QVariant val = getValue(StopwatchBackgroundEnabled, true);

    if (val == -1) return true;
    else return val.toBool();
}



QString QSettingsManager::FetchStopwatchFont()
{
    QVariant val = getValue(StopwatchFont);

    if (val == false) return StylesheetGenerator::DefaultFont.toString();
    else return val.toString();
}

QString QSettingsManager::FetchClockFont()
{
    QVariant val = getValue(ClockFont);

    if (val == false) return StylesheetGenerator::DefaultFont.toString();
    else return val.toString();
}

QString QSettingsManager::FetchClockFontColor()
{
    QVariant val = getValue(ClockFontColor);

    if (val == false) return StylesheetGenerator::DefaultFontHexColor;
    else return val.toString();
}

QPair<float,float> QSettingsManager::FetchStopwatchLastPosition()
{
    QVariant val = getValue(LastStopwatchPosition);

    if (val == false) return QPair<float,float>(0,0);

    QStringList pos = val.toString().split(",");
    float x = pos[0].toFloat();
    float y = pos[1].toFloat();

    return QPair<float,float>(x,y);
}

QPair<float,float> QSettingsManager::FetchSystemClockLastPosition()
{
    QVariant val = getValue(LastClockPosition);

    if (val == false) return QPair<float,float>(0,100);

    QStringList pos = val.toString().split(",");
    float x = pos[0].toFloat();
    float y = pos[1].toFloat();
    return QPair<float,float>(x,y);
}

bool QSettingsManager::CheckIfClockEnabled()
{
    QVariant val = getValue(IsClockEnabled, true);

    if (val == -1) return true;
    else return val.toBool();
}

QString QSettingsManager::FetchUUID()
{
    QVariant val = getValue(UUID);

    if (val == false) return GenerateUUID();
    else return val.toString();
}

int QSettingsManager::FetchLastPingUnix()
{
    QVariant val = getValue(LastPingUnix);

    if (val == false) return 0;
    else return val.toInt();
}

int QSettingsManager::FetchLastUpdateCheckUnix()
{
    QVariant val = getValue(LastUpdateCheckUnix);

    if (val == false) return 0;
    else return val.toInt();
}

bool QSettingsManager::ShouldPostPing()
{
    qint64 last = FetchLastPingUnix();
    qint64 now = QDateTime::currentSecsSinceEpoch();

    return (now - last) > MinutesToSeconds(0);
}

bool QSettingsManager::ShouldCheckUpdate()
{
    qint64 last = FetchLastUpdateCheckUnix();
    qint64 now = QDateTime::currentSecsSinceEpoch();

    return (now - last) > MinutesToSeconds(0);
}

qint64 QSettingsManager::MinutesToSeconds(int minutes)
{
    return static_cast<qint64>(minutes) * 60;
}

QString QSettingsManager::GenerateUUID()
{
    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces).toLower();
    setValue(UUID, uuid);
    return uuid;

}
