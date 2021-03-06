//----------------------------------------------------------------------------//
// model.cpp                                                                  //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#if defined(_MSC_VER) && _MSC_VER <= 0x0600
#pragma warning(disable : 4786)
#endif

#include <QOpenGLFunctions>

#include "cal3dExt/model.h"
#include "cal3d/model.h"
#include "tga.h"


//----------------------------------------------------------------------------//
// Static member variables initialization                                     //
//----------------------------------------------------------------------------//

const int Model::STATE_IDLE = 0;
const int Model::STATE_FANCY = 1;
const int Model::STATE_MOTION = 2;

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

Model::Model()
{
  m_calCoreModel = new cal3d::CalCoreModel("dummy");

  m_state = STATE_IDLE;
  m_motionBlend[0] = 0.6f;
  m_motionBlend[1] = 0.1f;
  m_motionBlend[2] = 0.3f;
//  m_animationCount = 0;
//  m_meshCount = 0;
  m_renderScale = 0.0001f;
  m_lodLevel = 1.0f;

}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

Model::~Model()
{
}

//----------------------------------------------------------------------------//
// Execute an action of the model                                             //
//----------------------------------------------------------------------------//

void Model::executeAction(int action)
{
  switch(action)
  {
    case 0:
      m_calModel->getMixer()->executeAction(m_animationId[5], 0.3f, 0.3f);
      break;
    case 1:
      m_calModel->getMixer()->executeAction(m_animationId[6], 0.3f, 0.3f);
      break;
  }
}

//----------------------------------------------------------------------------//
// Get the lod level of the model                                             //
//----------------------------------------------------------------------------//

float Model::getLodLevel()
{
  return m_lodLevel;
}

//----------------------------------------------------------------------------//
// Get the motion blend factors state of the model                            //
//----------------------------------------------------------------------------//

void Model::getMotionBlend(float *pMotionBlend)
{
  pMotionBlend[0] = m_motionBlend[0];
  pMotionBlend[1] = m_motionBlend[1];
  pMotionBlend[2] = m_motionBlend[2];
}

//----------------------------------------------------------------------------//
// Get the render scale of the model                                          //
//----------------------------------------------------------------------------//

float Model::getRenderScale()
{
  return m_renderScale;
}

//----------------------------------------------------------------------------//
// Get the animation state of the model                                       //
//----------------------------------------------------------------------------//

int Model::getState()
{
  return m_state;
}


//----------------------------------------------------------------------------//
// Initialize the model                                                       //
//----------------------------------------------------------------------------//

bool Model::onInit()
{


  m_calCoreModel->scale(0.01f);
  // Calculate Bounding Boxes

  m_calCoreModel->getCoreSkeleton()->calculateBoundingBoxes(m_calCoreModel);

  m_calModel = new cal3d::CalModel(m_calCoreModel);

  // attach all meshes to the model
  int meshId;
  for(meshId = 0; meshId < m_calCoreModel->getCoreMeshCount(); meshId++)
  {
    m_calModel->attachMesh(meshId);
  }

  // set the material set of the whole model
  m_calModel->setMaterialSet(0);

  // set initial animation state
  m_state = STATE_MOTION;
  m_calModel->getMixer()->blendCycle(m_animationId[STATE_MOTION], m_motionBlend[0], 0.0f);
  m_calModel->getMixer()->blendCycle(m_animationId[STATE_MOTION + 1], m_motionBlend[1], 0.0f);
  m_calModel->getMixer()->blendCycle(m_animationId[STATE_MOTION + 2], m_motionBlend[2], 0.0f);

  return true;
}

//----------------------------------------------------------------------------//
// Render the skeleton of the model                                           //
//----------------------------------------------------------------------------//

