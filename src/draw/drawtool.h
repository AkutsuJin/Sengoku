#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include "colorpicker.h"
class ToolButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ToolButton(int id,QString url,QWidget *parent = nullptr) : QPushButton(parent),
        m_id(id)
    {
        m_iconSize = QSize(30,30);
        m_ellSize = QSize(35,35);

        setPixmalUrl(url);
    }

    int getId() const
    {
        return m_id;
    }


    void setColor(QColor color)
    {
        m_color = color;
        update();
    }

    void setPixmalUrl(const QString& url)
    {
        m_pixmap = QPixmap(url).scaled(30,30,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }

    void setPressPixmalUrl(const QString& url)
    {
        m_presspixmap = QPixmap(url).scaled(30,30,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }
protected:
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing);
        painter.setBrush(QColor(150,150,150,200));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(rect());

        if(m_id == 4)
        {
            painter.setBrush(m_color);
            painter.drawEllipse((rect().width() - m_ellSize.width()) / 2,
                                (rect().height() - m_ellSize.height()) / 2,m_ellSize.width(),m_ellSize.height());

        }
        else
        {

            if(isChecked() == false || m_presspixmap.isNull())
            {
                painter.drawPixmap((rect().width() - m_iconSize.width()) / 2,
                                   (rect().height() - m_iconSize.height()) / 2,m_pixmap);
            }
            else
            {
                painter.drawPixmap((rect().width() - m_iconSize.width()) / 2,
                                   (rect().height() - m_iconSize.height()) / 2,m_presspixmap);
            }

        }

    }

private:
    int m_id;
    QPixmap m_pixmap;
    QPixmap m_presspixmap;

    QColor m_color{Qt::red};

    QSize m_ellSize;
    QSize m_iconSize;
};

class DrawTool : public QWidget
{
    Q_OBJECT
public:
    explicit DrawTool(int x, int y, int w, int h, QWidget *parent = nullptr);

    bool isWhiteBoard();
signals:
    void sigSelectColor(QColor color);

    void sigSelectShape(int value);


    void sigTool(int value,bool ischecked);

public slots:
    void sltSender(bool flag);

    void sltColor(int value);

    void sltShape(int value);

private:
    QVector<ToolButton*> m_Vector;

    ColorPicker* m_Picker;
    ColorPicker* m_Shape;
};

#endif // DRAWTOOL_H
