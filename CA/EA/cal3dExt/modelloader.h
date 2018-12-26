#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#include <QOpenGLWidget>
#include "cal3d/cal3d.h"
#include <map>
#include <vector>


class ModelData
{
public:
    CalCoreModel*	 coreModel;
    std::vector<int> animationIds;
    float			 renderScale;
};


class ModelLoader
{
public:
    static ModelData* getModel(const std::string& name, const std::string& path, const std::string& cfgFile);

protected:
    static bool loadModel(ModelData* data, const std::string& name, const std::string& path, const std::string& cfgFile);
    static GLuint loadTexture(const std::string& strFilename);

    static std::map<std::string, ModelData*>  s_models;

};



#endif //MODELLOADER_H
