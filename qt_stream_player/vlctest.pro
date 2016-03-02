#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T22:11:08
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vlctest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videowidget.cpp

HEADERS  += mainwindow.h \
    videowidget.h

FORMS    +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../Qt/VLC-Qt_1.0.1_win32_mingw/lib/ -lVLCQtCore -lVLCQtWidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../Qt/VLC-Qt_1.0.1_win32_mingw/lib/ -lVLCQtCored -lVLCQtWidgetsd
else:unix: LIBS += -L$$PWD/../../../../../Qt/VLC-Qt_1.0.1_win32_mingw/lib/ -lVLCQtCore -lVLCQtWidgets

INCLUDEPATH += $$PWD/../../../../../Qt/VLC-Qt_1.0.1_win32_mingw/include
DEPENDPATH += $$PWD/../../../../../Qt/VLC-Qt_1.0.1_win32_mingw/include
