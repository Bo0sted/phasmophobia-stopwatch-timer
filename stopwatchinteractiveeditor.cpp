#include "ui_stopwatchinteractiveeditor.h"
#include "mainwindow.h"
#include "stylesheetgenerator.h"
#include "qsettingsmanager.h"
#include "colorpickerdialog.h"
#include "systemtimemodule.h"
#include "stopwatchinteractiveeditor.h"
#include "warmwelcome.h"
#include "uiokey.h"

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
    RefreshUptimeLabel();


}

void StopwatchInteractiveEditor::closeEvent(QCloseEvent *event)
{
    event->accept();
    mw->qsm.setValue(QSettingsManager::StopwatchFont,ui->FontPickerCombo->currentText());
    mw->qsm.setValue(QSettingsManager::ClockFont,ui->FontPickerComboClock->currentText());
    mw->qsm.setValue(QSettingsManager::IsClockEnabled,QString("%1").arg(mw->stm->CheckIfModuleIsEnabled()));
    mw->qsm.setValue(QSettingsManager::StopwatchRainbowModeIndex, QString("%1").arg(ui->rainbowColorComboBox->currentIndex()));
    mw->qsm.setValue(QSettingsManager::StopwatchFormatModeIndex, QString("%1").arg(ui->formatTimeComboBox->currentIndex()));
    mw->qsm.setValue(QSettingsManager::StopwatchFontSize, QString("%1").arg(mw->GetCurrentStopwatchFontSize()));
    mw->qsm.setValue(QSettingsManager::ClockFontSize, QString("%1").arg(mw->stm->GetCurrentFontSize()));
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
        ui->ToggleTabActiveAssignmentLabel->setText(QString("%1").arg(mw->uiohm.GetDisplayFromQListOfKeycodes(mw->uiohm.FetchToggleStopwatchHotkey())));
        ui->ResetTabActiveAssignmentLabel->setText(QString("%1").arg(mw->uiohm.GetDisplayFromQListOfKeycodes(mw->uiohm.FetchResetStopwatchHotkey())));
        ui->RestoreTabActiveAssignmentLabel->setText(QString("%1").arg(mw->uiohm.GetDisplayFromQListOfKeycodes(mw->uiohm.FetchRestoreStopwatchHotkey())));
        ui->quitStopwatch->setStyleSheet(StylesheetGenerator::DefaultDangerButton());
        UpdateSystemModuleTogglePushButton();
        ui->primaryColorPickerPushButton->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->qsm.FetchStopwatchFontColor()));
        ui->pausedColorPickerPushButton->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->qsm.FetchPausedStopwatchFontColor()));
        ui->ResetColorPickerPushButton->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->qsm.FetchResetStopwatchFontColor()));
        ui->pushButton_2->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->qsm.FetchClockFontColor()));
        ui->backgroundColorPickerPushButton->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->qsm.FetchStopwatchBackgroundColor()));
        ui->borderColorPickerPushButton->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12, mw->qsm.FetchStopwatchBorderColor()));

        auto backgroundEnabled = mw->qsm.FetchIsBackgroundEnabled();
        ui->backgroundToggleCheckbox->setChecked(backgroundEnabled ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
        SetBorderOptionsVisible(backgroundEnabled);
        SetBackgroundOptionsEnabled(backgroundEnabled);

        ui->rainbowColorComboBox->addItems({"Disabled", "Text", "Background"});
        ui->formatTimeComboBox->addItems({"Hour:Minute:Second", "Total minutes only", "Total seconds only"});

        readyForUserUIchanges = true;
        ui->rainbowColorComboBox->setCurrentIndex(mw->GetRainbowMode());
        ui->formatTimeComboBox->setCurrentIndex(mw->GetFormatMode());

        ui->ToggleHotkeyRecordPushButton->setFocusPolicy(Qt::ClickFocus);
        ui->FontPickerCombo->setFocusPolicy(Qt::ClickFocus);
        ui->SettingsTabWidget->setFocusPolicy(Qt::ClickFocus);

        connect(ui->FontSizeSlider, &QSlider::valueChanged,
                this, [this](int value) {
                QString fontName = mw->GetCurrentFont().family();
                    mw->UpdateStopwatchFont(fontName, value);
                    mw->ResizeWindowToFitStopwatch();
                });
        connect(ui->ClockFontSizeSlider, &QSlider::valueChanged,
                this, [this](int value) {
                    QString fontName = mw->stm->GetCurrentFont().family();
                    mw->stm->UpdateClockFont(fontName, value);
                    mw->stm->ResizeClockToFitWindow();

                });

        ui->FontSizeSlider->setSliderPosition(mw->GetCurrentStopwatchFontSize());
        ui->ClockFontSizeSlider->setSliderPosition(mw->stm->GetCurrentFontSize());

    }

    return QWidget::event(event);

}

