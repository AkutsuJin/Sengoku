#include "drawwidget.h"
#include <QMouseEvent>
#include <QScreen>
#include "line/linetool.h"
#include "modificationcommand.h"
#include "toolfactory.h"
#include "capturetool.h"
#include <QDebug>


DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);



    m_toolFactory = new ToolFactory(this);
    m_context = new CaptureContext;

    initContext();

}

void DrawWidget::setCanvas(QPixmap pixmap)
{
    m_pixmap = pixmap;
    m_context->screenshot = pixmap;
    this->resize(pixmap.size());

}

void DrawWidget::initContext()
{
    m_context->widgetDimensions = rect();
    m_context->color = Qt::red;
    m_context->widgetOffset = mapToGlobal(QPoint(0,0));
    m_context->mousePos= mapFromGlobal(QCursor::pos());
    m_context->thickness = 2;
    m_context->fullscreen = true;

}



void DrawWidget::setShape(int shape)
{
    m_type = shape;
}

int DrawWidget::getShape() const
{
    return m_type;
}

void DrawWidget::setColor(QColor color)
{
    m_context->color = color;

    emit colorChanged(color);
}

QColor DrawWidget::getColor() const
{
    return m_context->color;
}

void DrawWidget::setThickness(int value)
{
    m_context->thickness = value;

    emit thicknessChanged(value);

}

int DrawWidget::getThickness() const
{
    return m_context->thickness;
}

void DrawWidget::clearDraw()
{
    m_context->screenshot = m_pixmap;
    m_undoStack.clear();
    this->update();
}

bool DrawWidget::save(const QString &path)
{
    return m_context->screenshot.save(path);
}




void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, m_context->screenshot);


    if (m_activeTool && m_mouseIsClicked) {
        painter.save();
        m_activeTool->process(painter, m_context->screenshot);
        painter.restore();
    }


    QWidget::paintEvent(event);
}

void DrawWidget::mousePressEvent(QMouseEvent *e)
{
    m_mouseIsClicked = true;
    m_context->mousePos = e->pos();

    if (m_activeTool)
    {
        if (m_activeTool->isValid())
        {
            pushToolToStack();
        }
        else
        {
            m_activeTool->deleteLater();
            m_activeTool = nullptr;
        }
    }

    m_activeTool = m_toolFactory->CreateTool(ESHAPETYPE(m_type),this);

    connect(this, &DrawWidget::colorChanged,
            m_activeTool, &CaptureTool::colorChanged);
    connect(this, &DrawWidget::thicknessChanged,
            m_activeTool, &CaptureTool::thicknessChanged);
    m_activeTool->drawStart(*m_context);

}

void DrawWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(m_mouseIsClicked)
    {
        m_activeTool->drawMove(e->pos());

        update();
    }

}

void DrawWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_activeTool->drawEnd(m_context->mousePos);
    if (m_activeTool->isValid())
    {
        pushToolToStack();

    }

    m_mouseIsClicked = false;
    update();

}


void DrawWidget::undo()
{
    m_undoStack.undo();
    update();
}

void DrawWidget::redo()
{
    m_undoStack.redo();
    update();
}

void DrawWidget::pushToolToStack() {
    auto mod = new ModificationCommand(
                &m_context->screenshot, m_activeTool);
    disconnect(this, &DrawWidget::colorChanged,
               m_activeTool, &CaptureTool::colorChanged);
    disconnect(this, &DrawWidget::thicknessChanged,
               m_activeTool, &CaptureTool::thicknessChanged);
    m_undoStack.push(mod);
    m_activeTool = nullptr;
}




