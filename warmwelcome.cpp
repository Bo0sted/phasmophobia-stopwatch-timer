#include "warmwelcome.h"
#include "ui_warmwelcome.h"

#include <QMouseEvent>

WarmWelcome::WarmWelcome(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WarmWelcome)
{
    ui->setupUi(this);
}

WarmWelcome::~WarmWelcome()
{
    delete ui;
}

void WarmWelcome::closeEvent(QCloseEvent *event)
{

}

void WarmWelcome::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::MouseButton::RightButton)) {
        const QPointF delta = event->globalPosition() - oldPosition;
        move(x()+delta.x(), y()+delta.y());
        oldPosition = event->globalPosition();
    }
}

bool WarmWelcome::event(QEvent *event)
{

}
