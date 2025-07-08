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
    inline static int instances = 0;

private slots:
    void on_FontPickerCombo_currentFontChanged(const QFont &f);

    void on_FontPickerResetButton_clicked();

    void on_ToggleTabApplyNewHotkey_clicked();

    void on_ResetTabApplyNewHotkey_clicked();

    void on_BringToForegroundTabApplyNewHotkey_clicked();

private:
    Ui::StopwatchInteractiveEditor *ui;
    MainWindow *mw;

    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

};

#endif // STOPWATCHINTERACTIVEEDITOR_H
