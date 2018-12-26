
TEMPLATE = app
TARGET = Base

QT += gui opengl

QMAKE_MAC_SDK = macosx10.14

CONFIG += c++11


INCLUDEPATH += /usr/local/Cellar/glm/0.9.9.2/include/glm \
$$PWD/../../../../Library/Developer/Xcode/DerivedData/cal3d-adkqhxvjhhxkdieeiwwdgrftkguv/Build/Products/Debug/cal3d.framework/Versions/A/Headers/


# Input
HEADERS += glwidget.h mainwindow.h \
    geometry.h \
    particle.h \
    generator.h \
    shader.h \
    generator.h \
    geometry.h \
    glwidget.h \
    mainwindow.h \
    particle.h \
    shader.h \
    cal3dExt/tga.h \
    cal3dExt/model.h \
    cal3dExt/modelloader.h
FORMS += mainwindow.ui
SOURCES += glwidget.cpp main.cpp mainwindow.cpp \
    geometry.cpp \
    particle.cpp \
    generator.cpp \
    cal3dExt/tga.cpp \
    cal3dExt/model.cpp \
    cal3dExt/modelloader.cpp

DISTFILES += \
    shaders/simpleshader.vert \
    shaders/simpleshader.frag \
    shaders/particles.frag \
    shaders/particles.vert \
    data/cally/cally.csf \
    data/cally/cally_calf_left.cmf \
    data/cally/cally_calf_right.cmf \
    data/cally/cally_chest.cmf \
    data/cally/cally_foot_left.cmf \
    data/cally/cally_foot_right.cmf \
    data/cally/cally_hand_left.cmf \
    data/cally/cally_hand_right.cmf \
    data/cally/cally_head.cmf \
    data/cally/cally_idle.caf \
    data/cally/cally_jog.caf \
    data/cally/cally_lowerarm_left.cmf \
    data/cally/cally_lowerarm_right.cmf \
    data/cally/cally_neck.cmf \
    data/cally/cally_pelvis.cmf \
    data/cally/cally_ponytail.cmf \
    data/cally/cally_shoot_arrow.caf \
    data/cally/cally_strut.caf \
    data/cally/cally_thigh_left.cmf \
    data/cally/cally_thigh_right.cmf \
    data/cally/cally_tornado_kick.caf \
    data/cally/cally_upperarm_left.cmf \
    data/cally/cally_upperarm_right.cmf \
    data/cally/cally_walk.caf \
    data/cally/cally_wave.caf \
    data/paladin/paladin.csf \
    data/paladin/paladin_body.cmf \
    data/paladin/paladin_cape.cmf \
    data/paladin/paladin_idle.caf \
    data/paladin/paladin_jog.caf \
    data/paladin/paladin_loincloth.cmf \
    data/paladin/paladin_shoot_arrow.caf \
    data/paladin/paladin_sneak.caf \
    data/paladin/paladin_strut.caf \
    data/paladin/paladin_walk.caf \
    data/paladin/paladin_wave.caf \
    data/skeleton/skeleton.csf \
    data/skeleton/skeleton_calf_left.cmf \
    data/skeleton/skeleton_calf_right.cmf \
    data/skeleton/skeleton_chest.cmf \
    data/skeleton/skeleton_foot_left.cmf \
    data/skeleton/skeleton_foot_right.cmf \
    data/skeleton/skeleton_hair.cmf \
    data/skeleton/skeleton_hand_left.cmf \
    data/skeleton/skeleton_hand_right.cmf \
    data/skeleton/skeleton_head.cmf \
    data/skeleton/skeleton_hiphop.caf \
    data/skeleton/skeleton_idle.caf \
    data/skeleton/skeleton_jog.caf \
    data/skeleton/skeleton_lowerarm_left.cmf \
    data/skeleton/skeleton_lowerarm_right.cmf \
    data/skeleton/skeleton_neck.cmf \
    data/skeleton/skeleton_pelvis.cmf \
    data/skeleton/skeleton_shoot_arrow.caf \
    data/skeleton/skeleton_spine_lower.cmf \
    data/skeleton/skeleton_spine_upper.cmf \
    data/skeleton/skeleton_strut.caf \
    data/skeleton/skeleton_thigh_left.cmf \
    data/skeleton/skeleton_thigh_right.cmf \
    data/skeleton/skeleton_upperarm_left.cmf \
    data/skeleton/skeleton_upperarm_right.cmf \
    data/skeleton/skeleton_walk.caf \
    data/skeleton/skeleton_wave.caf \
    data/paladin/paladin_arm.tga \
    data/paladin/paladin_cape.tga \
    data/paladin/paladin_chest.tga \
    data/paladin/paladin_cloth.tga \
    data/paladin/paladin_edge.tga \
    data/paladin/paladin_edge_alpha.tga \
    data/paladin/paladin_flesh.tga \
    data/paladin/paladin_gird.tga \
    data/paladin/paladin_head.tga \
    data/paladin/paladin_legging.tga \
    data/paladin/paladin_legguard.tga \
    data/paladin/paladin_legplate_left.tga \
    data/paladin/paladin_legplate_right.tga \
    data/paladin/paladin_plate.tga \
    data/paladin/paladin_ponytail.tga \
    data/paladin/paladin_shoe.tga \
    data/paladin/paladin_stripe.tga \
    data/cally/cally_chest.xrf \
    data/cally/cally_pelvis.xrf \
    data/cally/cally_ponytail.xrf \
    data/cally/cally_skin.xrf \
    data/paladin/paladin_arm_left.xrf \
    data/paladin/paladin_arm_right.xrf \
    data/paladin/paladin_cape.xrf \
    data/paladin/paladin_chest.xrf \
    data/paladin/paladin_edge.xrf \
    data/paladin/paladin_flesh.xrf \
    data/paladin/paladin_gird.xrf \
    data/paladin/paladin_head.xrf \
    data/paladin/paladin_leg_left.xrf \
    data/paladin/paladin_leg_right.xrf \
    data/paladin/paladin_legguard_left.xrf \
    data/paladin/paladin_legguard_right.xrf \
    data/paladin/paladin_legplate_left.xrf \
    data/paladin/paladin_legplate_right.xrf \
    data/paladin/paladin_loin_lower.xrf \
    data/paladin/paladin_loin_upper.xrf \
    data/paladin/paladin_ponytail.xrf \
    data/paladin/paladin_shoe.xrf \
    data/paladin/paladin_stripe.xrf \
    data/skeleton/skeleton_bone.xrf \
    data/skeleton/skeleton_hair1.xrf \
    data/skeleton/skeleton_hair2.xrf \
    data/skeleton/skeleton_hair3.xrf \
    data/cally.cfg \
    data/paladin.cfg \
    data/skeleton.cfg


