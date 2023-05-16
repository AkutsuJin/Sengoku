#ifndef DRAWENUM_H
#define DRAWENUM_H

#include <QObject>
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QColor>
#include <QRect>
#include <QPoint>
#include <QPixmap>
#include <QPainter>


enum ESHAPETYPE
{
    TYPE_PENCIL, //> 曲线
    TYPE_DRAWER,  //> 直线
    TYPE_ARROW,     //> 箭头;
    TYPE_SELECTION,    //> 矩形;
    TYPE_RECTANGLE,     //> 填充矩形;
    TYPE_CIRCLE,  //> 椭圆;
    TYPE_BLUR,  //> 带马赛克的矩形
    TYPE_MARKER,  //> 带遮罩的直线；
    TYPE_TEXT,
    TYPE_ERASER,
};




struct CaptureContext {
    // screenshot with modifications
    QPixmap screenshot;
    // unmodified screenshot
    QPixmap origScreenshot;
    // Selection area
    QRect selection;
    // Widget dimensions
    QRect widgetDimensions;
    // Selected tool color
    QColor color;
    // Path where the content has to be saved
    QString savePath;
    // Ofset of the capture widget based on the system's screen (top-left)
    QPoint widgetOffset;
    // Mouse position inside the widget
    QPoint mousePos;
    // Value of the desired thickness
    int thickness;
    // Mode of the capture widget
    bool fullscreen;
};


static inline qreal getColorLuma(const QColor &c)
{
    return 0.30 * c.redF() + 0.59 * c.greenF() + 0.11 * c.blueF();
}

static bool colorIsDark(const QColor &c)
{
    bool isWhite = false;
    if (getColorLuma(c) <= 0.60) {
        isWhite = true;
    }
    return isWhite;

}

static QColor contrastColor(const QColor &c)
{
    int change = colorIsDark(c) ? 30 : -45;

    return QColor(qBound(0, c.red() + change, 255),
                  qBound(0, c.green() + change, 255),
                  qBound(0, c.blue() + change, 255));

}


static const QString whiteIconPath()
{
    return ":/img/material/white/";
}

static const QString blackIconPath()
{
    return ":/img/material/black/";
}

static QStringList translationsPaths()
{
    QString binaryPath = QFileInfo(qApp->applicationDirPath())
            .absoluteFilePath();
    QString trPath = QDir::toNativeSeparators(binaryPath + "/translations") ;
#if defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
    return QStringList()
            << QString(APP_PREFIX) + "/share/flameshot/translations"
            << trPath
            << "/usr/share/flameshot/translations"
            << "/usr/local/share/flameshot/translations";
#elif defined(Q_OS_WIN)
    return QStringList()
            << trPath;
#endif

}



#endif // DRAWENUM_H
