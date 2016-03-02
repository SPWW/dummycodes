#include "videowidget.h"
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>

VideoWidget::VideoWidget(QWidget *parent):QWidget(parent),_media(0),_videowidget(0)
{
    _instance = new VlcInstance(VlcCommon::args(), parent);
    QString url = "udp/h264://@147.8.179.120:6666";
    _media = new VlcMedia(url, _instance);
    _player = new VlcMediaPlayer(_instance);
    _videowidget = new VlcWidgetVideo(_player,this);
    _player->setVideoWidget(_videowidget);
    _videowidget->show();
    _player->open(_media);
    _player->play();

    //setCentralWidget(_videowidget);
    //setSizeIncrement(500,500);

    layout = new QVBoxLayout;
    layout->addWidget(_videowidget);
    this->setLayout(layout);
}



void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(isFullScreen()){
        QPoint pos = event->pos();
        qDebug() << pos.x() << " " << pos.y();
    }else{
        showFullScreen();
        this->parentWidget()->showFullScreen();
    }
    event->accept();
}

void VideoWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && isFullScreen()) {
        showNormal();
        event->accept();
    } else if (event->key() == Qt::Key_Enter && event->modifiers() & Qt::Key_Alt) {
        showFullScreen();

        event->accept();
    } else {

    }
}
