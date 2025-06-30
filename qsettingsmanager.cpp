#include "qsettingsmanager.h"
#include <QSettings>
#include <QCoreApplication>

const QList<QString> QSettingsManager::SettingsForHotkeyGroup = {
    "Hotkeys/ToggleStopwatchHotkey",
    "Hotkeys/ResetStopwatchHotkey"
};

QSettingsManager::QSettingsManager()
    : settings(QString("%1/%2").arg(QCoreApplication::applicationDirPath(),"StopwatchSettings.ini"), QSettings::Format::IniFormat)

{
    setValue("Application", "Version", "0.1-alpha");
}

void QSettingsManager::setValue(QString groupName, QString valueName, QString value)
{
    settings.setValue(QString("%1/%2").arg(groupName, valueName), value);
}

const QVariant QSettingsManager::getValue(enum SettingsForHotkeyGroupIndex e)
{
    return settings.value(SettingsForHotkeyGroup[e], false);
}

const QVariant QSettingsManager::getValue(QString groupName, QString valueName)
{
    return settings.value(QString("%1/%2").arg(groupName, valueName),false);
}
