#include "stylesheetgenerator.h"

#include <QString>

const QFont StylesheetGenerator::DefaultFont = QFont("DejaVu Sans");
const QString StylesheetGenerator::DefaultFontHexColor = QString("#1D7CF2");

StylesheetGenerator::StylesheetGenerator()
{
}

const QString StylesheetGenerator::DefaultHeaderTextDejaVu(int fontSize, QString color, QString background)
{
    return QString("QLabel { "
                   "font-family: DejaVu Sans Bold;"
                   "font-size: %1pt;"
                   "font-weight: bold;"
                   "color: %2;"
                   "background: %3;"
                   "}").arg(fontSize).arg(color, background);
}

const QString StylesheetGenerator::DefaultLabel(int fontSize)
{
    return QString("QLabel { "
                   "font-family: DejaVu Sans; "
                   "font-size: %1pt;"
                   "}").arg(fontSize);
}
const QString StylesheetGenerator::DefaultWidgetBackground(QString hex)
{
    //hex = "Red";
    return QString("QWidget {"
                    "background-color: %1;"
                    "color: White;"
                   "}"
                   "QSplitter {"
                   "background-color: %1;"
                   "color: White;"
                   "}"
                   "QTabWidget {"
                   "background-color: %1;"
                    "color: White;"
                   "}").arg(hex);
}

const QString StylesheetGenerator::DefaultButtonStyle(int fontSize, QString backgroundColor, QString borderColor)
{
    return QString("QPushButton {"
                   "font-size: %1pt;"
                   "font-weight: bold;"
                   "background-color: %2;"
                   "border-style: outset;"
                   "border-width: 0px;"
                   "border-radius: 10px;"
                   "border-color: %3;"
                   "padding: 6px;"
                   "}"
                   "QPushButton:pressed {"
                   "border-style: inset;"
                   "}").arg(fontSize).arg(backgroundColor, borderColor);
}

const QString StylesheetGenerator::DefaultQKeySequenceEditStyle(int fontSize, QString backgroundColor, QString borderColor)
{
    return QString("QKeySequenceEdit {"
                   "font-size: %1pt;"
                   "font-weight: bold;"
                   "background-color: %2;"
                   "border-width: 2px;"
                   "border-color: %3;"
                   "}").arg(fontSize).arg(backgroundColor, borderColor);
}

const QString StylesheetGenerator::DefaultDangerHexColor()
{
    return QString("QPushButton { "
                   "color: White;"
                   "background-color: #EE2C2C;"
                   "border-width: 2px;"
                   "border-color: #BE2625;}");

}

