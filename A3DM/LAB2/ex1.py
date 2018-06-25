import bpy
from time import time

def ex1(me):
    x=0
    y=0
    z=0
    n=len(me.vertices)
    for i in range(n):
        coord=me.vertices[i].co
        x+=coord[0]
        y+=coord[1]
        z+=coord[2]
    print("Centroid :",x/n,y/n,z/n)

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
    ex1(mesh)
    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))
