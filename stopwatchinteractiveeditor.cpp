#include "stopwatchinteractiveeditor.h"
#include "ui_stopwatchinteractiveeditor.h"
#include "mainwindow.h"
#include "stylesheetgenerator.h"
#include "qsettingsmanager.h"
#include "colorpickerdialog.h"

#include <QCloseEvent>
#include <QPushButton>
#include <QMessageBox>
#include <QDateTime>
#include <QString>



StopwatchInteractiveEditor::StopwatchInteractiveEditor(QWidget *parent, MainWindow *mwr)
    : QWidget(parent)
    , ui(new Ui::StopwatchInteractiveEditor)
    , mw{mwr}
{
    ui->setupUi(this);
    instances++;
    mw->qhm.UpdateHotkeySignalBlock(true);
    connect(this, &StopwatchInteractiveEditor::toggleModuleSignal, mw->stm, &SystemTimeModule::setLoadModule);
}

StopwatchInteractiveEditor::~StopwatchInteractiveEditor()
{
    delete ui;
    instances--;
    mw->qhm.UpdateHotkeySignalBlock(false);
}

void StopwatchInteractiveEditor::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    this->setStyleSheet(QString("%1"
                                " %2"
                                " %3"
                                " %4").arg(
                                StylesheetGenerator::DefaultLabel(),
                                StylesheetGenerator::DefaultWidgetBackground(),
                                StylesheetGenerator::DefaultQKeySequenceEditStyle(12, mw->FetchStopwatchFontColorAsHex()),
                                StylesheetGenerator::DefaultButtonStyle(12, mw->FetchStopwatchFontColorAsHex())));
    ui->FontPickerCombo->setCurrentText(mw->GetCurrentFont().family());
    ui->FontPickerComboClock->setCurrentText(mw->stm->GetCurrentFont().family());
    ui->EditorHeaderText->setStyleSheet(StylesheetGenerator::DefaultHeader());
    ui->EditorHotkeyHeaderText->setStyleSheet(StylesheetGenerator::DefaultHeader());
    ui->CustomizationClockHeaderText_2->setStyleSheet(StylesheetGenerator::DefaultHeader());
    ui->CustomizationModulesHeaderText_2->setStyleSheet(StylesheetGenerator::DefaultHeader());
    ui->CustomizationStopwatchHeaderText_2->setStyleSheet(StylesheetGenerator::DefaultHeader());
    ui->ToggleTabActiveAssignmentLabel->setText(mw->qhm.FetchToggleStopwatchHotkey());
    ui->ResetTabActiveAssignmentLabel->setText(mw->qhm.FetchResetStopwatchHotkey());
    ui->BringToForegroundTabActiveAssignmentLabel->setText(mw->qhm.FetchBringToForegroundHotkey());
    //ui->quitStopwatch->setStyleSheet(StylesheetGenerator::DefaultDangerButton());
    UpdateSystemModuleTogglePushButton();

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 elapsedMs = now - mw->Uptime;

    double elapsedHours = elapsedMs / 3600000.0;
    int elapsedMinutes = elapsedMs / 60000;

    ui->UptimeLabel->setText(QString("Uptime ~ %1 hours (%2 minutes)").arg(QString::number(elapsedHours, 'f', 2)).arg(elapsedMinutes));

}

void StopwatchInteractiveEditor::closeEvent(QCloseEvent *event)
{
    mw->qsm.setValue(QSettingsManager::StopwatchFont,ui->FontPickerCombo->currentText());
    mw->qsm.setValue(QSettingsManager::ClockFont,ui->FontPickerComboClock->currentText());
    event->accept();
}

void StopwatchInteractiveEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::MouseButton::RightButton)) {
        const QPointF delta = event->globalPosition() - oldPosition;
        move(x()+delta.x(), y()+delta.y());
        oldPosition = event->globalPosition();
    }
}

void StopwatchInteractiveEditor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::MouseButton::RightButton)
        oldPosition = event->globalPosition();
}

void StopwatchInteractiveEditor::UpdateSystemModuleTogglePushButton()
{
    bool check = mw->stm->CheckIfModuleIsEnabled();
    ui->ToggleSystemModulePushButton->setText(check ? "Disable System Clock Module": "Enable System Clock Module");

    // if (check)
    //     ui->ToggleSystemModulePushButton->setStyleSheet(StylesheetGenerator::DefaultDangerButton());
    // else
    //     ui->ToggleSystemModulePushButton->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->FetchStopwatchFontColorAsHex()));

    ui->ToggleSystemModulePushButton->repaint();
}

