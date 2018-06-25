import bpy
from time import time
import math

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


def ex7(me):

    for f in me.polygons:

        X=[]
        Y=[]
        Z=[]

        for v in f.vertices:
            #Create lists of X Y Z coordinates of each vertex of the face
            X.append(me.vertices[v].co[0])
            Y.append(me.vertices[v].co[1])
            Z.append(me.vertices[v].co[2])

        #Use aux function to calculate non-normalized coordinates of normal vector
        Sx=area2d(Y,Z)
        Sy=area2d(Z,X)
        Sz=area2d(X,Y)

        #The area is the norm of the normal vector
        area=math.sqrt(Sx**2+Sy**2+Sz**2)

        print("Computed Area",area)
        print("Stored Area",f.area)



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
    ex7(mesh)

    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))
