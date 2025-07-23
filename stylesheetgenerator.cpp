#include "stylesheetgenerator.h"

#include <QString>
#include <QPair>
#include <QVector>
#include <QColor>

const QFont StylesheetGenerator::DefaultFont = QFont("DejaVu Sans");
const QString StylesheetGenerator::DefaultFontHexColor = QString("#42C0FB");
const QString StylesheetGenerator::DefaultPausedFontHexColor = QString("#FBA16C");
const QString StylesheetGenerator::DefaultWidgetDarkModeBackground = QString("#1A1A1A");
const QString StylesheetGenerator::DefaultStopwatchBackground = QString("black");
const QString StylesheetGenerator::DefaultStopwatchBorderColor = QString("black");
const QString StylesheetGenerator::DefaultStopwatchBorderWidth = QString("0px");
const QVector<QPair<QString,QString>> StylesheetGenerator::NiceColors = {
    {"#ffffff", "white"},
    {"#BB2A3C", "braeburn apple"},
    {"#F08080", "lightcoral"},
    {"#FF7F50", "coral"},
    {"#FCD59C", "bread"},
    {"#FFD700", "gold"},
    {"#5EDA9E", "fresh green"},
    {"#00C957", "emeraldgreen"},
    {"#42C0FB", "caribbean"},
    {"#4973AB", "blue bird"},
    {"#F6A4D5", "dog tongue"},
    {"#F54D70", "cranberry jello"},
    {"#CC00FF", "grape"},
    {"#3A3A38", "blackberry"},
    {"#000000", "black"}
};

StylesheetGenerator::StylesheetGenerator()
{
}

const QString StylesheetGenerator::DefaultHeader(int fontSize, QString color, QString background)
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

const QString StylesheetGenerator::DefaultDangerLabel(int fontSize)
{
    return QString("QLabel { "
                   "font-family: DejaVu Sans; "
                   "font-size: %1pt;"
                   "color: #EE2C2C;"
                   "}").arg(fontSize);
}
const QString StylesheetGenerator::DefaultWidgetBackground()
{
    //hex = "Red";
    return QString("QWidget, QFrame {"
                    "background-color: %1;"
                    "color: White;"
                   "font-size: 12pt;"
                   "}"
                   "QSplitter {"
                   "background-color: %1;"
                   "color: White;"
                   "}"
                   "QTabWidget {"
                   "background-color: %1;"
                    "color: White;"
                   "}").arg(DefaultWidgetDarkModeBackground);
}

const QString StylesheetGenerator::DefaultButtonStyle(int fontSize, QString backgroundColor, QString borderColor)
{
    return QString("QPushButton {"
                   "color: %4;"
                   "font-size: %1pt;"
                   "font-weight: medium;"
                   "background-color: %2;"
                   "border-style: outset;"
                   "border-width: 2px;"
                   "border-radius: 10px;"
                   "border-color: %3;"
                   "padding: 6px;"
                   "}"
                   "QPushButton:hover {"
                   "  text-decoration: underline;"
                   "}"
                   "QPushButton:pressed {"
                   "border-style: inset;"
                   "padding-left: 7px;"
                   "padding-top: 7px;"
                   "}").arg(fontSize).arg(backgroundColor).arg(borderColor).arg("white");
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

const QString StylesheetGenerator::DefaultDangerButton()
{
    return QString("QPushButton { "
                   "color: White;"
                   //"background-color: #EE2C2C;"
                   "border-width: 2px;"
                   "border-color: #EE2C2C;}");
                   //"border-color: #BE2625;}");

}

const QString StylesheetGenerator::NewStopwatchStylesheet(QColor textColor, QColor backgroundColor)
{
    return QString("QLabel "
                   "{ "
                   "background-color : %2; "
                   "color : %1; "
                   //"border: %4 solid %3;"
                   "}").arg(textColor.name())
                       .arg(backgroundColor.name());
        // .arg(border.name()) // Border color
        // .arg(thickness); // Border thickness
}

const QString StylesheetGenerator::NewModuleOutputStylesheet(QColor color, QColor background, QColor border, QString thickness)
{
    if (!thickness.endsWith("px"))
        thickness = "0px";
    return QString("QLabel "
                   "{ "
                   "background-color : %2; "
                   "color : %1; "
                   "border: %4 solid %3;"
                   "}").arg(color.name()) // Text color
                    .arg(background.name()) // Background color
                    .arg(border.name()) // Border color
                    .arg(thickness); // Border thickness
}

