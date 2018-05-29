QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ViewerPBS
TEMPLATE = app

CONFIG += c++14
CONFIG(release, release|debug):QMAKE_CXXFLAGS += -Wall -O2 -std=c++11
CONFIG(debug, release|debug):QMAKE_CXXFLAGS += -Wall -O2 -std=c++11

CONFIG(release, release|debug):DESTDIR = release/
CONFIG(release, release|debug):OBJECTS_DIR = release/
CONFIG(release, release|debug):MOC_DIR = release/
CONFIG(release, release|debug):UI_DIR = release/

CONFIG(debug, release|debug):DESTDIR = debug/
CONFIG(debug, release|debug):OBJECTS_DIR = debug/
CONFIG(debug, release|debug):MOC_DIR = debug/
CONFIG(debug, release|debug):UI_DIR = debug/



INCLUDEPATH += /dades/emeline.got/linux/

LIBS += -lGLEW

SOURCES += \
    triangle_mesh.cc \
    mesh_io.cc \
    main.cc \
    main_window.cc \
    glwidget.cc \
    camera.cc

HEADERS  += \
    triangle_mesh.h \
    mesh_io.h \
    main_window.h \
    glwidget.h \
    camera.h

FORMS    += \
    main_window.ui

OTHER_FILES +=

DISTFILES += \
    shaders/reflection.frag \
    shaders/reflection.vert \
    shaders/brdf.frag \
    shaders/brdf.vert \
    shaders/sky.frag \
    shaders/sky.vert


