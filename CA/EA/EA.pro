
TEMPLATE = app
TARGET = Base

QT += gui opengl

QMAKE_MAC_SDK = macosx10.14

CONFIG += c++11

INCLUDEPATH += /usr/local/Cellar/glm/0.9.9.2/include/glm \
/usr/local/include

# Input
HEADERS += glwidget.h mainwindow.h \
    geometry.h \
    particle.h \
    generator.h \
    shader.h
FORMS += mainwindow.ui
SOURCES += glwidget.cpp main.cpp mainwindow.cpp \
    geometry.cpp \
    particle.cpp \
    generator.cpp

DISTFILES += \
    shaders/simpleshader.vert \
    shaders/simpleshader.frag \
    shaders/particles.frag \
    shaders/particles.vert

RESOURCES += \
    resources.qrc

