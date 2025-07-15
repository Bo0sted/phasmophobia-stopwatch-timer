#ifndef SYSTEMTIMEMODULE_H
#define SYSTEMTIMEMODULE_H

#include "mainwindow.h"

#include <QWidget>

namespace Ui {
class SystemTimeModule;
}

class SystemTimeModule : public QWidget
{
    Q_OBJECT

public:
    explicit SystemTimeModule(QWidget *parent = nullptr, MainWindow *mwr=nullptr);
    ~SystemTimeModule();
    void RefreshClockThread();
    void UpdateClockFont(QString fontName, int fontSize);
    void UpdateClockFontColor(QColor color);
    QFont GetCurrentFont();
    bool CheckIfModuleIsEnabled();
    void RefreshModuleState();
signals:
    void signalRefreshClock(const QString &time); // Signal to update elapsed time
public slots:
    void updateClock(const QString &time);
    void setLoadModule(bool shouldEnable); // Declare the slot
private:
    Ui::SystemTimeModule *ui;
    MainWindow *mw;
    QPointF oldPosition;
    bool isDeconstructing;
    QFuture<void> refreshClockThread;
    bool enabled;
    QColor systemClockFontColor;

    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void ResizeClockToFitWindow();
};

#endif // SYSTEMTIMEMODULE_H
