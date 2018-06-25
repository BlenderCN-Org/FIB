import bpy
from time import time


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


def ex4(me):

    neighbor=neighbors(me)
    n=len(me.edges)
    convex=0
    concave=0

    for edge in range(n):
        list_poly=neighbor[edge]
        if (len(list_poly)==2):
            f1=list_poly[0]
            f2=list_poly[1]
            n1=f1.normal
            n2=f2.normal
            angle=n1.dot(n2)
            if (angle<180):
                convex+=1
            else:
                concave+=1


    print("Convex edges :",convex)
    print("Concave edges",concave)




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
    ex4(mesh)
    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))
