import bpy
from time import time

def ex2(me):

    n=len(me.vertices)
    m=len(me.edges)
    valence=[0]*n

    for i in range(n):
        for j in range(m):
            if (me.edges[j].vertices[0]==i) or (me.edges[j].vertices[1]==i):
                valence[i]+=1


    print("Max : ",max(valence))
    print("Min :",min(valence))
    print("Average :", sum(valence)/n)


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
    ex2(mesh)
    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))