void StopwatchInteractiveEditor::UpdateSystemModuleTogglePushButton()
{
    bool check = mw->stm->CheckIfModuleIsEnabled();
    ui->ToggleSystemModulePushButton->setText(check ? "Disable System Clock Module": "Enable System Clock Module");

    int index = ui->SettingsTabWidget->indexOf(ui->ClockSettingsTab);
    ui->SettingsTabWidget->setTabText(index, (check ? "Clock": "Clock (Disabled)"));
    if (check) ui->SettingsTabWidget->setTabEnabled(index, true);
    else ui->SettingsTabWidget->setTabEnabled(index, false);

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
        mw->uiohm.UpdateHotkeySignalBlock(true);
    }
    else {
        mw->uiohm.UpdateHotkeySignalBlock(false);
    }
}

void StopwatchInteractiveEditor::RefreshUptimeLabel()
{
    ui->UptimeLabel->setText([&]() {
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        qint64 elapsedMs = now - mw->Uptime;
        return QString("Uptime ~ %1 hours (%2 minutes)")
            .arg(QString::number(elapsedMs / 3600000.0, 'f', 2))
            .arg(elapsedMs / 60000);
    }());
}

void StopwatchInteractiveEditor::SetBorderOptionsVisible(bool visible)
{
    ui->borderColorPickerPushButton->setVisible(visible);
    ui->borderColorReseButton->setVisible(visible);
    ui->borderLabel->setVisible(visible);
}

void StopwatchInteractiveEditor::SetBackgroundOptionsEnabled(bool enabled)
{
    ui->backgroundColorPickerPushButton->setEnabled(enabled);
    ui->backgroundColorResetPushButton->setEnabled(enabled);
}

void StopwatchInteractiveEditor::RefreshToggleHotkeyPushButton()
{
    auto mode = mw->uiohm.GetHotkeyAssignMode();
    ui->ToggleHotkeyRecordPushButton->setText(mode ? "Disable" : "Record");
}

void StopwatchInteractiveEditor::RefreshToggleHotkeyAssignModeDisplay()
{
    ui->HotkeyAssignModeDisplay->setText(mw->uiohm.GetDisplayFromQListOfKeycodes(mw->uiohm.GetHotkeyAssignBuffer()));
}


int StopwatchInteractiveEditor::GetActiveTabFromHotkeyGroup() {
    return ui->ToggleHotkeyTabWidget->currentIndex();
}

void StopwatchInteractiveEditor::setEditorOpen(bool shouldOpen)
{
    open = shouldOpen;
    RefreshOpenState();
}

void StopwatchInteractiveEditor::refreshHotkeyDisplays()
{
    QStringList parts;

    for (int mod : mw->uiohm.GetNewModifiers()) {
        switch (mod) {
        case VC_SHIFT_L:   parts << "Left Shift";   break;
        case VC_SHIFT_R:   parts << "Right Shift";  break;
        case VC_CONTROL_L: parts << "Left Ctrl";    break;
        case VC_CONTROL_R: parts << "Right Ctrl";   break;
        case VC_ALT_L:     parts << "Left Alt";     break;
        case VC_ALT_R:     parts << "Right Alt";    break;
        case VC_META_L:    parts << "Left Meta";    break;
        case VC_META_R:    parts << "Right Meta";   break;
        default:           parts << KeycodeToQString(mod); break;
        }
    }

    // If a main key is selected, add it
    int key = mw->uiohm.GetHotkeyToReasign();
    if (key != -1 && !IsKeycodeModifierKey(key, 0)) {
        parts << KeycodeToQString(key); // Convert int → readable key name
    }

    // Join with " + " and set to label
    if (mw->uiohm.GetHotkeyAssignMode()) {
        ui->HotkeyAssignModeDisplay->setText(parts.join(" + "));
    }

    RefreshToggleHotkeyPushButton();
}

void StopwatchInteractiveEditor::on_FontPickerCombo_currentFontChanged(const QFont &f)
{
    mw->UpdateStopwatchFont(f.family(),mw->GetCurrentFont().pointSize());
}


