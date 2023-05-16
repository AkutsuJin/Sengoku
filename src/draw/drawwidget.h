#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QUndoStack>
#include <QWidget>
#include <QUndoCommand>


class ToolFactory;
class CaptureTool;
class CaptureContext;


class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = nullptr);

    void setCanvas(QPixmap pixmap);

    int getShape() const;

    QColor getColor() const ;

    int getThickness() const;

    void clearDraw();

    bool save(const QString& path);
signals:
    void colorChanged(const QColor &c);
    void thicknessChanged(const int thickness);

public slots:
    void setShape(int shape);

    void undo();
    void redo();

    void setColor(QColor color);
    void setThickness(int value);


protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *e);

    void mouseMoveEvent(QMouseEvent *e);

    void mouseReleaseEvent(QMouseEvent *e);


    void pushToolToStack();
private:
    void initContext();


private:
    int m_type{0};
    ToolFactory* m_toolFactory{nullptr};
    CaptureTool* m_activeTool{nullptr};
    bool m_mouseIsClicked;

    QUndoStack m_undoStack;

    CaptureContext* m_context;

    QPixmap m_pixmap;

};


#endif // DRAWWIDGET_H
