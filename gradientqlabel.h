#ifndef GRADIENTQLABEL_H
#define GRADIENTQLABEL_H

#include <QLabel>
#include <QColor>

class GradientQLabel : public QLabel
{
    Q_OBJECT

public:
    explicit GradientQLabel(QWidget *parent = nullptr);

    void setGradientColors(const QColor &start, const QColor &end);
    void setUseGradient(bool enable);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor startColor;
    QColor endColor;
    bool useGradient;
};

#endif // GRADIENTQLABEL_H
