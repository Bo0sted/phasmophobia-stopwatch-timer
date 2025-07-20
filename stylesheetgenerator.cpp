#include "stylesheetgenerator.h"

#include <QString>
#include <QPair>
#include <QVector>
#include <QColor>

const QFont StylesheetGenerator::DefaultFont = QFont("DejaVu Sans");
const QString StylesheetGenerator::DefaultFontHexColor = QString("#42C0FB");
const QString StylesheetGenerator::DefaultPausedFontHexColor = QString("#FBA16C");
const QString StylesheetGenerator::DefaultWidgetDarkModeBackground = QString("#1A1A1A");
const QVector<QPair<QString,QString>> StylesheetGenerator::NiceColors = {
    {"#BE2625", "strawberry"},
    {"#F08080", "lightcoral"},
    {"#FF7F50", "coral"},
    {"#FCDC3B", "pineapple"},
    {"#5EDA9E", "fresh green"},
    {"#42C0FB", "caribbean"},
    {"#EEAEEE", "plum2"},
    {"#F54D70", "cranberry jello"},
    {"#3A3A38", "blackberry"}
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
                   "QPushButton:pressed {"
                   "border-style: inset;"
                   "}").arg(fontSize).arg(backgroundColor, borderColor).arg("white");
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
                   "background-color: #EE2C2C;"
                   "border-width: 2px;"
                   "border-color: #BE2625;}");

}

const QString StylesheetGenerator::NewModuleOutputStylesheet(QColor color)
{
    return QString("QLabel "
                   "{ "
                   "background-color : black; "
                   "color : %1; "
                   "}").arg(color.name());
}