void Model::renderSkeleton()
{
//  // draw the bone lines
//  float lines[1024][2][3];
//  int nrLines;
//  nrLines =  m_calModel->getSkeleton()->getBoneLines(&lines[0][0][0]);
////  nrLines = m_calModel->getSkeleton()->getBoneLinesStatic(&lines[0][0][0]);

//  glLineWidth(3.0f);
//  glColor3f(1.0f, 1.0f, 1.0f);
//  glBegin(GL_LINES);
//    int currLine;
//    for(currLine = 0; currLine < nrLines; currLine++)
//    {
//      glVertex3f(lines[currLine][0][0], lines[currLine][0][1], lines[currLine][0][2]);
//      glVertex3f(lines[currLine][1][0], lines[currLine][1][1], lines[currLine][1][2]);
//    }
//  glEnd();
//  glLineWidth(1.0f);

//  // draw the bone points
//  float points[1024][3];
//  int nrPoints;
//  nrPoints =  m_calModel->getSkeleton()->getBonePoints(&points[0][0]);
////  nrPoints = m_calModel->getSkeleton()->getBonePointsStatic(&points[0][0]);

//  glPointSize(4.0f);
//  glBegin(GL_POINTS);
//    glColor3f(0.0f, 0.0f, 1.0f);
//    int currPoint;
//    for(currPoint = 0; currPoint < nrPoints; currPoint++)
//    {
//      glVertex3f(points[currPoint][0], points[currPoint][1], points[currPoint][2]);
//    }
//  glEnd();
//  glPointSize(1.0f);
}

//----------------------------------------------------------------------------//
// Render the bounding boxes of a model                                       //
//----------------------------------------------------------------------------//

void Model::renderBoundingBox()
{

//   cal3d::CalSkeleton *pCalSkeleton = m_calModel->getSkeleton();

//   std::vector<cal3d::CalBone*> &vectorCoreBone = pCalSkeleton->getVectorBone();

//   glColor3f(1.0f, 1.0f, 1.0f);
//   glBegin(GL_LINES);

//   for(size_t boneId=0;boneId<vectorCoreBone.size();++boneId)
//   {
//      cal3d::CalBoundingBox & calBoundingBox  = vectorCoreBone[boneId]->getBoundingBox();

//      cal3d::CalVector p[8];
//	  calBoundingBox.computePoints(p);


//	  glVertex3f(p[0].x,p[0].y,p[0].z);
//	  glVertex3f(p[1].x,p[1].y,p[1].z);

//	  glVertex3f(p[0].x,p[0].y,p[0].z);
//	  glVertex3f(p[2].x,p[2].y,p[2].z);

//	  glVertex3f(p[1].x,p[1].y,p[1].z);
//	  glVertex3f(p[3].x,p[3].y,p[3].z);

//	  glVertex3f(p[2].x,p[2].y,p[2].z);
//	  glVertex3f(p[3].x,p[3].y,p[3].z);

//  	  glVertex3f(p[4].x,p[4].y,p[4].z);
//	  glVertex3f(p[5].x,p[5].y,p[5].z);

//	  glVertex3f(p[4].x,p[4].y,p[4].z);
//	  glVertex3f(p[6].x,p[6].y,p[6].z);

//	  glVertex3f(p[5].x,p[5].y,p[5].z);
//	  glVertex3f(p[7].x,p[7].y,p[7].z);

//	  glVertex3f(p[6].x,p[6].y,p[6].z);
//	  glVertex3f(p[7].x,p[7].y,p[7].z);

//	  glVertex3f(p[0].x,p[0].y,p[0].z);
//	  glVertex3f(p[4].x,p[4].y,p[4].z);

//	  glVertex3f(p[1].x,p[1].y,p[1].z);
//	  glVertex3f(p[5].x,p[5].y,p[5].z);

//	  glVertex3f(p[2].x,p[2].y,p[2].z);
//	  glVertex3f(p[6].x,p[6].y,p[6].z);

//	  glVertex3f(p[3].x,p[3].y,p[3].z);
//	  glVertex3f(p[7].x,p[7].y,p[7].z);

//   }

//   glEnd();

}


//----------------------------------------------------------------------------//
// Render the mesh of the model                                               //
//----------------------------------------------------------------------------//

