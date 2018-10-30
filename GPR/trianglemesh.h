#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H


#include <vector>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>


using namespace std;

struct CornerEdge
{
    int vertexA, vertexB, corner;

    bool operator<(const CornerEdge &cEdge) { return (vertexA < cEdge.vertexA) || ((vertexA == cEdge.vertexA) && (vertexB < cEdge.vertexB)); }
    bool operator==(const CornerEdge &cEdge) { return (vertexA == cEdge.vertexA) && (vertexB == cEdge.vertexB); }
};

//int next(int corner)
//{
//    return 3 * (corner / 3) + (corner + 1) % 3;
//}

//int previous(int corner)
//{
//    return 3 * (corner / 3) + (corner + 2) % 3;
//}

//int opposite(int corner)
//{
//    return cTable[corner];
//}


class TriangleMesh
{

public:
	TriangleMesh();

public:
	void addVertex(const QVector3D &position);
	void addTriangle(int v0, int v1, int v2);

	void buildCube();

	bool init(QOpenGLShaderProgram *program);
	void destroy();

	void render(QOpenGLFunctions &gl);

    void buildCornerTable();

private:
	void buildReplicatedVertices(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles);
	void fillVBOs(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles);

    void addEdgeCorner(int vertexA, int vertexB, int corner);
    void Insert(CornerEdge CEdge);

    float gCurvature(int v);
    float meanCurvature(int v);
    float tArea(int vi, int vj, int vk);
    float getAngle(int vi, int vj, int vk);
    void findNeighbors();
    void colorVertices();

    private:
    bool gaussianColor=true;

	vector<QVector3D> vertices;
	vector<int> triangles;

	QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vboVertices, vboNormals, eboTriangles, vboColors;

    vector<int> vertex_corners;
    vector<int> corner_vertices;
    vector<CornerEdge> edge_corners;
    vector<int> cTable;
    vector<vector<int>> VNeighbors;
    vector<float> gaussianColors;
    vector<float> meanColors;

};


#endif // TRIANGLEMESH_H
