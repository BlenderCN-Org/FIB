// Author: Marc Comino 2018

#include "./mesh_io.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "./triangle_mesh.h"

namespace data_representation {

namespace {

template <typename T>
void Add3Items(T i1, T i2, T i3, int index, std::vector<T> *vector) {
  (*vector)[index] = i1;
  (*vector)[index + 1] = i2;
  (*vector)[index + 2] = i3;
}

bool ReadPlyHeader(std::ifstream *fin, int *vertices, int *faces) {
  char line[100];

  fin->getline(line, 100);
  if (strncmp(line, "ply", 3) != 0) return false;

  *vertices = 0;
  fin->getline(line, 100);
  while (strncmp(line, "end_header", 10) != 0) {
    if (strncmp(line, "element vertex", 14) == 0) *vertices = atoi(&line[15]);
    fin->getline(line, 100);
    if (strncmp(line, "element face", 12) == 0) *faces = atoi(&line[13]);
  }

  if (*vertices <= 0) return false;

  std::cout << "Loading triangle mesh" << std::endl;
  std::cout << "\tVertices = " << *vertices << std::endl;
  std::cout << "\tFaces = " << *faces << std::endl;

  return true;
}

void ReadPlyVertices(std::ifstream *fin, TriangleMesh *mesh) {
  const int kVertices = mesh->vertices_.size() / 3;
  for (int i = 0; i < kVertices; ++i) {
    float x, y, z;
    fin->read(reinterpret_cast<char *>(&x), sizeof(float));
    fin->read(reinterpret_cast<char *>(&y), sizeof(float));
    fin->read(reinterpret_cast<char *>(&z), sizeof(float));

    Add3Items(x, y, z, i * 3, &(mesh->vertices_));
  }
}

void ReadPlyFaces(std::ifstream *fin, TriangleMesh *mesh) {
  unsigned char vertex_per_face;

  const int kFaces = mesh->faces_.size() / 3;
  for (int i = 0; i < kFaces; ++i) {
    int v1, v2, v3;
    fin->read(reinterpret_cast<char *>(&vertex_per_face),
              sizeof(unsigned char));
    assert(vertex_per_face == 3);

    fin->read(reinterpret_cast<char *>(&v1), sizeof(int));
    fin->read(reinterpret_cast<char *>(&v2), sizeof(int));
    fin->read(reinterpret_cast<char *>(&v3), sizeof(int));
    Add3Items(v1, v2, v3, i * 3, &(mesh->faces_));
  }
}

void ComputeVertexNormals(const std::vector<float> &vertices,
                          const std::vector<int> &faces,
                          std::vector<float> *normals) {
  const int kFaces = faces.size();
  std::vector<float> face_normals(kFaces, 0);

  for (int i = 0; i < kFaces; i += 3) {
    Eigen::Vector3d v1(vertices[faces[i] * 3], vertices[faces[i] * 3 + 1],
                       vertices[faces[i] * 3 + 2]);
    Eigen::Vector3d v2(vertices[faces[i + 1] * 3],
                       vertices[faces[i + 1] * 3 + 1],
                       vertices[faces[i + 1] * 3 + 2]);
    Eigen::Vector3d v3(vertices[faces[i + 2] * 3],
                       vertices[faces[i + 2] * 3 + 1],
                       vertices[faces[i + 2] * 3 + 2]);
    Eigen::Vector3d v1v2 = v2 - v1;
    Eigen::Vector3d v1v3 = v3 - v1;
    Eigen::Vector3d normal = v1v2.cross(v1v3);

    if (normal.norm() < 0.0000000001) {
      normal = Eigen::Vector3d(0.0, 0.0, 0.0);
    } else {
      normal.normalize();
    }

    for (int j = 0; j < 3; ++j) face_normals[i + j] = normal[j];
  }

  const int kVertices = vertices.size();
  normals->resize(kVertices, 0);
  for (int i = 0; i < kFaces; i += 3) {
    for (int j = 0; j < 3; ++j) {
      int idx = faces[i + j];
      Eigen::Vector3d v1(vertices[faces[i + j] * 3],
                         vertices[faces[i + j] * 3 + 1],
                         vertices[faces[i + j] * 3 + 2]);
      Eigen::Vector3d v2(vertices[faces[i + (j + 1) % 3] * 3],
                         vertices[faces[i + (j + 1) % 3] * 3 + 1],
                         vertices[faces[i + (j + 1) % 3] * 3 + 2]);
      Eigen::Vector3d v3(vertices[faces[i + (j + 2) % 3] * 3],
                         vertices[faces[i + (j + 2) % 3] * 3 + 1],
                         vertices[faces[i + (j + 2) % 3] * 3 + 2]);

      Eigen::Vector3d v1v2 = v2 - v1;
      Eigen::Vector3d v1v3 = v3 - v1;
      double angle = acos(v1v2.dot(v1v3) / (v1v2.norm() * v1v3.norm()));

      if (angle == angle) {
        for (int k = 0; k < 3; ++k) {
          (*normals)[idx * 3 + k] += face_normals[i + k] * angle;
        }
      }
    }
  }

  const int kNormals = normals->size();
  for (int i = 0; i < kNormals; i += 3) {
    Eigen::Vector3d normal((*normals)[i], (*normals)[i + 1], (*normals)[i + 2]);
    if (normal.norm() > 0.00001) {
      normal.normalize();
    } else {
      normal = Eigen::Vector3d(0, 0, 0);
    }

    for (int j = 0; j < 3; ++j) (*normals)[i + j] = normal[j];
  }
}

void ComputeBoundingBox(const std::vector<float> vertices, TriangleMesh *mesh) {
  const int kVertices = vertices.size() / 3;
  for (int i = 0; i < kVertices; ++i) {
    mesh->min_[0] = std::min(mesh->min_[0], vertices[i * 3]);
    mesh->min_[1] = std::min(mesh->min_[1], vertices[i * 3 + 1]);
    mesh->min_[2] = std::min(mesh->min_[2], vertices[i * 3 + 2]);

    mesh->max_[0] = std::max(mesh->max_[0], vertices[i * 3]);
    mesh->max_[1] = std::max(mesh->max_[1], vertices[i * 3 + 1]);
    mesh->max_[2] = std::max(mesh->max_[2], vertices[i * 3 + 2]);
  }
}

}  // namespace

bool ReadFromPly(const std::string &filename, TriangleMesh *mesh) {
  std::ifstream fin;

  fin.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!fin.is_open() || !fin.good()) return false;