void StopwatchInteractiveEditor::on_FontPickerCombo_currentFontChanged(const QFont &f)
{
    mw->UpdateStopwatchFont(f.family(),mw->GetCurrentFont().pointSize());
}


void StopwatchInteractiveEditor::on_FontPickerResetButton_clicked()
{
    ui->FontPickerCombo->setCurrentText(StylesheetGenerator::DefaultFont.family());
}




void StopwatchInteractiveEditor::on_ToggleTabApplyNewHotkey_clicked()
{
    if (mw->qhm.IsHotkeyAvailable(ui->ToggleTabHotkeyInput->keySequence().toString(),true)) {
        QString hotkeyName = ui->ToggleTabHotkeyInput->keySequence().toString();
        mw->qhm.ReassignHotkey(QHotkeyManager::AvailableHotkeys::ToggleKey,hotkeyName);
        ui->ToggleTabActiveAssignmentLabel->setText(hotkeyName);
        mw->qsm.setValue(QSettingsManager::ToggleKey,hotkeyName);
    }
}


void StopwatchInteractiveEditor::on_ResetTabApplyNewHotkey_clicked()
{
    if (mw->qhm.IsHotkeyAvailable(ui->ResetTabHotkeyInput->keySequence().toString(),true)) {
        QString hotkeyName = ui->ResetTabHotkeyInput->keySequence().toString();
        mw->qhm.ReassignHotkey(QHotkeyManager::AvailableHotkeys::ResetKey,hotkeyName);
        ui->ResetTabActiveAssignmentLabel->setText(hotkeyName);
        mw->qsm.setValue(QSettingsManager::ResetKey,hotkeyName);
    }
}


void StopwatchInteractiveEditor::on_BringToForegroundTabApplyNewHotkey_clicked()
{
    if (mw->qhm.IsHotkeyAvailable(ui->BringToForegroundTabHotkeyInput->keySequence().toString(),true)) {
        QString hotkeyName = ui->BringToForegroundTabHotkeyInput->keySequence().toString();
        mw->qhm.ReassignHotkey(QHotkeyManager::AvailableHotkeys::BringToForeground,hotkeyName);
        ui->BringToForegroundTabActiveAssignmentLabel->setText(hotkeyName);
        mw->qsm.setValue(QSettingsManager::BringToForeground,hotkeyName);
    }
}


void StopwatchInteractiveEditor::on_closeWindow_clicked()
{
    this->close();
}


void StopwatchInteractiveEditor::on_quitStopwatch_clicked()
{
    QApplication::quit();
}




void StopwatchInteractiveEditor::on_FontPickerComboClock_currentFontChanged(const QFont &f)
{
    mw->stm->UpdateClockFont(f.family(),mw->GetCurrentFont().pointSize());
}


void StopwatchInteractiveEditor::on_FontSyncWithStopwatchPushbutton_clicked()
{
    ui->FontPickerComboClock->setCurrentFont(mw->GetCurrentFont());
}


void StopwatchInteractiveEditor::on_ToggleSystemModulePushButton_clicked()
{
    bool clockState = mw->stm->CheckIfModuleIsEnabled();

    // Set the module as the opposite of whatever state it currently is

    emit toggleModuleSignal(!clockState);
    UpdateSystemModuleTogglePushButton();

}


void StopwatchInteractiveEditor::on_pushButton_clicked()
{
    auto cpd = new ColorPickerDialog();
    auto val = cpd->exec();

    if (val == QDialog::Accepted) {
        auto color = cpd->FetchColorSelection();
        mw->UpdateStopwatchColor(color);
        mw->qsm.setValue(QSettingsManager::StopwatchFontColor,color.name());

        this->setStyleSheet(this->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color.name()));
    }

}


void StopwatchInteractiveEditor::on_pushButton_2_clicked()
{
    auto cpd = new ColorPickerDialog();
    auto val = cpd->exec();

    if (val == QDialog::Accepted) {
        auto color = cpd->FetchColorSelection();
        mw->stm->UpdateClockFontColor(color);
        mw->qsm.setValue(QSettingsManager::ClockFontColor,color.name());
    }
}


void StopwatchInteractiveEditor::on_pushButton_3_clicked()
{
    auto color = StylesheetGenerator::DefaultFontHexColor;
    mw->UpdateStopwatchColor(color);
    mw->qsm.setValue(QSettingsManager::StopwatchFontColor,color);
    this->setStyleSheet(this->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color));
}


void StopwatchInteractiveEditor::on_pushButton_4_clicked()
{
    auto color = StylesheetGenerator::DefaultFontHexColor;
    mw->stm->UpdateClockFontColor(color);
    mw->qsm.setValue(QSettingsManager::ClockFontColor,color);
}

