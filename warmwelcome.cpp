#include "warmwelcome.h"
#include "ui_warmwelcome.h"
#include "stylesheetgenerator.h"
#include "uiokey.h"

#include <QMouseEvent>
#include <QCursor>

WarmWelcome::WarmWelcome(MainWindow *mwr, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WarmWelcome)
    , mw(mwr)
{
    ui->setupUi(this);
}

WarmWelcome::~WarmWelcome()
{
    delete ui;
}

// void WarmWelcome::closeEvent(QCloseEvent *event)
// {
// }

// void WarmWelcome::showEvent(QShowEvent *event)
// {


// }
void WarmWelcome::mouseMoveEvent(QMouseEvent *event)
{
    // This wont be needed since mouse will keep its border
    // if (!event->buttons().testFlag(Qt::MouseButton::RightButton)) {
    //     const QPointF delta = event->globalPosition() - oldPosition;
    //     move(x()+delta.x(), y()+delta.y());
    //     oldPosition = event->globalPosition();
    // }
}

bool WarmWelcome::event(QEvent *event)
{
    if (event->type() == QEvent::Polish) {
        QPixmap pixmap(":/assets/512x512/stopwatchpicture.png");
        QPixmap githubpxmap(":/assets/github.png");
        QPixmap scaledPixmap = pixmap.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap githubScaledPxmap = githubpxmap.scaled(64, 64, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        ui->iconViewLabel->setPixmap(scaledPixmap);
        ui->githubIconViewLabel->setPixmap(githubScaledPxmap);
        ui->githubIconViewLabel->setCursor(QCursor(Qt::PointingHandCursor));
        ui->programTitleLabel->setStyleSheet(StylesheetGenerator::DefaultHeader());
        ui->programTitleLabel->setText("Cross Platform Timer");
        ui->sourceCodeHeaderLabel->setText("View source code:");
        ui->aboutSubtextLabel->setStyleSheet(StylesheetGenerator::DefaultLabel());
        ui->refreshLatestVersionPushButton->setStyleSheet(StylesheetGenerator::DefaultButtonStyle(12));
        ui->aboutSubtextLabel->setText(QString("Developed by Bo0sted<br>"
                                               "Start/Stop Hotkey: %1<br>"
                                               "Reset Hotkey: %2<br>"
                                               ).arg(mw->uiohm.GetDisplayFromQListOfKeycodes(mw->uiohm.FetchToggleStopwatchHotkey()))
                                           .arg(mw->uiohm.GetDisplayFromQListOfKeycodes(mw->uiohm.FetchResetStopwatchHotkey())));

        RefreshVersion();

    }


    return QDialog::event(event);

}

void WarmWelcome::RefreshVersion() {
    ui->versionCheck->setText(QString("Current version: %1<br>Latest version: %2").arg(QCoreApplication::applicationVersion(), mw->FetchLatestVersion()));
}

void WarmWelcome::on_refreshLatestVersionPushButton_clicked()
{
    mw->um.CheckForUpdateAndPromptUser([this]() {
        RefreshVersion();
    });
    ui->refreshLatestVersionPushButton->setVisible(false);
}

