################################################################################
# Needed Qt packages and build plugin
################################################################################

CONFIG += qt opengl thread release

QT += opengl uitools 
LIBPATH += /usr/local/Cellar/open-mesh/6.3/lib  /usr/local/Cellar/freeglut/3.0.0/lib
LIBS += -lOpenMeshCore -lOpenMeshTools -lglut
INCLUDEPATH += . /usr/local/include/eigen3/Eigen/ /usr/local/include

HEADERS = MeshViewerWidget.hh MeshViewerWidgetT.hh QGLViewerWidget.hh \
    taulaMC.hpp
SOURCES = meshviewer.cc MeshViewerWidgetT.cc QGLViewerWidget.cc \
    taulaMC.cc