  int vertices = 0, faces = 0;
  if (!ReadPlyHeader(&fin, &vertices, &faces)) {
    fin.close();
    return false;
  }

  mesh->vertices_.resize(vertices * 3);
  ReadPlyVertices(&fin, mesh);

  mesh->faces_.resize(faces * 3);
  ReadPlyFaces(&fin, mesh);

  fin.close();

  ComputeVertexNormals(mesh->vertices_, mesh->faces_, &mesh->normals_);
  ComputeBoundingBox(mesh->vertices_, mesh);

  return true;
}

bool WriteToPly(const std::string &filename, TriangleMesh *mesh) {
  (void)filename;
  (void)mesh;


  std::ofstream outFile;

  outFile.open(filename.c_str(), std::ios_base::out | std::ios_base::binary);
  if (!outFile.is_open() || !outFile.good()) return false;

  outFile << "ply" << std::endl;
  outFile << "format binary_little_endian 1.0" << std::endl;
  outFile << "comment saved by EG" << std::endl;
  outFile << "element vertex " << mesh->vertices_.size()/3 << std::endl;
  outFile << "property float x" << std::endl;
  outFile << "property float y" << std::endl;
  outFile << "property float z" << std::endl;
  outFile << "element face " << mesh->faces_.size()/3 << std::endl;
  outFile << "property list uchar int vertex_indices" << std::endl;
  outFile << "end_header" << std::endl;

  //Points
  for(int i=0; i<(int) mesh->vertices_.size()/3; i++)
  {
      for (int j=0; j<3; j++)
      {
          //outFile.write( reinterpret_cast<const char*>( &mesh->vertices_[i*3+j] ), sizeof( float ));
          outFile << mesh->vertices_[i*3+j];
          outFile << " ";
      }
      outFile << std::endl;
  }


  //Faces
  for(int i=0; i<(int) mesh->faces_.size()/3; i++)
  {
      outFile << "3 ";
      for (int j=0; j<3; j++)
      {
          //outFile.write( reinterpret_cast<const char*>( &mesh->faces_[i*3+j] ), sizeof( int ));
          outFile << mesh->faces_[i*3+j];
          outFile << " ";
      }
      outFile << std::endl;
  }

  outFile.close();

  return true;
}






}  // namespace data_representation
