#ifndef STYLESHEETGENERATOR_H
#define STYLESHEETGENERATOR_H

#include <QString>
#include <QFont>
#include <QPair>
#include <QVector>

class StylesheetGenerator
{

public:
    StylesheetGenerator();
    static const QString DefaultHeader(int fontSize=24, QString color="White", QString background="Transparent");
    static const QString DefaultLabel(int fontSize=12);
    static const QString DefaultWidgetBackground();
    static const QString DefaultButtonStyle(int fontSize=12, QString backgroundColor=DefaultFontHexColor, QString borderColor="white");
    static const QString DefaultQKeySequenceEditStyle(int fontSize=12, QString backgroundColor="white", QString borderColor="white");
    static const QString DefaultDangerButton();
    static const QString NewStopwatchStylesheet(QColor color);
    static const QString NewModuleOutputStylesheet(QColor color);

    static const QFont DefaultFont;
    static const QString DefaultFontHexColor;
    static const QVector<QPair<QString,QString>> NiceColors;
    static const QString DefaultWidgetDarkModeBackground;

};

#endif // STYLESHEETGENERATOR_H
