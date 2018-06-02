TEMPLATE = app
TARGET = ViewerPro
QT += widgets opengl 
CONFIG += qt debug_and_release


INCLUDEPATH     += . camera  /usr/local/include/eigen3/Eigen/ /usr/local/include/

win32-msvc2015{
	LIBS           += -lopengl32
	QMAKE_CXXFLAGS += /MP /Zi
	QMAKE_LFLAGS   += /MACHINE:X64
}

macx{
        QMAKE_SONAME_PREFIX = @executable_path/../Frameworks
        QMAKE_LFLAGS += -Wl,-rpath,@executable_path/../Frameworks
	QMAKE_LFLAGS   -= -mmacosx-version-min=10.8
	QMAKE_LFLAGS   += -mmacosx-version-min=10.9
	QMAKE_CXXFLAGS -= -mmacosx-version-min=10.8
	QMAKE_CXXFLAGS += -mmacosx-version-min=10.9
        QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
}

unix:!macx{
        CONFIG += C++11
}

mingw{
        LIBS += -lopengl32
}

# Input
HEADERS +=	MainWindow.h		\
		HW.h			\
                dummy.h			\
		camera/Camera.h \
                Helpers/mesh_io.h \
                Helpers/triangle_mesh.h \
                FRR/Viewer1.h \
                Helpers/mesh_importer.h \
    Helpers/shader.h \
    FRR/ssao_viewer.h \
    FRR/separable.h \
    SRGGE/ex1.h \
    SRGGE/ex2.h \
    SRGGE/ex4.h \
    SRGGE/ex5.h
		
SOURCES +=	main.cpp		\ 
		MainWindow.cpp 		\
		HW.cpp			\
                dummy.cpp		\
		camera/Camera.cpp \
                Helpers/mesh_io.cc \
                Helpers/triangle_mesh.cc \
                FRR/Viewer1.cpp \
                Helpers/mesh_importer.cpp \
    FRR/ssao_viewer.cpp \
    FRR/separable.cpp \
    SRGGE/ex1.cpp \
    SRGGE/ex2.cpp \
    SRGGE/ex4.cpp \
    SRGGE/ex5.cpp


DISTFILES += \
    shaders/ssao_geom.frag \
    shaders/ssao_geom.vert \
    shaders/ssao.frag \
    shaders/ssao.vert \
    shaders/blur.vert \
    shaders/blur.frag \
    shaders/separable.frag \
    shaders/separable.vert \
    shaders/try.frag \
    shaders/try.vert \
    shaders/try_ex4.frag \
    shaders/try_ex4.vert \
    shaders/phong.frag \
    shaders/phong.vert
