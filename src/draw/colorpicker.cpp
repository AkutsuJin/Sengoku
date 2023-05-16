
#include "colorpicker.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>




ColorPicker::ColorPicker(bool flag,QWidget *parent) : QWidget(parent),
    m_flag(flag)
{
    m_colorList = getUserColors();
    m_colorAreaSize = 35;
    setMouseTracking(true);

    m_uiColor = QColor(116, 0, 150);
    m_drawColor = Qt::red;

    const int extraSize = 6;
    double radius = (m_colorList.size() * m_colorAreaSize / 1.6) / (3.141592);
    resize(radius*2 + m_colorAreaSize + extraSize,
           radius*2 + m_colorAreaSize+ extraSize);
    double degree = 360 / (m_colorList.size());
    double degreeAcum = degree;

    QLineF baseLine = QLineF(QPoint(radius+extraSize/2, radius+extraSize/2),
                             QPoint(radius*2, radius));

    for (int i = 0; i<m_colorList.size(); ++i)
    {
        m_colorAreaList.append(QRect(baseLine.x2(), baseLine.y2(),
                                     m_colorAreaSize, m_colorAreaSize));
        baseLine.setAngle(degreeAcum);
        degreeAcum += degree;
    }
}

QColor ColorPicker::drawColor()
{
    return m_drawColor;
}


QVector<QColor> ColorPicker::getUserColors()
{
    QVector<QColor> colors;
    const QVector<QColor> &defaultColors = {
        Qt::darkRed,
        Qt::red,
        Qt::yellow,
        Qt::green,
        Qt::darkGreen,
        Qt::cyan,
        Qt::blue
//        Qt::magenta
//        Qt::darkMagenta
    };

    colors = defaultColors;


    return colors;
}


void ColorPicker::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QVector<QRect> rects = handleMask();
    painter.setPen(Qt::NoPen);
    for (int i = 0; i < rects.size(); ++i)
    {
        if (m_drawColor == QColor(m_colorList.at(i)))
        {
            QColor c = QColor(m_uiColor);
            c.setAlpha(155);
            painter.setBrush(c);
            c.setAlpha(100);
            painter.setPen(c);
            QRect highlight = rects.at(i);
            highlight.moveTo(highlight.x() - 3, highlight.y() - 3);
            highlight.setHeight(highlight.height() + 6);
            highlight.setWidth(highlight.width() + 6);
            painter.drawRoundRect(highlight, 100, 100);
            painter.setPen(Qt::NoPen);
        }

        if(m_flag)
        {
            painter.setBrush(QColor(m_colorList.at(i)));
            painter.drawRoundRect(rects.at(i), 100, 100);

        }
        else
        {
            painter.setBrush(QColor(249, 118, 128,180));
            painter.drawRoundRect(rects.at(i), 100, 100);

            painter.drawPixmap(rects.at(i).x() + 8,rects.at(i).y() + 8,QPixmap(QString(":/image/shape_%1.png").arg(i))
                               .scaled(20,20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

        }

    }
}



void ColorPicker::mouseReleaseEvent(QMouseEvent *e)
{
    for (int i = 0; i < m_colorList.size(); ++i) {
        if (m_colorAreaList.at(i).contains(e->pos())) {
            m_drawColor = m_colorList.at(i);
            emit sigSelected(i);
            update();
            break;
        }
    }

}

QVector<QRect> ColorPicker::handleMask() const {
    QVector<QRect> areas;
    for (const QRect &rect: m_colorAreaList) {
        areas.append(rect);
    }
    return areas;
}
