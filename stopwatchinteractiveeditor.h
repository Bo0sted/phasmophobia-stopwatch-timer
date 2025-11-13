#ifndef STOPWATCHINTERACTIVEEDITOR_H
#define STOPWATCHINTERACTIVEEDITOR_H

#include <QWidget>

class MainWindow;

#include "mainwindow.h"

namespace Ui {
class StopwatchInteractiveEditor;
}

class StopwatchInteractiveEditor : public QWidget
{
    Q_OBJECT

public:
    explicit StopwatchInteractiveEditor(QWidget *parent = nullptr, MainWindow *mwr=nullptr);
    ~StopwatchInteractiveEditor();

    void UpdateSystemModuleTogglePushButton();
    void UpdateStopwatchModuleTogglePushButton();
    void RefreshOpenState();
    void RefreshUptimeLabel();
    void SetBorderOptionsVisible(bool visible);
    void SetBackgroundOptionsEnabled(bool enabled);
    void RefreshToggleHotkeyPushButton();
    void RefreshToggleHotkeyAssignModeDisplay();
    int GetActiveTabFromHotkeyGroup();
signals:
    void toggleModuleSignal(bool shouldEnable);
    void toggleStopwatchSignal(bool shouldEnable);
public slots:
    void setEditorOpen(bool shouldOpen);
    void refreshHotkeyDisplays();
private slots:
    void on_FontPickerCombo_currentFontChanged(const QFont &f);

    void on_FontPickerResetButton_clicked();

    void on_closeWindow_clicked();

    void on_quitStopwatch_clicked();

    void on_FontPickerComboClock_currentFontChanged(const QFont &f);

    void on_FontSyncWithStopwatchPushbutton_clicked();

    void on_ToggleSystemModulePushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_PausedColorResetPushButton_clicked();

    void on_primaryColorPickerPushButton_pressed();

    void on_pausedColorPickerPushButton_pressed();

    void on_ResetColorPickerPushButton_pressed();

    void on_ResetColorResetPushButton_clicked();


    void on_pushButton_2_pressed();


    void on_backgroundColorPickerPushButton_pressed();

    void backgroundColorResetPushButton();

    void on_borderColorReseButton_clicked();

    void on_borderColorPickerPushButton_pressed();

    void on_backgroundToggleCheckbox_checkStateChanged(const Qt::CheckState &arg1);


    void on_AboutProgramPushButton_clicked();


    void on_rainbowColorComboBox_currentIndexChanged(int index);



    void on_formatTimeComboBox_currentIndexChanged(int index);

    void on_backgroundColorResetPushButton_clicked();


    void on_AssignToggleHotkeyPushButton_clicked();

    void on_ToggleHotkeyRecordPushButton_clicked();

    void on_ToggleStopwatchPushButton_clicked();

    void on_gradientOneColorPickerPushButton_pressed();

    void on_gradientTwoColorPickerPushButton_pressed();

    void on_gradientToggleCheckbox_clicked(bool checked);

    void on_exitProgramButton_clicked();

private:
    Ui::StopwatchInteractiveEditor *ui;
    MainWindow *mw;
    QPointF oldPosition;
    bool open;
    bool readyForUserUIchanges = false;

    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;
    bool AreAllModulesDisabled();

};

#endif // STOPWATCHINTERACTIVEEDITOR_H
