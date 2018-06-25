import bpy
from time import time

#Neighboring function
def neighbors(me):

    n=len(me.edges)
    neighbors=[[] for i in range(n)]
    m=len(me.polygons)

    for edge in range(n):
        for poly in me.polygons:
            vert=me.edges[edge].vertices
            if ((vert[0] in poly.vertices) and (vert[1] in poly.vertices)):
                neighbors[edge].append(poly)

    return(neighbors)

#Union find function on lists
def union_find(lis):
    lis = map(set, lis)
    unions = []
    for item in lis:
        temp = []
        for s in unions:
            if not s.isdisjoint(item):
                item = s.union(item)
            else:
                temp.append(s)
        temp.append(item)
        unions = temp
    return unions


#Function that creates the list of manifold polygons
def manifoldpolygons(me):

    n = len(me.edges)
    neighbor=neighbors(me)  #We obtain the neighboring faces with the neighbors function
    edge_list=[]   #List of all the edges
    non_manifold_list=[]

    for e in range(n): #We create a list of 2-manifold edges and a list of non-manifold vertices
        if (len(neighbor[e])==2):
            edge_list.append([me.edges[e].vertices[0],me.edges[e].vertices[1]])
        else:
            non_manifold_list.append(me.edges[e].vertices[0])
            non_manifold_list.append(me.edges[e].vertices[1])

    #Lists of manifold and non manifold shells
    shells=union_find(edge_list)

    #We store the non manifold shells
    non_manifold_shells=[]
    if len(non_manifold_list)!=0:
        for s in shells:
            for nm in non_manifold_list:
                if nm in s:
                    non_manifold_shells.append(s)


    #We create a list of manifold polygons (i.e. that form a 2-manifold shell)
    manifoldpolygons=[]
    for f in me.polygons:
        flag=True
        for s in non_manifold_shells:
            for v in f.vertices:
                if v in s:
                    flag=False
        if flag:
            manifoldpolygons.append(f)

    return(manifoldpolygons)


#Rounding function
def r(a):
    return int(a*1000+0.5)/1000.0


#Compute area of 2D Polygon if given lists of X coordinates
#and Y coordinates of the vertices
def area2d(X,Y):
    area=0
    N=len(X)
    for i in range(N):
        if i==N-1:
            area+=(X[i]-X[0])*(Y[i]+Y[0])
        else:
            area+=(X[i]-X[i+1])*(Y[i]+Y[i+1])
    return (area/2)



def ex8(me):

    Vol=0

    #We only compute the volume of 2-manifold shells
    for f in manifoldpolygons(me):

        X=[]
        Y=[]
        Z=[]

        #M is the matrix of the Gaussian integration method
        M=[[5,5,5],[11,2,2],[2,11,2],[2,2,11]]

        for v in f.vertices:
            #Create lists of X Y Z coordinates of each vertex of the face
            X.append(me.vertices[v].co[0])
            Y.append(me.vertices[v].co[1])
            Z.append(me.vertices[v].co[2])

        #Use area2d function to calculate non-normalized coordinates of normal vector
        Sx=area2d(Y,Z)
        Sy=area2d(Z,X)
        Sz=area2d(X,Y)

        i=0
        j=1

        for k in range(2,len(X)):
            for l in range(4):
                if (l==0):
                    w=-27/96  #weight for the centroid
                else:
                    w=25/96  #weight for the other points

                #Apply Gaussian integration method for each coordinates
                x=(M[l][0]*X[i]+M[l][1]*X[j]+M[l][2]*X[k])/15
                y=(M[l][0]*Y[i]+M[l][1]*Y[j]+M[l][2]*Y[k])/15
                z=(M[l][0]*Z[i]+M[l][1]*Z[j]+M[l][2]*Z[k])/15

                #Add the weighted sum
                Vol+=w*(x*Sx+y*Sy+z*Sz)/3

    print("Volume =", r(Vol))





def main():
    # Retrieve the active object (the last one selected)
    ob = bpy.data.scenes['Scene'].objects.active

    # Check that it is indeed a mesh
    if not ob or ob.type != 'MESH':
        BPyMessages.Error_NoMeshActive()
        return

    # If we are in edit mode, return to object mode
    bpy.ops.object.mode_set(mode='OBJECT')

    # Retrieve the mesh data
    mesh = ob.data

    # Get current time
    t = time()

    # Function that does all the work
    ex8(mesh)

    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))
