#include <iostream>
#include <cmath>
#include "trianglemesh.h"


using namespace std;


//struct CornerEdge
//{
//	int vertexA, vertexB, corner;

//	bool operator<(const CornerEdge &cEdge) { return (vertexA < cEdge.vertexA) || ((vertexA == cEdge.vertexA) && (vertexB < cEdge.vertexB)); }
//	bool operator==(const CornerEdge &cEdge) { return (vertexA == cEdge.vertexA) && (vertexB == cEdge.vertexB); }
//};


int next(int corner)
{
    return 3 * (corner / 3) + (corner + 1) % 3;
}

int previous(int corner)
{
    return 3 * (corner / 3) + (corner + 2) % 3;
}

//Insert in order
vector<int> insert(int value, vector<int> v){
    bool flag=false;
    int i=0;

    while (!flag && i<v.size()){
        flag = (value==v[i]);
        i++;
    }

    if (!flag){
        v.push_back(value);
    }

    return v;
}



TriangleMesh::TriangleMesh() : vboVertices(QOpenGLBuffer::VertexBuffer),
										 vboNormals(QOpenGLBuffer::VertexBuffer),
										 eboTriangles(QOpenGLBuffer::IndexBuffer)
{
}


void TriangleMesh::addVertex(const QVector3D &position)
{
	vertices.push_back(position);
}

void TriangleMesh::addTriangle(int v0, int v1, int v2)
{
	triangles.push_back(v0);
	triangles.push_back(v1);
	triangles.push_back(v2);
}

void TriangleMesh::buildCube()
{
	GLfloat vertices[] = {-1, -1, -1,
								  1, -1, -1,
								  1,  1, -1,
								 -1,  1, -1,
								 -1, -1,  1,
								  1, -1,  1,
								  1,  1,  1,
								 -1,  1,  1
								};

	GLuint faces[] = {3, 1, 0, 3, 2, 1,
							5, 6, 7, 4, 5, 7,
							7, 3, 0, 0, 4, 7,
							1, 2, 6, 6, 5, 1,
							0, 1, 4, 5, 4, 1,
							2, 3, 7, 7, 6, 2
						  };

	int i;

	for(i=0; i<8; i+=1)
		addVertex(0.5f * QVector3D(vertices[3*i], vertices[3*i+1], vertices[3*i+2]));
	for(i=0; i<12; i++)
		addTriangle(faces[3*i], faces[3*i+1], faces[3*i+2]);
}

bool TriangleMesh::init(QOpenGLShaderProgram *program)
{
	vector<QVector3D> replicatedVertices, normals;
	vector<unsigned int> perFaceTriangles;

    buildReplicatedVertices(replicatedVertices, normals, perFaceTriangles);
    program->bind();

	vao.destroy();
	vao.create();
	if(vao.isCreated())
		vao.bind();
	else
		return false;

	vboVertices.destroy();
	vboVertices.create();
	if(vboVertices.isCreated())
		vboVertices.bind();
	else
		return false;
	vboVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);
	program->enableAttributeArray(0);
	program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

	vboNormals.destroy();
	vboNormals.create();
	if(vboNormals.isCreated())
		vboNormals.bind();
	else
		return false;
	vboNormals.setUsagePattern(QOpenGLBuffer::StaticDraw);
	program->enableAttributeArray(1);
	program->setAttributeBuffer(1, GL_FLOAT, 0, 3, 0);

	eboTriangles.destroy();
	eboTriangles.create();
	if(eboTriangles.isCreated())
		eboTriangles.bind();
	else
		return false;
	eboTriangles.setUsagePattern(QOpenGLBuffer::StaticDraw);



    //COLORS----------------------------------------------
    colorVertices();

    vboColors.destroy();
    vboColors.create();
    if(vboColors.isCreated())
        vboColors.bind();
    else
        return false;
    vboColors.setUsagePattern(QOpenGLBuffer::StaticDraw);
    program->enableAttributeArray(2);
    program->setAttributeBuffer(2, GL_FLOAT, 0, 3, 0);

    //----------------------------------------------------


	fillVBOs(replicatedVertices, normals, perFaceTriangles);

	vao.release();
	program->release();

	return true;
}

void TriangleMesh::destroy()
{
	vao.destroy();
	vboVertices.destroy();
	vboNormals.destroy();
	eboTriangles.destroy();
    vboColors.destroy();

	vertices.clear();
	triangles.clear();

    vertex_corners.clear();
    corner_vertices.clear();
    edge_corners.clear();
    cTable.clear();
    VNeighbors.clear();
    gaussianColors.clear();
    meanColors.clear();
}

void TriangleMesh::render(QOpenGLFunctions &gl)
{
	vao.bind();
	eboTriangles.bind();
	gl.glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
    vao.release();

}

