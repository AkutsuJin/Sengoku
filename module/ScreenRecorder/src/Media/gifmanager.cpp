#include "gifmanager.h"

#include <QApplication>
#include <QImage>
#include <QPixmap>
#include <QScreen>
#include <QDebug>



GIFManager::GIFManager(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(100);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(saveImage()));

}

GIFManager::~GIFManager()
{
    stopRecord();
}

void GIFManager::addCaptureWindowTask(const int &id, const QString& path,const QRect &srcRect)
{
    m_path = path;
    m_srcRect = srcRect;
}

void GIFManager::startRecord()
{
    if(gifWriter != nullptr)
    {
        delete gifWriter;
        gifWriter = 0;

    }

    m_timer->stop();

    m_startDateTime = QDateTime::currentMSecsSinceEpoch();
    gifWriter = new Gif::GifWriter;
    bool bOk = gif.GifBegin(gifWriter, m_path.toLocal8Bit().data(), m_srcRect.width(), m_srcRect.height(), fps);
    if (!bOk) {
        delete gifWriter;
        gifWriter = 0;
        return;
    }

    m_timer->start(1000 / fps);
    saveImage();
}

void GIFManager::pauseRecord()
{
    m_timer->stop();

    qint64 dur = QDateTime::currentMSecsSinceEpoch() - m_startDateTime;
    m_dur += dur;
}

void GIFManager::restoreRecord()
{
    m_timer->start(1000 / fps);

    m_startDateTime = QDateTime::currentMSecsSinceEpoch();
}

void GIFManager::stopRecord()
{
    m_timer->stop();
    m_dur = 0;
    m_startDateTime = 0;
    m_dateTime = 0;

    if(gifWriter)
    {
        gif.GifEnd(gifWriter);
        delete gifWriter;
        gifWriter = 0;

    }
}

int64_t GIFManager::getVideoFileCurrentTime()
{
    return m_dateTime;
}

void GIFManager::saveImage()
{
    if (!gifWriter) {
        return;
    }

    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    m_dateTime = m_dur + currentTime - m_startDateTime;

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    //由于qt4没有RGBA8888,采用最接近RGBA8888的是ARGB32,颜色会有点偏差
    QPixmap pix = QPixmap::grabWindow(0, x() + rectGif.x(), y() + rectGif.y(), rectGif.width(), rectGif.height());
    QImage image = pix.toImage().convertToFormat(QImage::Format_ARGB32);
#else
    QScreen *screen = QApplication::primaryScreen();
    //    QPixmap pix = screen->grabWindow(0, x() + rectGif.x(), y() + rectGif.y(), rectGif.width(), rectGif.height());
    QPixmap pix = screen->grabWindow(0, m_srcRect.x(), m_srcRect.y(), m_srcRect.width(), m_srcRect.height());
    QImage image = pix.toImage().convertToFormat(QImage::Format_RGBA8888);
#endif

    gif.GifWriteFrame(gifWriter, image.bits(), m_srcRect.width(), m_srcRect.height(), fps);
}


