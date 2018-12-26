//----------------------------------------------------------------------------//
// model.h                                                                    //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef MODEL_H
#define MODEL_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "global.h"
#include "cal3d/cal3d.h"
#include <QOpenGLFunctions>

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class Model
{
// misc
public:
  static const int STATE_IDLE;
  static const int STATE_FANCY;
  static const int STATE_MOTION;


// member variables
protected:
  int m_state;
  CalCoreModel* m_calCoreModel;
  CalModel* m_calModel;
//  int m_animationId[16];
  std::vector<int> m_animationId;
  int m_animationCount;
//  int m_meshId[32];
//  int m_meshCount;
//  GLuint m_textureId[32];
//  int m_textureCount;
  float m_motionBlend[3];
  float m_lodLevel;
  float m_renderScale;
//  std::string m_path;

// constructors/destructor
public:
  Model();
  virtual ~Model();

// member functions
public:
  void executeAction(int action);
  float getLodLevel();
  void getMotionBlend(float *pMotionBlend);
  float getRenderScale();
  int getState();
  bool onInit();
  void onRender();
  void onShutdown();
  void onUpdate(float elapsedSeconds);
  void setLodLevel(float lodLevel);
  void setMotionBlend(float *pMotionBlend, float delay);
  void setState(int state, float delay);
//  void setPath( const std::string& strPath );

  float getModelScale();
  void  setModelScale(float s);
  void  setCoreModel(CalCoreModel* coremodel);
  void  setAnimationIds(const std::vector<int>& animationIds);


/* DEBUG-CODE
  struct
  {
    float x, y, z;
  } Sphere;
  void adjustSphere(float x, float y, float z) { Sphere.x += x; Sphere.y += y; Sphere.z += z; };
*/

protected:
  GLuint loadTexture(const std::string& strFilename);
  void renderMesh(bool bWireframe, bool bLight);
  void renderSkeleton();
  void renderBoundingBox();

  void initBuffers(CalRenderer *pCalRenderer);

  GLuint VAO, VerticesBuffer, FacesBuffer;
  std::vector<float> vertices_array;
  std::vector<int> faces_array;
};


inline float Model::getModelScale() {
    return m_renderScale;
}

//inline void Model::getBoundingBox(Vec3f& bmin, Vec3f& bmax) {
//    bmin[0] = m_bbMin[0];
//    bmin[1] = m_bbMin[1];
//    bmin[2] = m_bbMin[2];
//    bmax[0] = m_bbMax[0];
//    bmax[1] = m_bbMax[1];
//    bmax[2] = m_bbMax[2];
//}

inline void Model::setCoreModel(CalCoreModel* coremodel) {
    m_calCoreModel = coremodel;
}

inline void Model::setAnimationIds(const std::vector<int>& ids) {
    m_animationId = ids;
}

inline void Model::setModelScale(float s) {
    m_renderScale = s;
}


#endif
