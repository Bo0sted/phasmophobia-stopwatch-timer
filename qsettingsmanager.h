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
        BackgroundColor
    };

    const QList<QString> SettingsForHotkeyGroup;
    const QList<QString> SettingsForStopwatchGroup;

    QSettingsManager();
    void setValue(QString groupName, QString valueName, QString value);
    void setValue(enum SettingsForStopwatchGroupIndex e, QString value);
    void setValue(enum SettingsForHotkeyGroupIndex e, QString value);
    const QVariant getValue(enum SettingsForHotkeyGroupIndex e);
    const QVariant getValue(enum SettingsForStopwatchGroupIndex e);
    const QVariant getValue(QString groupName, QString valueName);

    QString FetchStopwatchFontColor();
    QString FetchStopwatchFont();
private:
    QSettings settings;
};

#endif // QSETTINGSMANAGER_H
