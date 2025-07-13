#include "stopwatchinteractiveeditor.h"
#include "ui_stopwatchinteractiveeditor.h"
#include "mainwindow.h"
#include "stylesheetgenerator.h"
#include "qsettingsmanager.h"

#include <QCloseEvent>
#include <QPushButton>



StopwatchInteractiveEditor::StopwatchInteractiveEditor(QWidget *parent, MainWindow *mwr)
    : QWidget(parent)
    , ui(new Ui::StopwatchInteractiveEditor)
    , mw{mwr}
{
    ui->setupUi(this);
    instances++;
    mw->qhm.UpdateHotkeySignalBlock(true);
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
    ui->EditorHeaderText->setStyleSheet(StylesheetGenerator::DefaultHeaderTextDejaVu());
    ui->ColorHeaderText->setStyleSheet(StylesheetGenerator::DefaultHeaderTextDejaVu());
    ui->EditorHotkeyHeaderText->setStyleSheet(StylesheetGenerator::DefaultHeaderTextDejaVu());
    ui->ToggleTabActiveAssignmentLabel->setText(mw->qhm.FetchToggleStopwatchHotkey());
    ui->ResetTabActiveAssignmentLabel->setText(mw->qhm.FetchResetStopwatchHotkey());
    ui->BringToForegroundTabActiveAssignmentLabel->setText(mw->qhm.FetchBringToForegroundHotkey());
    ui->horizontalLayout->setAlignment(ui->closeWindow, Qt::AlignRight);
    ui->quitStopwatch->setStyleSheet(StylesheetGenerator::DefaultDangerHexColor());
}

void StopwatchInteractiveEditor::closeEvent(QCloseEvent *event)
{
    mw->qsm.setValue(QSettingsManager::Font,ui->FontPickerCombo->currentText());
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

