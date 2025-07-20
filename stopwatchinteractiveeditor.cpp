#include "ui_stopwatchinteractiveeditor.h"
#include "mainwindow.h"
#include "stylesheetgenerator.h"
#include "qsettingsmanager.h"
#include "colorpickerdialog.h"
#include "systemtimemodule.h"
#include "stopwatchinteractiveeditor.h"

#include <QCloseEvent>
#include <QPushButton>
#include <QMessageBox>
#include <QDateTime>
#include <QString>

StopwatchInteractiveEditor::StopwatchInteractiveEditor(QWidget *parent, MainWindow *mwr)
    : QWidget(parent)
    , ui(new Ui::StopwatchInteractiveEditor)
    , mw{mwr}
    , open{false}
{
    ui->setupUi(this);
}

StopwatchInteractiveEditor::~StopwatchInteractiveEditor()
{
    delete ui;
}

void StopwatchInteractiveEditor::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

}

void StopwatchInteractiveEditor::closeEvent(QCloseEvent *event)
{
    event->accept();
    mw->qsm.setValue(QSettingsManager::StopwatchFont,ui->FontPickerCombo->currentText());
    mw->qsm.setValue(QSettingsManager::ClockFont,ui->FontPickerComboClock->currentText());
    mw->qsm.setValue(QSettingsManager::IsClockEnabled,QString("%1").arg(mw->stm->CheckIfModuleIsEnabled()));
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

bool StopwatchInteractiveEditor::event(QEvent *event)
{
    if (event->type() == QEvent::Polish) {
        this->setStyleSheet(QString("%1"
                                    " %2"
                                    " %3"
                                    " %4").arg(
                                    StylesheetGenerator::DefaultLabel(),
                                    StylesheetGenerator::DefaultWidgetBackground(),
                                    StylesheetGenerator::DefaultQKeySequenceEditStyle(12, mw->qsm.FetchStopwatchFontColor()),
                                    StylesheetGenerator::DefaultButtonStyle(12)));
        ui->FontPickerCombo->setCurrentText(mw->GetCurrentFont().family());
        ui->FontPickerComboClock->setCurrentText(mw->stm->GetCurrentFont().family());
        ui->EditorHeaderText->setStyleSheet(StylesheetGenerator::DefaultHeader());
        ui->CustomizationModulesHeaderText_2->setStyleSheet(StylesheetGenerator::DefaultHeader());
        ui->ToggleTabActiveAssignmentLabel->setText(mw->qhm.FetchToggleStopwatchHotkey());
        ui->ResetTabActiveAssignmentLabel->setText(mw->qhm.FetchResetStopwatchHotkey());
        //ui->quitStopwatch->setStyleSheet(StylesheetGenerator::DefaultDangerButton());
        UpdateSystemModuleTogglePushButton();
        ui->primaryColorPickerPushButton->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->qsm.FetchStopwatchFontColor()));
        ui->pausedColorPickerPushButton->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->qsm.FetchPausedStopwatchFontColor()));
        ui->ResetColorPickerPushButton->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->qsm.FetchResetStopwatchFontColor()));
        ui->pushButton_2->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->qsm.FetchClockFontColor()));

        qint64 now = QDateTime::currentMSecsSinceEpoch();
        qint64 elapsedMs = now - mw->Uptime;

        double elapsedHours = elapsedMs / 3600000.0;
        int elapsedMinutes = elapsedMs / 60000;

        ui->UptimeLabel->setText(QString("Uptime ~ %1 hours (%2 minutes)").arg(QString::number(elapsedHours, 'f', 2)).arg(elapsedMinutes));
    }

    return QWidget::event(event);

}

void StopwatchInteractiveEditor::UpdateSystemModuleTogglePushButton()
{
    bool check = mw->stm->CheckIfModuleIsEnabled();
    ui->ToggleSystemModulePushButton->setText(check ? "Disable System Clock Module": "Enable System Clock Module");
    ui->SettingsTabWidget->setTabText(ui->SettingsTabWidget->indexOf(ui->ClockSettingsTab), (check ? "Clock": "Clock (Disabled)"));

    // if (check)
    //     ui->ToggleSystemModulePushButton->setStyleSheet(StylesheetGenerator::DefaultDangerButton());
    // else
    //     ui->ToggleSystemModulePushButton->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->FetchStopwatchFontColorAsHex()));

    ui->ToggleSystemModulePushButton->repaint();
}

void StopwatchInteractiveEditor::RefreshOpenState()
{
    this->setVisible(open);

    if (open) {
        mw->qhm.UpdateHotkeySignalBlock(true);
    }
    else {
        mw->qhm.UpdateHotkeySignalBlock(false);
    }
}