void TriangleMesh::buildReplicatedVertices(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles)
{
	normals.resize(triangles.size());

	for(unsigned int i=0; i<triangles.size(); i+=3)
	{
		replicatedVertices.push_back(vertices[triangles[i]]);
		replicatedVertices.push_back(vertices[triangles[i+1]]);
		replicatedVertices.push_back(vertices[triangles[i+2]]);

		QVector3D N = QVector3D::crossProduct(vertices[triangles[i+1]] - vertices[triangles[i]], vertices[triangles[i+2]] - vertices[triangles[i]]);
		N.normalize();
		normals[i] = N;
		normals[i+1] = N;
		normals[i+2] = N;

		perFaceTriangles.push_back(perFaceTriangles.size());
		perFaceTriangles.push_back(perFaceTriangles.size());
		perFaceTriangles.push_back(perFaceTriangles.size());
    }

}

void TriangleMesh::fillVBOs(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles)
{
	vboVertices.bind();
	vboVertices.allocate(&replicatedVertices[0], 3 * sizeof(float) * replicatedVertices.size());
	vboVertices.release();

	vboNormals.bind();
	vboNormals.allocate(&normals[0], 3 * sizeof(float) * normals.size());
	vboNormals.release();

	eboTriangles.bind();
	eboTriangles.allocate(&perFaceTriangles[0], sizeof(int) * perFaceTriangles.size());
	eboTriangles.release();

    vboColors.bind();
    vboColors.allocate(&gaussianColors[0], 3 * sizeof(float) * gaussianColors.size());
    vboColors.release();


}



//--------------------------------------------------------------------------------------
//----------------------------- CORNER TABLE -------------------------------------------
//--------------------------------------------------------------------------------------


void TriangleMesh::buildCornerTable(){

    CornerEdge CE;
    CE.vertexA=0;
    CE.vertexB=0;
    CE.corner=0;
    edge_corners.clear();
    edge_corners.resize(triangles.size(),CE);

    //Create temp variables for the vectors
    vector<int> cornerTable(triangles.size());  //Corner table
    vector<int> vertex_corners_temp(vertices.size());  //Vertex to corner table
    vector<int> corner_vertices_temp(triangles.size());   //Corner to vertex table


    //Add a corner for every edge
    for(int i=0; i<triangles.size()-2; i+=3){
        addEdgeCorner(triangles[i],triangles[i+1],i);
        addEdgeCorner(triangles[i+1],triangles[i+2],i+1);
        addEdgeCorner(triangles[i+2],triangles[i],i+2);

        //Create V to C table
        vertex_corners_temp[triangles[i+2]] = i;
        vertex_corners_temp[triangles[i]] = i+1;
        vertex_corners_temp[triangles[i+1]] = i+2;

        //Create C to V table
        corner_vertices_temp[i] = triangles[i+2];
        corner_vertices_temp[i+1] = triangles[i];
        corner_vertices_temp[i+2] = triangles[i+1];
    }


    //Find the opposites and fill corner table
    for(int i=0;i<edge_corners.size()-1;i+=2){
        cornerTable[edge_corners[i].corner]=edge_corners[i+1].corner;
        cornerTable[edge_corners[i+1].corner]=edge_corners[i].corner;
    }

    cTable = cornerTable;
    vertex_corners = vertex_corners_temp;
    corner_vertices = corner_vertices_temp;


}




//Insert in order
void TriangleMesh::Insert(CornerEdge CEdge)
{

    if(CEdge.corner==0){
        edge_corners[0] = CEdge;
    }

    else{
        int j=CEdge.corner;
        while(CEdge<edge_corners[j-1] && j>0){
            edge_corners[j]=edge_corners[j-1];
            j-=1;
        }
        edge_corners[j]=CEdge;
    }

}


//Add a CornerEdge in the list of corner edges in order (i,j,c with i<j in ordered list)
void TriangleMesh::addEdgeCorner(int vA, int vB, int c){

    CornerEdge CE;
    CE.vertexA=vA;
    CE.vertexB=vB;
    CE.corner=c;

    if(vA > vB){
        CE.vertexA=vB;
        CE.vertexB=vA;
    }

    Insert(CE);
}




//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
//------------------------------- CURVATURES -------------------------------------------
//--------------------------------------------------------------------------------------


//Gaussian curvature
float TriangleMesh::gCurvature(int v){

    vector<int> neighbors = VNeighbors[v];  //Get vertices of the first ring around v

    float Area = 0.0f;
    float Theta = 0.0f;

    for(int i=0; i<neighbors.size()-1; i++){
        int vj = neighbors[i];
        int vk = neighbors[i+1];
        Area+=tArea(v,vj,vk);
        Theta+=getAngle(v,vj,vk);
    }

    Area+=tArea(v,neighbors[neighbors.size()-1],neighbors[0]);
    Theta+=getAngle(v,neighbors[neighbors.size()-1],neighbors[0]);

    return (2*M_PI - Theta)/(Area/3);
}


