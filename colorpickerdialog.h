#ifndef COLORPICKERDIALOG_H
#define COLORPICKERDIALOG_H

#include <QDialog>

namespace Ui {
class ColorPickerDialog;
}

class ColorPickerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorPickerDialog(QWidget *parent = nullptr);
    ~ColorPickerDialog();
    QColor FetchColorSelection();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void on_SaveChanges_clicked();

    void on_hexColorInput_textChanged(const QString &arg1);


private:
    Ui::ColorPickerDialog *ui;
    void showEvent(QShowEvent *event) override;
    QString hex;

    bool validateHex(QString hex);
};

#endif // COLORPICKERDIALOG_H
