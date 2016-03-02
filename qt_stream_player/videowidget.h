#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H


#include <QtWidgets>
#include "VLCQtCore/Common.h"
#include "VLCQtCore/Media.h"
#include "VLCQtCore/MediaListPlayer.h"
#include "VLCQtCore/MediaPlayer.h"
#include "VLCQtCore/Instance.h"
#include "VLCQtWidgets/WidgetVideo.h"

class VideoWidget:public QWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget *parent = 0);

protected:
    QBoxLayout* layout;
    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;
    VlcWidgetVideo *_videowidget;

    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    //void mousePressEvent(QMouseEvent *event);
};




#endif // VIDEOWIDGET_H