float TriangleMesh::meanCurvature(int v){
    vector<int> neighbors = VNeighbors[v];  //Get vertices of the first ring around v

    float Area = 0.0f;
    QVector3D sum(0.0f,0.0f,0.0f);

    for(int i=0; i<neighbors.size()-2; i++){
        int vk = neighbors[i];
        int vj = neighbors[i+1];
        int vl = neighbors[i+2];
        Area+=tArea(v,vj,vk);
        float angle = ((1/tan(getAngle(vk,vj,v)))+(1/tan(getAngle(vl,v,vj))));
        sum += angle*(vertices[vj]-vertices[v]);
    }

    int last = neighbors[neighbors.size()-1];
    int previouslast = neighbors[neighbors.size()-2];
    int first = neighbors[0];
    Area+=tArea(v,last,first)+tArea(v,previouslast,last);
    float angle = (tan(M_PI_2 - getAngle(previouslast,last,v))-tan(M_PI_2 - getAngle(first,v,last)));
    sum+=angle*(vertices[last]-vertices[v]);

    QVector3D DeltaS = (0.5f*sum)/Area;


    return 0.5f*DeltaS.length();
}


//Compute area of a triangle giving 3 vertices' indices
float TriangleMesh::tArea(int vi, int vj, int vk){
    return 0.5f*QVector3D::crossProduct((vertices[vj]-vertices[vi]),(vertices[vk]-vertices[vi])).length();
}

//Compute angle of vi in triangle vi,vj,vk
float TriangleMesh::getAngle(int vi, int vj, int vk){
    QVector3D u = (vertices[vj]-vertices[vi]);
    QVector3D v = (vertices[vk]-vertices[vi]);

    u.normalize();
    v.normalize();

    return acos(u.dotProduct(u,v));
}



//Find first ring of all vertices
void TriangleMesh::findNeighbors(){

    for(int i=0; i<vertices.size(); i++){

        vector<int> neighbors;
        vector<int> s;
        bool flag=false; //flag to continue exploring

        //Get corner of the vertex
        int vCorner = vertex_corners[i];

        int endCorner = previous(vCorner);  //Used to check if all vertices have been explored
        neighbors.push_back(corner_vertices[endCorner]); //Add it to the list of neighbors

        int addCorner = next(vCorner); //Starting point of exploration

        while(!flag){

            addCorner = cTable[addCorner]; //Find opposite

            flag = (addCorner==endCorner); //Check if we reach the end

            if(!flag){
                neighbors=insert(corner_vertices[addCorner],neighbors);
                addCorner = previous(addCorner);
                neighbors=insert(corner_vertices[addCorner],neighbors);
            }

        }

        VNeighbors.push_back(neighbors);
    }


}


void TriangleMesh::colorVertices(){

    buildCornerTable();
    findNeighbors();

    vector<float> gaussianC;
    vector<float> meanC;

    //Find max of both curvatures
    float maxGaussian = gCurvature(0);
    float maxMean = meanCurvature(0);
    gaussianC.push_back(gCurvature(0));
    meanC.push_back(meanCurvature(0));


    //Get the gaussian curvatures and mean curvatures of all the vertices
    for(int i=1; i<vertices.size(); i++){

        float gVal = gCurvature(i);
        float mVal = meanCurvature(i);

        gaussianC.push_back(gVal);
        meanC.push_back(mVal);

        if(gVal>maxGaussian)
            maxGaussian = gVal;
        if(mVal>maxMean)
            maxMean = mVal;
    }

    for(int i=0; i<vertices.size(); i++){
        //Get value between 0 & 1
        float gVal = (gaussianC[i]+maxGaussian)/(2*maxGaussian);
        float mVal = (meanC[i]/maxMean);

        if(gVal<=0.5){
            gaussianColors.push_back(1.0-gVal);
            gaussianColors.push_back(2*gVal+2);
            gaussianColors.push_back(gVal);
        }
        else{
            gaussianColors.push_back(1.0-gVal);
            gaussianColors.push_back(-2*gVal+2);
            gaussianColors.push_back(gVal);
        }


        if(mVal<=0.5){
            meanColors.push_back(1.0-mVal);
            meanColors.push_back(2*mVal+2);
            meanColors.push_back(mVal);
        }
        else{
            meanColors.push_back(1.0-mVal);
            meanColors.push_back(-2*mVal+2);
            meanColors.push_back(mVal);
        }

    }

//    for(int i=0; i<gaussianColors.size(); i++){
//        cout << gaussianColors[i] << endl;
//    }

}





//--------------------------------------------------------------------------------------


