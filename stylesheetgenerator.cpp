#include "stylesheetgenerator.h"

#include <QString>

const QFont StylesheetGenerator::DefaultFont = QFont("DejaVu Sans");
const QString StylesheetGenerator::DefaultFontHexColor = QString("#87CEFF");

StylesheetGenerator::StylesheetGenerator()
{
}

const QString StylesheetGenerator::DefaultHeaderTextDejaVu(int fontSize)
{
    return QString("QLabel { "
                   "font-family: 'DejaVu Sans Bold'; "
                   "color: 'White';"
                   "font-size: %1pt;"
                   "}").arg(fontSize);
}

const QString StylesheetGenerator::DefaultLabel(int fontSize)
{
    return QString("QLabel { "
                   "font-family: 'DejaVu Sans'; "
                   "color: 'White';"
                   "font-size: %1pt;"
                   "}").arg(fontSize);
}
const QString StylesheetGenerator::DefaultDialogueBackground(QString hex)
{
    return QString("QDialog {"
                    "background-color: '%1'"
                   "}").arg(hex);
}

const QString StylesheetGenerator::DefaultButtonStyle(int fontSize)
{
    return QString("QButton {"
                   "font-size: %1pt;"
                   "}").arg(fontSize);
}
