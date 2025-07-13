#ifndef STYLESHEETGENERATOR_H
#define STYLESHEETGENERATOR_H

#include <QString>
#include <QFont>

class StylesheetGenerator
{

public:
    StylesheetGenerator();
    static const QString DefaultHeaderTextDejaVu(int fontSize=24, QString color="White", QString background="Transparent");
    static const QString DefaultLabel(int fontSize=12);
    static const QString DefaultWidgetBackground(QString hex="#1A1A1A");
    static const QString DefaultButtonStyle(int fontSize=12, QString backgroundColor="white", QString borderColor="white");
    static const QString DefaultQKeySequenceEditStyle(int fontSize=12, QString backgroundColor="white", QString borderColor="white");
    static const QString DefaultDangerHexColor();

    static const QFont DefaultFont;
    static const QString DefaultFontHexColor;

};

#endif // STYLESHEETGENERATOR_H
