#ifndef QSETTINGSMANAGER_H
#define QSETTINGSMANAGER_H

#include <QSettings>
#include <QList>

class QSettingsManager
{
public:
    enum SettingsForHotkeyGroupIndex {
        ToggleKey,
        ResetKey
    };
    static const QList<QString> SettingsForHotkeyGroup;

    QSettingsManager();
    void setValue(QString groupName, QString valueName, QString value);
    const QVariant getValue(enum SettingsForHotkeyGroupIndex e);
    const QVariant getValue(QString groupName, QString valueName);
private:
    QSettings settings;
};

#endif // QSETTINGSMANAGER_H