void Model::renderMesh(bool bWireframe, bool bLight)
{

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VerticesBuffer);
    glGenBuffers(1, &FacesBuffer);
    glGenBuffers(1,&NormalsBuffer);
    glGenBuffers(1,&ColorsBuffer);


  // get the renderer of the model
  cal3d::CalRenderer *pCalRenderer;
  pCalRenderer = m_calModel->getRenderer();

  // begin the rendering loop
  if(!pCalRenderer->beginRendering()) return;

  // set wireframe mode if necessary
  if(bWireframe)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  // set the global OpenGL states
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  // set the lighting mode if necessary
  if(bLight)
  {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  }

  // we will use vertex arrays, so enable them
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  // get the number of meshes
  int meshCount;
  meshCount = pCalRenderer->getMeshCount();

  // render all meshes of the model
  int meshId;
  for(meshId = 0; meshId < meshCount; meshId++)
  {
    // get the number of submeshes
    int submeshCount;
    submeshCount = pCalRenderer->getSubmeshCount(meshId);

    // render all submeshes of the mesh
    int submeshId;
    for(submeshId = 0; submeshId < submeshCount; submeshId++)
    {
      // select mesh and submesh for further data access
      if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
      {
//        unsigned char meshColor[4];
//        GLfloat materialColor[4];

//        // set the material ambient color
//        pCalRenderer->getAmbientColor(&meshColor[0]);
//        materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
//        glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);

//        // set the material diffuse color
//        pCalRenderer->getDiffuseColor(&meshColor[0]);
//        materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
//        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);

//        // set the vertex color if we have no lights
//        if(!bLight)
//        {
//          glColor4fv(materialColor);
//        }

//        // set the material specular color
//        pCalRenderer->getSpecularColor(&meshColor[0]);
//        materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
//        glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);

//        // set the material shininess factor
//        float shininess;
//        shininess = 50.0f; //TODO: pCalRenderer->getShininess();
//        glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

//        // get the transformed vertices of the submesh
//        static float meshVertices[30000][3];
//        int vertexCount;
//        vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

//        // get the transformed normals of the submesh
//        static float meshNormals[30000][3];
//        pCalRenderer->getNormals(&meshNormals[0][0]);

        // get the texture coordinates of the submesh
//        static float meshTextureCoordinates[30000][2];
//        int textureCoordinateCount;
//        textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

//        // get the faces of the submesh
//        static CalIndex meshFaces[50000][3];
//        int faceCount;
//        faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

        // set the vertex and normal buffers
//        glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
//        glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

//        // set the texture coordinate buffer and state if necessary
//        if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
//        {
//          glEnable(GL_TEXTURE_2D);
//          glActiveTexture(GL_TEXTURE0);
//          glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//          glEnable(GL_COLOR_MATERIAL);

//          // set the texture id we stored in the map user data
//          glBindTexture(GL_TEXTURE_2D, (GLuint)(size_t)pCalRenderer->getMapUserData(0));

//          // set the texture coordinate buffer
//          glTexCoordPointer(2, GL_FLOAT, 0, &meshTextureCoordinates[0][0]);
//          glColor3f(1.0f, 1.0f, 1.0f);
//        }

        // draw the submesh

        initBuffers(pCalRenderer);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3*faceCount, GL_UNSIGNED_INT,(GLvoid*) 0);
        glBindVertexArray(0);


//        if(sizeof(CalIndex)==2)
//			  glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_SHORT, &meshFaces[0][0]);
//		  else
//			  glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, &meshFaces[0][0]);

        // disable the texture coordinate state if necessary
//        if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
//        {
//          glDisable(GL_COLOR_MATERIAL);
//          glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//          glDisable(GL_TEXTURE_2D);
//        }

// DEBUG-CODE //////////////////////////////////////////////////////////////////
/*
glBegin(GL_LINES);
glColor3f(1.0f, 1.0f, 1.0f);
int vertexId;
for(vertexId = 0; vertexId < vertexCount; vertexId++)
{
const float scale = 0.3f;
  glVertex3f(meshVertices[vertexId][0], meshVertices[vertexId][1], meshVertices[vertexId][2]);
  glVertex3f(meshVertices[vertexId][0] + meshNormals[vertexId][0] * scale, meshVertices[vertexId][1] + meshNormals[vertexId][1] * scale, meshVertices[vertexId][2] + meshNormals[vertexId][2] * scale);
}
glEnd();
*/
////////////////////////////////////////////////////////////////////////////////
      }
    }
  }

  // clear vertex array state
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  // reset the lighting mode
  if(bLight)
  {
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
  }

  // reset the global OpenGL states
  glDisable(GL_DEPTH_TEST);

  // reset wireframe mode if necessary
  if(bWireframe)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  // end the rendering
  pCalRenderer->endRendering();


  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1,&VerticesBuffer);
  glDeleteBuffers(1, &FacesBuffer);
  glDeleteBuffers(1,&NormalsBuffer);
  glDeleteBuffers(1,&ColorsBuffer);


}

//----------------------------------------------------------------------------//
// Render the model                                                           //
//----------------------------------------------------------------------------//

