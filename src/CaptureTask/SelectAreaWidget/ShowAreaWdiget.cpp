﻿/**
 * 叶海辉
 * QQ群121376426
 * http://blog.yundiantech.com/
 */

#include "ShowAreaWdiget.h"
#include "ui_ShowAreaWdiget.h"

#include <QDesktopWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QDebug>

ShowAreaWdiget::ShowAreaWdiget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowAreaWdiget)
{
    ui->setupUi(this);

//    ///定时器用于定制检测鼠标位置，防止鼠标快速移入窗口，没有检测到，导致鼠标箭头呈现拖拉的形状
//    mTimer = new QTimer;
//    mTimer->setInterval(1000);
//    connect(mTimer, &QTimer::timeout, this, &ShowAreaWdiget::slotTimerTimeOut);
//    mTimer->start();

///改变窗体大小相关
    isMax = false;

    int w = this->width();
    int h = this->height();

    QRect screenRect = QApplication::desktop()->screenGeometry();//获取设备屏幕大小
    int x = (screenRect.width() - w) / 2;
    int y = (screenRect.height() - h) / 2;

    mLocation = this->geometry();
//    mLocation = QRect(x, y, w, h);
//    this->setGeometry(mLocation);

    isLeftPressDown = false;
    this->dir = NONE;
    this->setMouseTracking(true);// 追踪鼠标
    ui->widget_frame->setMouseTracking(true);
//    ui->widget_center->setMouseTracking(true);

    this->setFocusPolicy(Qt::ClickFocus);

}

ShowAreaWdiget::~ShowAreaWdiget()
{

}

////////////改变窗体大小相关

void ShowAreaWdiget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
//        qDebug()<<__FUNCTION__;
        isLeftPressDown = false;
        if(dir != NONE)
        {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void ShowAreaWdiget::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<__FUNCTION__;
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        if (event->button() == Qt::LeftButton)
        {
//            if(QApplication::keyboardModifiers() == (Qt::ControlModifier|Qt::ShiftModifier|Qt::AltModifier))
//            {
//                doChangeFullScreen(); //ctrl + 左键
//                doChangeMaxSize();
//            }
        }
    }

    switch(event->button())
    {
    case Qt::LeftButton:
        if (isMax || this->isFullScreen()) break;
        isLeftPressDown = true;
        checkCursorDirect(event->globalPos());

        if(dir != NONE)
        {
            this->mouseGrabber();
            mIsResizeMode = true;
        }
        else
        {
            dragPosition  = event->globalPos() - this->frameGeometry().topLeft();
            mIsResizeMode = false;
        }
        break;
//    case Qt::RightButton:
//        if (!this->isFullScreen())
//            mAction_FullScreen->setText(tr("show fullscreen"));
//        else
//            mAction_FullScreen->setText(tr("quit fullscreen"));
//        mPopMenu->exec(QCursor::pos());
//        break;
    default:
        QWidget::mousePressEvent(event);
    }

}

void ShowAreaWdiget::mouseDoubleClickEvent(QMouseEvent *event)
{
//qDebug()<<__FUNCTION__;
    emit sig_DoubleClicked();
}

void ShowAreaWdiget::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<__FUNCTION__<<isLeftPressDown;

    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    if (isMax || this->isFullScreen()) return;
    if (!isLeftPressDown)
    {
        checkCursorDirect(gloPoint);
        return;
    }

//    if(!isLeftPressDown)
//    {
//        checkCursorDirect(gloPoint);
//    }
//    else
    {

//        if(dir != NONE)
        if (mIsResizeMode)
        {
            QRect rMove(tl, rb);

            switch(dir) {
            case LEFT:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                break;
            case RIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                break;
            case UP:
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case DOWN:
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case LEFTTOP:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case RIGHTTOP:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setY(gloPoint.y());
                break;
            case LEFTBOTTOM:
                rMove.setX(gloPoint.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case RIGHTBOTTOM:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            default:
                break;
            }
            this->setGeometry(rMove);
            emit sig_WindowMoved(rMove);
        } else {
            checkCursorDirect(event->globalPos());

            if (dir == NONE && !isMax)
            {
                QPoint point = event->globalPos() - dragPosition;

                QRect mLimitRect = QApplication::desktop()->availableGeometry();

                if (point.x() < mLimitRect.x())
                    point.setX(mLimitRect.x());

                if (point.x() > (mLimitRect.x()+mLimitRect.width()-this->width()))
                    point.setX(mLimitRect.x()+mLimitRect.width()-this->width());


                if (point.y() < mLimitRect.y())
                    point.setY(mLimitRect.y());

                if (point.y() > (mLimitRect.y()+mLimitRect.height()-this->height()))
                    point.setY(mLimitRect.y()+mLimitRect.height()-this->height());

                move(point);

                emit sig_WindowMoved(QRect(point.x(), point.y(), this->width(), this->height()));
            }

            event->accept();
        }



    }
//    QWidget::mouseMoveEvent(event);、
    event->accept();
}

void ShowAreaWdiget::checkCursorDirect(const QPoint &cursorGlobalPoint)
{
    // 获取窗体在屏幕上的位置区域，tl为topleft点，rb为rightbottom点
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
        // 左上角
        dir = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
    } else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        // 右下角
        dir = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        //左下角
        dir = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
        // 右上角
        dir = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x()) {
        // 左边
        dir = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - PADDING) {
        // 右边
        dir = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= tl.y() && y <= tl.y() + PADDING){
        // 上边
        dir = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - PADDING) {
        // 下边
        dir = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else {
        // 默认
        dir = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void ShowAreaWdiget::slotTimerTimeOut()
{
    if (QObject::sender() == mTimer)
    {
        if (!isLeftPressDown)
            checkCursorDirect(QCursor::pos());
    }
}