void StopwatchInteractiveEditor::on_FontPickerResetButton_clicked()
{
    ui->FontPickerCombo->setCurrentText(StylesheetGenerator::DefaultFont.family());
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
    mw->stm->UpdateClockFont(f.family(),mw->stm->GetCurrentFont().pointSize());
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
    mw->RefreshStopwatchState(true);
}


void StopwatchInteractiveEditor::on_pushButton_4_clicked()
{
    auto color = StylesheetGenerator::DefaultFontHexColor;
    mw->stm->UpdateClockFontColor(color);
    mw->qsm.setValue(QSettingsManager::ClockFontColor,color);
    ui->pushButton_2->setStyleSheet(ui->pushButton_2->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color));
}



void StopwatchInteractiveEditor::on_PausedColorResetPushButton_clicked()
{
    auto color = StylesheetGenerator::DefaultPausedFontHexColor;
    mw->UpdateStopwatchPausedColor(color);
    mw->RefreshStopwatchState(true);
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
        mw->RefreshStopwatchState(true);
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
        mw->RefreshStopwatchState(true);
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
        mw->RefreshStopwatchState(true);
        ui->ResetColorPickerPushButton->setStyleSheet(ui->ResetColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color.name()));
    }
}


void StopwatchInteractiveEditor::on_ResetColorResetPushButton_clicked()
{
    auto color = StylesheetGenerator::DefaultPausedFontHexColor;
    mw->UpdateStopwatchResetColor(color);
    mw->RefreshStopwatchState(true);
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
        ui->pushButton_2->setStyleSheet(ui->pushButton_2->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color.name()));
    }
}



void StopwatchInteractiveEditor::on_backgroundColorPickerPushButton_pressed()
{
    auto cpd = new ColorPickerDialog();
    auto val = cpd->exec();

    if (val == QDialog::Accepted) {
        auto color = cpd->FetchColorSelection();
        mw->UpdateStopwatchBackground(color);
        mw->qsm.setValue(QSettingsManager::StopwatchBackgroundColor,color.name());
        mw->RefreshStopwatchState(true);
        ui->backgroundColorPickerPushButton->setStyleSheet(ui->backgroundColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color.name()));
    }
}


void StopwatchInteractiveEditor::backgroundColorResetPushButton()
{
    auto color = StylesheetGenerator::DefaultStopwatchBackground;
    mw->UpdateStopwatchBackground(color);
    mw->RefreshStopwatchState(true);
    mw->qsm.setValue(QSettingsManager::StopwatchBackgroundColor,color);
    ui->backgroundColorPickerPushButton->setStyleSheet(ui->backgroundColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color));
}



void StopwatchInteractiveEditor::on_borderColorReseButton_clicked()
{
    auto color = mw->GetBackground().name();
    mw->UpdateStopwatchBorderColor(color);
    mw->RefreshStopwatchState(true);
    mw->qsm.setValue(QSettingsManager::StopwatchBorderColor,color);
    ui->borderColorPickerPushButton->setStyleSheet(ui->borderColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color));
}


void StopwatchInteractiveEditor::on_borderColorPickerPushButton_pressed()
{
    auto cpd = new ColorPickerDialog();
    auto val = cpd->exec();

    if (val == QDialog::Accepted) {
        auto color = cpd->FetchColorSelection();
        mw->UpdateStopwatchBorderColor(color);
        mw->qsm.setValue(QSettingsManager::StopwatchBorderColor,color.name());
        mw->RefreshStopwatchState(true);
        ui->borderColorPickerPushButton->setStyleSheet(ui->borderColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color.name()));
    }
}


void StopwatchInteractiveEditor::on_backgroundToggleCheckbox_checkStateChanged(const Qt::CheckState &arg1)
{
    auto newState = arg1 == Qt::Checked ? true:false;

    mw->SetBackgroundEnabled(newState);
    mw->qsm.setValue(QSettingsManager::StopwatchBackgroundEnabled,QString("%1").arg(newState));
    mw->RefreshStopwatchState(false);
    SetBorderOptionsVisible(newState);
    SetBackgroundOptionsEnabled(newState);
}




void StopwatchInteractiveEditor::on_AboutProgramPushButton_clicked()
{
    auto cpd = new WarmWelcome(mw,this);
    cpd->setWindowFlag(Qt::WindowStaysOnTopHint, true);
    auto val = cpd->exec();
    if (val == QDialog::Accepted) {

    }
}



