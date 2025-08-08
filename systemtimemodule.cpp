#include "systemtimemodule.h"
#include "ui_systemtimemodule.h"
#include "mainwindow.h"
#include "stylesheetgenerator.h"

#include <QtConcurrent>
#include <QTime>
#include <QFont>

SystemTimeModule::SystemTimeModule(QWidget *parent, MainWindow *mwr)
    : QWidget(parent)
    , ui(new Ui::SystemTimeModule)
    , mw(mwr)
    , isDeconstructing{false}
    , enabled{mw->qsm.CheckIfClockEnabled()}
    , refreshClockThread(QtConcurrent::run(&SystemTimeModule::RefreshClockThread, this))
{
    ui->setupUi(this);
    connect(this, &SystemTimeModule::signalRefreshClock, this, &SystemTimeModule::updateClock);
}

SystemTimeModule::~SystemTimeModule()
{
    isDeconstructing = true;
    refreshClockThread.cancel();
    refreshClockThread.waitForFinished();
    delete ui;
}

void SystemTimeModule::showEvent(QShowEvent *event)
{
    event->accept();
    systemClockBackgroundColor = "black";
    systemClockFontColor = mw->qsm.FetchClockFontColor();
    QString style = StylesheetGenerator::NewModuleOutputStylesheet(systemClockFontColor, systemClockBackgroundColor);
    ui->SystemTimeLabel->setStyleSheet(style);
    UpdateClockFont(mw->qsm.FetchClockFont(), mw->qsm.FetchClockFontSize());


}

void SystemTimeModule::closeEvent(QCloseEvent *event)
{
    QString spos = QString("%1,%2").arg(this->pos().x()).arg(this->pos().y());
    mw->qsm.setValue(QSettingsManager::LastClockPosition, spos);
    mw->qsm.setValue(QSettingsManager::IsClockEnabled, QString("%1").arg(enabled));
    event->accept();
}

void SystemTimeModule::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::MouseButton::RightButton)) {
        const QPointF delta = event->globalPosition() - oldPosition;
        move(x()+delta.x(), y()+delta.y());
        oldPosition = event->globalPosition();
    }
}

void SystemTimeModule::mousePressEvent(QMouseEvent *event)
{
    auto rawEvent = event;
    auto button = event->button();
    if (button != Qt::MouseButton::RightButton)
        oldPosition = event->globalPosition();
    else if (button == Qt::MouseButton::RightButton) {
        mw->sie->setVisible(true);
    }
}

void SystemTimeModule::ResizeClockToFitWindow()
{

    ui->SystemTimeLabel->adjustSize();

    QSize centralSize = ui->SystemTimeLabel->sizeHint();

    // Calculate frame size difference (window decorations)
    int frameWidth = this->frameGeometry().width() - this->geometry().width();
    int frameHeight = this->frameGeometry().height() - this->geometry().height();

    // Resize main window explicitly
    this->resize(centralSize.width() + frameWidth,
                 centralSize.height() + frameHeight);
}

void SystemTimeModule::RefreshClockThread()
{
    qDebug() << "System Time Module initialized";
        while (!isDeconstructing) {
            QThread::msleep(1000);
            QString currentTimeString = QTime::currentTime().toString("h:mm ap");
            emit signalRefreshClock(currentTimeString);
        }
}

void SystemTimeModule::UpdateClockFont(QString fontName, int fontSize)
{
    ui->SystemTimeLabel->setFont(QFont(fontName, fontSize));
}

void SystemTimeModule::UpdateClockFontColor(QColor color)
{
    systemClockFontColor = color;
    ui->SystemTimeLabel->setStyleSheet(StylesheetGenerator::NewModuleOutputStylesheet(color));
}

void SystemTimeModule::UpdateClockBackgroundColor(QColor color)
{
    systemClockBackgroundColor = color;
    ui->SystemTimeLabel->setStyleSheet(StylesheetGenerator::NewModuleOutputStylesheet(systemClockFontColor,color));
}

QFont SystemTimeModule::GetCurrentFont()
{
    return ui->SystemTimeLabel->font();
}


bool SystemTimeModule::CheckIfModuleIsEnabled()
{
    return enabled;
}

void SystemTimeModule::RefreshModuleState()
{
    this->setVisible(enabled);
}

int SystemTimeModule::GetCurrentFontSize()
{
    return ui->SystemTimeLabel->font().pointSize();
}

void SystemTimeModule::updateClock(const QString &time)
{
    ui->SystemTimeLabel->setText(time);
    ResizeClockToFitWindow();
}

void SystemTimeModule::setLoadModule(bool shouldEnable)
{
    enabled = shouldEnable;
    RefreshModuleState();
}

void SystemTimeModule::refreshColorState(QColor color)
{
    //        ui->rainbowColorComboBox->addItems({"Disabled", "Text", "Background"});

    auto mode = mw->GetRainbowMode();
    if (mode ==  1)
        ui->SystemTimeLabel->setStyleSheet(StylesheetGenerator::NewModuleOutputStylesheet(color.name(), systemClockBackgroundColor.name()));
    else if (mode == 2)
        ui->SystemTimeLabel->setStyleSheet(StylesheetGenerator::NewModuleOutputStylesheet(systemClockFontColor.name(), color.name(), color.name()));
    else
        ui->SystemTimeLabel->setStyleSheet(StylesheetGenerator::NewModuleOutputStylesheet(systemClockFontColor.name(), systemClockBackgroundColor.name()));

}