void StopwatchInteractiveEditor::setEditorOpen(bool shouldOpen)
{
    open = shouldOpen;
    RefreshOpenState();
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
        mw->qhm.AssignHotkey(QHotkeyManager::AvailableHotkeys::ToggleKey,hotkeyName);
        ui->ToggleTabActiveAssignmentLabel->setText(hotkeyName);
        mw->qsm.setValue(QSettingsManager::ToggleKey,hotkeyName);
    }
}


void StopwatchInteractiveEditor::on_ResetTabApplyNewHotkey_clicked()
{
    if (mw->qhm.IsHotkeyAvailable(ui->ResetTabHotkeyInput->keySequence().toString(),true)) {
        QString hotkeyName = ui->ResetTabHotkeyInput->keySequence().toString();
        mw->qhm.AssignHotkey(QHotkeyManager::AvailableHotkeys::ResetKey,hotkeyName);
        ui->ResetTabActiveAssignmentLabel->setText(hotkeyName);
        mw->qsm.setValue(QSettingsManager::ResetKey,hotkeyName);
    }
}




void StopwatchInteractiveEditor::on_closeWindow_clicked()
{
    setEditorOpen(false);
}


void StopwatchInteractiveEditor::on_quitStopwatch_clicked()
{
    mw->BeginShutdown();
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




void StopwatchInteractiveEditor::on_pushButton_3_clicked()
{
    auto color = StylesheetGenerator::DefaultFontHexColor;
    mw->UpdateStopwatchColor(color);
    mw->qsm.setValue(QSettingsManager::StopwatchFontColor,color);
    ui->primaryColorPickerPushButton->setStyleSheet(ui->primaryColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color));
    mw->RefreshStopwatchColor(true);
}


void StopwatchInteractiveEditor::on_pushButton_4_clicked()
{
    auto color = StylesheetGenerator::DefaultFontHexColor;
    mw->stm->UpdateClockFontColor(color);
    mw->qsm.setValue(QSettingsManager::ClockFontColor,color);
}



void StopwatchInteractiveEditor::on_PausedColorResetPushButton_clicked()
{
    auto color = StylesheetGenerator::DefaultPausedFontHexColor;
    mw->UpdateStopwatchResetColor(color);
    mw->RefreshStopwatchColor(true);
    mw->qsm.setValue(QSettingsManager::StopwatchPausedFontColor,color);
    ui->pausedColorPickerPushButton->setStyleSheet(ui->pausedColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color));
}

void StopwatchInteractiveEditor::on_primaryColorPickerPushButton_pressed()
{
    auto cpd = new ColorPickerDialog();
    auto val = cpd->exec();

    if (val == QDialog::Accepted) {
        auto color = cpd->FetchColorSelection();
        mw->UpdateStopwatchColor(color);
        mw->qsm.setValue(QSettingsManager::StopwatchFontColor,color.name());
        mw->RefreshStopwatchColor(true);
        ui->primaryColorPickerPushButton->setStyleSheet(ui->primaryColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color.name()));
    }
}

void StopwatchInteractiveEditor::on_pausedColorPickerPushButton_pressed()
{
    auto cpd = new ColorPickerDialog();
    auto val = cpd->exec();

    if (val == QDialog::Accepted) {
        auto color = cpd->FetchColorSelection();
        mw->UpdateStopwatchPausedColor(color);
        mw->qsm.setValue(QSettingsManager::StopwatchPausedFontColor,color.name());
        mw->RefreshStopwatchColor(true);
        ui->pausedColorPickerPushButton->setStyleSheet(ui->pausedColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color.name()));
    }
}


void StopwatchInteractiveEditor::on_ResetColorPickerPushButton_pressed()
{
    auto cpd = new ColorPickerDialog();
    auto val = cpd->exec();

    if (val == QDialog::Accepted) {
        auto color = cpd->FetchColorSelection();
        mw->UpdateStopwatchResetColor(color);
        mw->qsm.setValue(QSettingsManager::StopwatchResetFontColor,color.name());
        mw->RefreshStopwatchColor(true);
        ui->ResetColorPickerPushButton->setStyleSheet(ui->ResetColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color.name()));
    }
}


void StopwatchInteractiveEditor::on_ResetColorResetPushButton_clicked()
{
    auto color = StylesheetGenerator::DefaultPausedFontHexColor;
    mw->UpdateStopwatchResetColor(color);
    mw->RefreshStopwatchColor(true);
    mw->qsm.setValue(QSettingsManager::StopwatchResetFontColor,color);
    ui->ResetColorPickerPushButton->setStyleSheet(ui->ResetColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color));
}




void StopwatchInteractiveEditor::on_pushButton_2_pressed()
{
    auto cpd = new ColorPickerDialog();
    auto val = cpd->exec();

    if (val == QDialog::Accepted) {
        auto color = cpd->FetchColorSelection();
        mw->stm->UpdateClockFontColor(color);
        mw->qsm.setValue(QSettingsManager::ClockFontColor,color.name());
    }
}

