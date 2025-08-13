#include <QPainter>
#include <QLinearGradient>
#include <QPainterPath>

#include "gradientqlabel.h"

GradientQLabel::GradientQLabel(QWidget *parent)
    : QLabel(parent), startColor(Qt::red), endColor(Qt::blue), useGradient(true)
{
}

void GradientQLabel::setGradientColors(const QColor &start, const QColor &end)
{
    startColor = start;
    endColor = end;
    update();
}

void GradientQLabel::setUseGradient(bool enable)
{
    useGradient = enable;
    update();
}

void GradientQLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    if (useGradient) {
        // Gradient spans the width of the label
        QLinearGradient gradient(0, 0, width(), 0);
        gradient.setColorAt(0, startColor);
        gradient.setColorAt(1, endColor);

        // Build path with the current font
        QFont f = font();
        f.setStyleStrategy(QFont::ForceOutline);

        QPainterPath path;
        path.addText(0, fontMetrics().ascent(), f, text());

        // Centering offset
        QRectF bounds = path.boundingRect();
        qreal x = (width() - bounds.width()) / 2.0 - bounds.left();
        qreal y = (height() - bounds.height()) / 2.0 - bounds.top();

        QTransform transform;
        transform.translate(x, y);
        path = transform.map(path);

        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawPath(path);
    } else {
        // Normal text rendering
        painter.setPen(palette().color(foregroundRole()));
        painter.setFont(font());
        setAlignment(Qt::AlignCenter);
        painter.drawText(rect(), alignment(), text());
    }
}