void Model::onRender()
{
  // set global OpenGL states
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

//// DEBUG: CLOTH SIM
/*
  static CalVector spherePosition;
  spherePosition.set(Sphere.x, Sphere.y, Sphere.z);
  static float sphereRadius = 15.0f;

//  m_calModel->getSpringSystem()->setSphere(spherePosition.x, spherePosition.y, spherePosition.z, sphereRadius);

  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINE_STRIP);
    glVertex3f(spherePosition.x + sphereRadius, spherePosition.y, spherePosition.z);
    glVertex3f(spherePosition.x, spherePosition.y + sphereRadius, spherePosition.z);
    glVertex3f(spherePosition.x - sphereRadius, spherePosition.y, spherePosition.z);
    glVertex3f(spherePosition.x, spherePosition.y - sphereRadius, spherePosition.z);

    glVertex3f(spherePosition.x + sphereRadius, spherePosition.y, spherePosition.z);
    glVertex3f(spherePosition.x, spherePosition.y, spherePosition.z + sphereRadius);
    glVertex3f(spherePosition.x - sphereRadius, spherePosition.y, spherePosition.z);
    glVertex3f(spherePosition.x, spherePosition.y, spherePosition.z - sphereRadius);

    glVertex3f(spherePosition.x + sphereRadius, spherePosition.y, spherePosition.z);

    glVertex3f(spherePosition.x, spherePosition.y + sphereRadius, spherePosition.z);
    glVertex3f(spherePosition.x, spherePosition.y, spherePosition.z + sphereRadius);
    glVertex3f(spherePosition.x, spherePosition.y - sphereRadius, spherePosition.z);
    glVertex3f(spherePosition.x, spherePosition.y, spherePosition.z - sphereRadius);
    glVertex3f(spherePosition.x, spherePosition.y + sphereRadius, spherePosition.z);
  glEnd();
*/
//// DEBUG END

  //CalSkeleton *pCalSkeleton = m_calModel->getSkeleton();

  // Note :
  // You have to call coreSkeleton.calculateBoundingBoxes(calCoreModel)
  // during the initialisation (before calModel.create(calCoreModel))
  // if you want to use bounding boxes.

  m_calModel->getSkeleton()->calculateBoundingBoxes();

  // Note:
  // Uncomment the next lines if you want to test the experimental collision system.
  // Also remember that you need to call calculateBoundingBoxes()

  //m_calModel->getSpringSystem()->setForceVector(CalVector(0.0f,0.0f,0.0f));
  //m_calModel->getSpringSystem()->setCollisionDetection(true);

//  // check if we need to render the skeleton
//  if(theMenu.isSkeleton()==1)
//  {
//    renderSkeleton();
//  }
//  else if(theMenu.isSkeleton()==2)
//  {
//    renderBoundingBox();
//  }

//  // check if we need to render the mesh
//  if(theMenu.isSkeleton()==0 || theMenu.isWireframe())
//  {
    renderMesh(false, true);
//  }

  // clear global OpenGL states
  glDisable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------//
// Update the model                                                           //
//----------------------------------------------------------------------------//

void Model::onUpdate(float elapsedSeconds)
{
  // update the model
  m_calModel->update(elapsedSeconds);
}

//----------------------------------------------------------------------------//
// Shut the model down                                                        //
//----------------------------------------------------------------------------//

void Model::onShutdown()
{
  delete m_calModel;
  delete m_calCoreModel;
}

//----------------------------------------------------------------------------//
// Set the lod level of the model                                             //
//----------------------------------------------------------------------------//

void Model::setLodLevel(float lodLevel)
{
  m_lodLevel = lodLevel;

  // set the new lod level in the cal model renderer
  m_calModel->setLodLevel(m_lodLevel);
}

//----------------------------------------------------------------------------//
// Set the motion blend factors state of the model                            //
//----------------------------------------------------------------------------//

void Model::setMotionBlend(float *pMotionBlend, float delay)
{
  m_motionBlend[0] = pMotionBlend[0];
  m_motionBlend[1] = pMotionBlend[1];
  m_motionBlend[2] = pMotionBlend[2];

  m_calModel->getMixer()->clearCycle(m_animationId[STATE_IDLE], delay);
  m_calModel->getMixer()->clearCycle(m_animationId[STATE_FANCY], delay);
  m_calModel->getMixer()->blendCycle(m_animationId[STATE_MOTION], m_motionBlend[0], delay);
  m_calModel->getMixer()->blendCycle(m_animationId[STATE_MOTION + 1], m_motionBlend[1], delay);
  m_calModel->getMixer()->blendCycle(m_animationId[STATE_MOTION + 2], m_motionBlend[2], delay);

  m_state = STATE_MOTION;
}

//----------------------------------------------------------------------------//
// Set a new animation state within a given delay                             //
//----------------------------------------------------------------------------//

void Model::setState(int state, float delay)
{
  // check if this is really a new state
  if(state != m_state)
  {
    if(state == STATE_IDLE)
    {
      m_calModel->getMixer()->blendCycle(m_animationId[STATE_IDLE], 1.0f, delay);
      m_calModel->getMixer()->clearCycle(m_animationId[STATE_FANCY], delay);
      m_calModel->getMixer()->clearCycle(m_animationId[STATE_MOTION], delay);
      m_calModel->getMixer()->clearCycle(m_animationId[STATE_MOTION + 1], delay);
      m_calModel->getMixer()->clearCycle(m_animationId[STATE_MOTION + 2], delay);
      m_state = STATE_IDLE;
    }
    else if(state == STATE_FANCY)
    {
      m_calModel->getMixer()->clearCycle(m_animationId[STATE_IDLE], delay);
      m_calModel->getMixer()->blendCycle(m_animationId[STATE_FANCY], 1.0f, delay);
      m_calModel->getMixer()->clearCycle(m_animationId[STATE_MOTION], delay);
      m_calModel->getMixer()->clearCycle(m_animationId[STATE_MOTION + 1], delay);
      m_calModel->getMixer()->clearCycle(m_animationId[STATE_MOTION + 2], delay);
      m_state = STATE_FANCY;
    }
    else if(state == STATE_MOTION)
    {
      m_calModel->getMixer()->clearCycle(m_animationId[STATE_IDLE], delay);
      m_calModel->getMixer()->clearCycle(m_animationId[STATE_FANCY], delay);
      m_calModel->getMixer()->blendCycle(m_animationId[STATE_MOTION], m_motionBlend[0], delay);
      m_calModel->getMixer()->blendCycle(m_animationId[STATE_MOTION + 1], m_motionBlend[1], delay);
      m_calModel->getMixer()->blendCycle(m_animationId[STATE_MOTION + 2], m_motionBlend[2], delay);
      m_state = STATE_MOTION;
    }
  }
}


////----------------------------------------------------------------------------//
//// Set a path to override any config file path                                //
////----------------------------------------------------------------------------//

//void Model::setPath( const std::string& strPath )
//{
//   m_path = strPath;
//}

////----------------------------------------------------------------------------//


void Model::initBuffers(cal3d::CalRenderer *pCalRenderer){

    //VERTICES
    static float meshVertices[30000][3];
    int vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);
    vertices_array.clear();
    vertices_array.resize(vertexCount*3);

    static float meshNormals[5000][3];
    int normalCount = pCalRenderer -> getNormals(&meshNormals[0][0]);
    normals_array.clear();
    normals_array.resize(normalCount*3);

    unsigned char meshColor[4];
    GLfloat materialColor[3];
    pCalRenderer->getDiffuseColor(&meshColor[0]);
    materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f;
    colors_array.clear();
    colors_array.resize(vertexCount*3);

    for(int i=0; i<vertexCount; i++){
        vertices_array[i*3]=meshVertices[i][0];
        vertices_array[i*3+1]=meshVertices[i][1];
        vertices_array[i*3+2]=meshVertices[i][2];

        normals_array[i*3]=meshNormals[i][0];
        normals_array[i*3+1]=meshNormals[i][1];
        normals_array[i*3+2]=meshNormals[i][2];

        colors_array[i*3]=materialColor[0];
        colors_array[i*3+1]=materialColor[1];
        colors_array[i*3+2]=materialColor[2];
    }

    glBindBuffer(GL_ARRAY_BUFFER, VerticesBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3*vertexCount*sizeof(float), &vertices_array[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)0);

    //NORMALS
    glBindBuffer(GL_ARRAY_BUFFER, NormalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3*normalCount*sizeof(float), &normals_array[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);


    //COLORS
    glBindBuffer(GL_ARRAY_BUFFER, ColorsBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3*vertexCount*sizeof(float), &colors_array[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);


    //FACES
    static CalIndex meshFaces[30000][3];
    faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);
    faces_array.clear();
    faces_array.resize(faceCount*3);

    for(int i=0; i<faceCount; i++){
        faces_array[i*3]=meshFaces[i][0];
        faces_array[i*3+1]=meshFaces[i][1];
        faces_array[i*3+2]=meshFaces[i][2];
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FacesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*faceCount*sizeof(float), &faces_array[0], GL_STATIC_DRAW);



    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}
