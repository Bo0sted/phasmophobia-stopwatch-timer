#ifndef STOPWATCHINTERACTIVEEDITOR_H
#define STOPWATCHINTERACTIVEEDITOR_H

#include <QWidget>

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
    void RefreshOpenState();
signals:
    void toggleModuleSignal(bool shouldEnable);
public slots:
    void setEditorOpen(bool shouldOpen);
private slots:
    void on_FontPickerCombo_currentFontChanged(const QFont &f);

    void on_FontPickerResetButton_clicked();

    void on_ToggleTabApplyNewHotkey_clicked();

    void on_ResetTabApplyNewHotkey_clicked();

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

private:
    Ui::StopwatchInteractiveEditor *ui;
    MainWindow *mw;
    QPointF oldPosition;
    bool open;

    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;

};

#endif // STOPWATCHINTERACTIVEEDITOR_H
