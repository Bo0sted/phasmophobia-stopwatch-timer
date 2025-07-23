#ifndef WARMWELCOME_H
#define WARMWELCOME_H

#include "mainwindow.h"

#include <QDialog>

namespace Ui {
class WarmWelcome;
}

class WarmWelcome : public QDialog
{
    Q_OBJECT

public:
    explicit WarmWelcome(MainWindow *mwr,QWidget *parent = nullptr);
    ~WarmWelcome();
    void RefreshVersion();

private:
    Ui::WarmWelcome *ui;
    QPointF oldPosition;
    MainWindow *mw;
protected:
    // void closeEvent(QCloseEvent *event) override;
    // void showEvent(QShowEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;
private slots:
    void on_refreshLatestVersionPushButton_clicked();
};

#endif // WARMWELCOME_H
