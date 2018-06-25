import bpy
from time import time


def neighbors(me):   #compute the list of neighboring faces for each edge

    n=len(me.edges)
    neighbors=[[] for i in range(n)]
    m=len(me.polygons)

    for edge in range(n): #To have a linear complexity, we should do the outer loop on faces but we don't access edges indexes from the faces in Blender
        for poly in me.polygons:
            vert=me.edges[edge].vertices
            if ((vert[0] in poly.vertices) and (vert[1] in poly.vertices)):
                neighbors[edge].append(poly)

    return(neighbors)

def union_find(lis):    #Union find function on lists
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


def ex5(me):

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

    #We look for loops in the list of edges with the union find function
    edge_list=union_find(edge_list)
    shells=len(edge_list)   #Compute the number of shells obtained from all 2-manifold edges

    #If a shell contains non-manifold edges, we don't consider it as a shell
    if len(non_manifold_list)!=0:
        for e in edge_list:
            flag=False
            for nm in non_manifold_list:
                if nm in e:
                    flag=True
            if flag:
                shells-=1

    return(shells)


def ex6(me):

    edges=len(me.edges)
    faces=len(me.polygons)
    vertices=len(me.vertices)
    shells=ex5(me)

    print("Genus =",(1/2)*(edges-faces-vertices)+shells)


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
    ex6(mesh)
    # Report performance...
    print("Script took %6.2f secs.\n\n"%(time()-t))
