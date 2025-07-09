#ifndef STYLESHEETGENERATOR_H
#define STYLESHEETGENERATOR_H

#include <QString>
#include <QFont>

class StylesheetGenerator
{

public:
    StylesheetGenerator();
    static const QString DefaultHeaderTextDejaVu(int fontSize=24);
    static const QString DefaultLabel(int fontSize=12);
    static const QString DefaultWidgetBackground(QString hex="#0D0D0D");
    static const QString DefaultButtonStyle(int fontSize=12);
    static const QFont DefaultFont;
    static const QString DefaultFontHexColor;

};

#endif // STYLESHEETGENERATOR_H
