#ifndef WARMWELCOME_H
#define WARMWELCOME_H

#include <QDialog>

namespace Ui {
class WarmWelcome;
}

class WarmWelcome : public QDialog
{
    Q_OBJECT

public:
    explicit WarmWelcome(QWidget *parent = nullptr);
    ~WarmWelcome();

private:
    Ui::WarmWelcome *ui;
    QPointF oldPosition;

    void closeEvent(QCloseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;
};

#endif // WARMWELCOME_H
