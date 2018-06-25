import bpy
from time import time


def ex3(me):

    n=len(me.edges)
    neighbors=[0 for i in range(n)]
    m=len(me.polygons)

    for edge in range(n):
        for poly in me.polygons:
            vert=me.edges[edge].vertices
            if ((vert[0] in poly.vertices) and (vert[1] in poly.vertices)):
                neighbors[edge]+=1
    boundary=0
    manifold=0
    nonmanifold=0

    for i in range(n):
        if (neighbors[i]==1):
            boundary+=1
        elif (neighbors[i]==2):
            manifold+=1
        elif (neighbors[i]>2):
            nonmanifold+=1

    print("Boudary edges : ",boundary)
    print("Manifold edges :",manifold)
    print("Non-manifold edges", nonmanifold)


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
    ex3(mesh)
    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))