RESOURCES += \
    resources.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Library/Developer/Xcode/DerivedData/cal3d-adkqhxvjhhxkdieeiwwdgrftkguv/Build/Products/Debug/cal3d.framework/Versions/A/Headers/cal3d/.libs/release/ -lcal3d.12.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Library/Developer/Xcode/DerivedData/cal3d-adkqhxvjhhxkdieeiwwdgrftkguv/Build/Products/Debug/cal3d.framework/Versions/A/Headers/cal3d/.libs/debug/ -lcal3d.12.0.0
else:unix: LIBS += -L$$PWD/../../../../Library/Developer/Xcode/DerivedData/cal3d-adkqhxvjhhxkdieeiwwdgrftkguv/Build/Products/Debug/cal3d.framework/Versions/A/Headers/cal3d/.libs/ -lcal3d.12.0.0

INCLUDEPATH += $$PWD/../../../../Library/Developer/Xcode/DerivedData/cal3d-adkqhxvjhhxkdieeiwwdgrftkguv/Build/Products/Debug/cal3d.framework/Versions/A/Headers/cal3d/.libs
DEPENDPATH += $$PWD/../../../../Library/Developer/Xcode/DerivedData/cal3d-adkqhxvjhhxkdieeiwwdgrftkguv/Build/Products/Debug/cal3d.framework/Versions/A/Headers/cal3d/.libs
