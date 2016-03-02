#include "mainwindow.h"
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),_media(0),_videowidget(0)
{

      _videowidget = new VideoWidget(this);
      setCentralWidget(_videowidget);


}

MainWindow::~MainWindow()
{

}

void MainWindow::openLocal()
{

}

void MainWindow::openUrl()
{
}


void MainWindow::keyPressEvent(QKeyEvent *event)
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














































