#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Eigen>

#include </usr/local/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/include/glm/glm.hpp>

class Shader
{
public:
    QGLShaderProgram  m_program;
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(){}

    Shader(QString vertexPath, QString fragmentPath, QString geometryPath = nullptr)
    {
        bool flag = m_program.addShaderFromSourceFile(QGLShader::Vertex, vertexPath);
        if(!flag) {
            QString error =  m_program.log();
            std::cout << "ERROR::VertexSHADER" << m_program.log().toStdString() << std::endl;
            error = "";
            return;
        }
        flag = m_program.addShaderFromSourceFile(QGLShader::Fragment, fragmentPath);
        if(!flag) {
            std::cout << "ERROR::FragmentSHADER" << m_program.log().toStdString() << std::endl;
            return;
        }

        if(geometryPath != nullptr)
        {
            flag = m_program.addShaderFromSourceFile(QGLShader::Geometry, geometryPath);
            if(!flag) {
                std::cout << "ERROR::GeometrySHADER" << m_program.log().toStdString() << std::endl;
                return;
            }
        }


        ID = m_program.programId();
        if(!m_program.link()) {
            std::cout << "ERROR::Link" << m_program.log().toStdString() << std::endl;
        }

    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const Eigen::Vector2f &value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const Eigen::Vector3f &value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const Eigen::Vector4f &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const Eigen::Matrix2f &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.data());
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const Eigen::Matrix3f &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.data());
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const Eigen::Matrix4f &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.data());
    }
    // ------------------------------------------------------------------------
    void setglm4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};
#endif
