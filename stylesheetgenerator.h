#ifndef STYLESHEETGENERATOR_H
#define STYLESHEETGENERATOR_H

#include <QString>
#include <QFont>
#include <QPair>
#include <QVector>
#include <QColor>

class StylesheetGenerator
{

public:
    StylesheetGenerator();
    static const QString DefaultHeader(int fontSize=24, QString color="White", QString background="Transparent");
    static const QString DefaultLabel(int fontSize=12);
    static const QString DefaultDangerLabel(int fontSize=12);
    static const QString DefaultWidgetBackground();
    static const QString DefaultButtonStyle(int fontSize=12, QString backgroundColor=DefaultWidgetDarkModeBackground, QString borderColor="white");
    static const QString DefaultQKeySequenceEditStyle(int fontSize=12, QString backgroundColor="white", QString borderColor="white");
    static const QString DefaultDangerButton();
    static const QString NewStopwatchStylesheet(QColor textColor, QColor backgroundColor);
    static const QString NewModuleOutputStylesheet(QColor color, QColor background=QColor::fromString("black"), QColor border=QColor::fromString("black"), QString thickness="2px");

    static const QFont DefaultFont;
    static const QString DefaultFontHexColor;
    static const QString DefaultPausedFontHexColor;
    static const QString DefaultGradientOneFontHexColor;
    static const QString DefaultGradientTwoFontHexColor;
    static const QVector<QPair<QString,QString>> NiceColors;
    static const QString DefaultWidgetDarkModeBackground;
    static const QString DefaultStopwatchBackground;
    static const QString DefaultStopwatchBorderColor;
    static const QString DefaultStopwatchBorderWidth;

};

#endif // STYLESHEETGENERATOR_H
