#ifndef GIFMANAGER_H
#define GIFMANAGER_H

#include <QDateTime>
#include <QObject>
#include <QRect>
#include <QTimer>
#include "Video/gif.h"

/**
 * @brief The GIFManager class      处理GIF的操作类
 */
class GIFManager : public QObject
{
    Q_OBJECT
public:
    explicit GIFManager(QObject *parent = nullptr);
    ~GIFManager();


    void addCaptureWindowTask(const int &id, const QString &path,
                              const QRect &srcRect);

    void startRecord();

    void pauseRecord();

    void restoreRecord();


    void stopRecord();



    int64_t getVideoFileCurrentTime(); //获取录屏的时间戳(毫秒)

signals:


private slots:
    void saveImage();


private:
    qint64 m_startDateTime{0};
    qint64 m_dateTime{0};
    qint64 m_dur{0};

    QTimer* m_timer;

    Gif gif;                    //gif类对象
    Gif::GifWriter *gifWriter{nullptr};  //gif写入对象

    QString m_path;
    QRect m_srcRect;
    int fps{10};
};

#endif // GIFMANAGER_H