void StopwatchInteractiveEditor::on_rainbowColorComboBox_currentIndexChanged(int index)
{
    if (readyForUserUIchanges)
        mw->SetRainbowMode(index);
}







void StopwatchInteractiveEditor::on_formatTimeComboBox_currentIndexChanged(int index)
{
    if (readyForUserUIchanges)
        mw->SetFormatMode(static_cast<StopwatchManager::FormatModes>(index));
}

void StopwatchInteractiveEditor::on_backgroundColorResetPushButton_clicked()
{
    auto color = StylesheetGenerator::DefaultStopwatchBackground;
    mw->UpdateStopwatchBackground(color);
    mw->RefreshStopwatchState(true);
    mw->qsm.setValue(QSettingsManager::StopwatchBackgroundColor,color);
    ui->backgroundColorPickerPushButton->setStyleSheet(ui->backgroundColorPickerPushButton->styleSheet() + StylesheetGenerator::DefaultButtonStyle(12, color));
}







void StopwatchInteractiveEditor::on_AssignToggleHotkeyPushButton_clicked()
{
    auto buffer = mw->uiohm.GetHotkeyAssignBuffer();
    auto targetHotkey = mw->uiohm.GetHotkeyForCurrentTab();

    if (buffer.empty()) {
        QMessageBox::information(this, "Empty", "No keys were recorded. Please try again!");
        return;
    }

    if (!mw->uiohm.IsHotkeyAvailable(buffer, targetHotkey, true)) {
        mw->uiohm.AssignHotkey(targetHotkey, buffer);
        mw->uiohm.ClearHotkeyAssignState();
        mw->uiohm.SetHotkeyReassignMode(false);
        RefreshToggleHotkeyPushButton();
        return;
    }

    mw->uiohm.SetHotkeyReassignMode(false);
    RefreshToggleHotkeyPushButton();

    QVariantList keycodes = {};

    for (const auto i: buffer)
        keycodes.append(i);

    mw->uiohm.AssignHotkey(targetHotkey, buffer);

    QSettingsManager::SettingsForHotkeyGroupIndex settingsKey;
    switch (targetHotkey) {
    case UioHotkeyManager::ToggleKey:  settingsKey = QSettingsManager::ToggleKey; break;
    case UioHotkeyManager::ResetKey:   settingsKey = QSettingsManager::ResetKey; break;
    case UioHotkeyManager::RestoreKey: settingsKey = QSettingsManager::RestoreKey; break;
    }
    mw->qsm.setValue(settingsKey, keycodes);

    QLabel *targetLabel = ui->HotkeyAssignModeDisplay;
    switch (targetHotkey) {
    case UioHotkeyManager::ToggleKey:  targetLabel = ui->ToggleTabActiveAssignmentLabel; break;
    case UioHotkeyManager::ResetKey:   targetLabel = ui->ResetTabActiveAssignmentLabel; break;
    case UioHotkeyManager::RestoreKey: targetLabel = ui->RestoreTabActiveAssignmentLabel; break;
    }
    if (targetLabel)
        targetLabel->setText(mw->uiohm.GetDisplayFromQListOfKeycodes(buffer));
}



void StopwatchInteractiveEditor::on_ToggleHotkeyRecordPushButton_clicked()
{
    auto mode = mw->uiohm.GetHotkeyAssignMode();
    mw->uiohm.SetHotkeyReassignMode(!mode);
    ui->ToggleHotkeyRecordPushButton->setText(mode ? "Record" : "Disable");

    if (!mode)
        mw->uiohm.ClearHotkeyAssignState();

    ui->ToggleHotkeyRecordPushButton->repaint();
    ui->HotkeyAssignModeDisplay->setFocus();

    // if (mw->uiohm.IsHotkeyAvailable(ui->ToggleTabHotkeyInput->keySequence().toString(),true)) {
    //     QString hotkeyName = ui->ToggleTabHotkeyInput->keySequence().toString();
    //     mw->uiohm.AssignHotkey(UioHotkeyManager::AvailableHotkeys::ToggleKey,hotkeyName);
    //     ui->ToggleTabActiveAssignmentLabel->setText(hotkeyName);
    //     mw->qsm.setValue(QSettingsManager::ToggleKey,hotkeyName);
    // }
}
