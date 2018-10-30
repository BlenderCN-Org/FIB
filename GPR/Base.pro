
TEMPLATE = app
TARGET = Base

QT += gui opengl

CONFIG += c++11 \
          sdk_no_version_check

INCLUDEPATH += .

# Input
HEADERS += glwidget.h mainwindow.h \
    trianglemesh.h \
    plyreader.h
FORMS += mainwindow.ui
SOURCES += glwidget.cpp main.cpp mainwindow.cpp \
    trianglemesh.cpp \
    plyreader.cpp

DISTFILES += \
    shaders/simpleshader.vert \
    shaders/simpleshader.frag

RESOURCES += \
    resources.qrc
