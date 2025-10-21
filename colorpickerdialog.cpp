#include "colorpickerdialog.h"
#include "ui_colorpickerdialog.h"
#include "stylesheetgenerator.h"

#include <QMessageBox>
#include <QClipboard>

ColorPickerDialog::ColorPickerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ColorPickerDialog)
    , hex{}
{
    ui->setupUi(this);
    connect(ui->DiscardChanges, &QPushButton::clicked, this, &QDialog::reject);
}

ColorPickerDialog::~ColorPickerDialog()
{
    delete ui;
}

QColor ColorPickerDialog::FetchColorSelection()
{
    return QColor(hex);
}

bool ColorPickerDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusIn) {
        QGraphicsView* view = qobject_cast<QGraphicsView*>(obj);
        if (view && view->property("hexColor").isValid()) {
            QString hex = view->property("hexColor").toString();
            ui->hexColorInput->setText(hex);
        }
    }
    return QWidget::eventFilter(obj, event);
}

void ColorPickerDialog::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    int colorIndex = 0;

    for (int i = 0; i < ui->horizontalLayout_7->count(); ++i) {
        QLayoutItem* vItem = ui->horizontalLayout_7->itemAt(i);
        QVBoxLayout* vLayout = qobject_cast<QVBoxLayout*>(vItem->layout());

        if (vLayout) {
            for (int j = 0; j < vLayout->count(); ++j) {
                QLayoutItem* widgetItem = vLayout->itemAt(j);
                QWidget* widget = widgetItem->widget();

                QGraphicsView* view = qobject_cast<QGraphicsView*>(widget);
                if (view) {
                    // Get color from NiceColors at colorIndex
                    QString color = StylesheetGenerator::NiceColors[colorIndex].first;
                    view->setStyleSheet(QString("QGraphicsView { background: %1; }").arg(color));

                    QLabel* label = new QLabel(view);
                    label->setText(StylesheetGenerator::NiceColors[colorIndex].second);
                    label->setStyleSheet("color: #F5F5F5; background: #000000;");

                    QFont font = label->font();
                    font.setPointSize(8);
                    font.setBold(true);
                    label->setFont(font);

                    // Position label at top-left inside the view
                    label->move(5, 5);  // adjust as needed
                    label->show();

                    view->setFocusPolicy(Qt::StrongFocus); // Enable focus by click

                    // Store color using setProperty
                    view->setProperty("hexColor", color);
                    view->setCursor(QCursor(Qt::PointingHandCursor));

                    // Install event filter
                    view->installEventFilter(this);

                    ++colorIndex;  // increment index for next view
                }
            }
        }
    }
}

bool ColorPickerDialog::validateHex(QString hex)
{
    if (hex.isEmpty()) return false;

    QColor color(hex);
    return color.isValid();
}


void ColorPickerDialog::on_SaveChanges_clicked()
{
    if (hex.isEmpty()){
        QMessageBox::warning(this, "Warning", "No valid hex color detected. Please try again or make a selection from the predefined colors.");
        return;
    }

    this->done(QDialog::Accepted);

}


void ColorPickerDialog::on_hexColorInput_textChanged(const QString &arg1)
{
    if (validateHex(arg1))
        hex = arg1;
     else
        hex.clear();

}


void ColorPickerDialog::on_pushButton_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    ui->hexColorInput->setText(clipboard->text());

}

