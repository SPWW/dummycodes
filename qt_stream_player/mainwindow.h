#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "VLCQtCore/Common.h"
#include "VLCQtCore/Media.h"
#include "VLCQtCore/MediaListPlayer.h"
#include "VLCQtCore/MediaPlayer.h"
#include "VLCQtCore/Instance.h"
#include "VLCQtWidgets/WidgetVideo.h"

#include "videowidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openLocal();
    void openUrl();
    void keyPressEvent(QKeyEvent *event);

private:

    VideoWidget *_videowidget;

};

#endif // MAINWINDOW_H
