import bpy
from math import radians


Shapes = ["cube","cylinder"]
Lot=[]

#ob=bpy.context.object
def translate(ob,x,y,z):
    ob.location=(x,y,z)

def rotate(ob,x,y,z):
    ob.rotation_euler=(radians(x),radians(y),radians(z))

def scale(x,y,z):
    bpy.ops.transform.resize(value=(x/2,y/2,z/2))


def extrude(ob,axis,val):
    if(axis=="X"):
        ob.scale=(val,0,0)
    if(axis=="Y"):
        ob.scale=(0,val,0)
    if(axis=="Z"):
        ob.scale=(0,0,val)


def create(shape,px,py,pz,sx,sy,sz):
    if(shape=="cube"):
        bpy.ops.mesh.primitive_cube_add()
        ob=bpy.context.object
        ob.name="cube"
        scale(sx,sy,sz)
        translate(ob,px,py,pz)

    if(shape=="cylinder"):
        bpy.ops.mesh.primitive_cylinder_add()
        ob=bpy.context.object
        ob.name="cylinder"
        scale(sx,sy,sz)
        translate(ob,px,py,pz)


#L1 = list of dimensions  (int/float for normal dimensions, string "1" for relative ones)
def subdivide_cube(ob ,axis,L1):
    #Object data
    x,y,z = ob.dimensions
    px,py,pz = ob.location
    bpy.ops.object.delete()

    #Get the axis for subdivision
    if(axis=="X"):
        dim=x
        pile=-x/2
    if(axis=="Y"):
        dim=y
        pile=-y/2
    if(axis=="Z"):
        dim=z
        pile=-z/2

    #Test if len(L1) = len(L2)
    # if(len(L1)!=len(L2)):
    #     print("Error on lists' lengths")
    #     exit()

    #Test if subdivision is possible on this axis
    sum=0
    str=0
    for i in L1:
        if(isinstance(i,int) or isinstance(i,float)):
            sum+=i
        else:
            str+=float(i[0])
    if(sum!=dim and str==0):
        print("Error on dimensions' list")
        exit()


    #Create new cubes of each dimension and pile them up
    for i in range(len(L1)):
        if(isinstance(L1[i],float) or isinstance(L1[i],int)):
            if(axis=="X"):
                create("cube",px+pile+L1[i]/2,py,pz,L1[i],y,z)
            if(axis=="Y"):
                create("cube",px,py+pile+L1[i]/2,pz,x,L1[i],z)
            if(axis=="Z"):
                create("cube",px,py,pz+pile+L1[i]/2,x,y,L1[i])
            pile+=L1[i]
        else:
            #in case it it a string, we get the value of the first character
            val=float(L1[i][0])
            if(axis=="X"):
                pile_temp = val*(x-sum)/str   #We use the formula given in the paper
                create("cube",px+pile+pile_temp/2,py,pz,pile_temp,y,z)
            if(axis=="Y"):
                pile_temp=val*(y-sum)/str
                create("cube",px,py+pile+pile_temp/2,pz,x,pile_temp,z)
            if(axis=="Z"):
                pile_temp=val*(z-sum)/str
                create("cube",px,py,pz+pile+pile_temp/2,x,y,pile_temp)
            pile+=pile_temp


#L1 = list of dimensions  (int/float for normal dimensions, string "1" for relative ones)
def subdivide_cylinder(ob ,axis,L1):
    #Object data
    x,y,z = ob.dimensions
    px,py,pz = ob.location
    bpy.ops.object.delete()

    #Get the axis for subdivision
    if(axis=="X"):
        dim=x
        pile=-x/2
    if(axis=="Y"):
        dim=y
        pile=-y/2
    if(axis=="Z"):
        dim=z
        pile=-z/2

    #Test if len(L1) = len(L2)
    # if(len(L1)!=len(L2)):
    #     print("Error on lists' lengths")
    #     exit()

    #Test if subdivision is possible on this axis
    sum=0
    str=0
    for i in L1:
        if(isinstance(i,int) or isinstance(i,float)):
            sum+=i
        else:
            str+=float(i[0])
    if(sum!=dim and str==0):
        print("Error on dimensions' list")
        exit()


    #Create new cubes of each dimension and pile them up
    for i in range(len(L1)):
        if(isinstance(L1[i],float) or isinstance(L1[i],int)):
            if(axis=="X"):
                create("cylinder",px+pile+L1[i]/2,py,pz,L1[i],y,z)
            if(axis=="Y"):
                create("cylinder",px,py+pile+L1[i]/2,pz,x,L1[i],z)
            if(axis=="Z"):
                create("cylinder",px,py,pz+pile+L1[i]/2,x,y,L1[i])
            pile+=L1[i]
        else:
            #in case it it a string, we get the value of the first character
            val=float(L1[i][0])
            if(axis=="X"):
                pile_temp = val*(x-sum)/str   #We use the formula given in the paper
                create("cylinder",px+pile+pile_temp/2,py,pz,pile_temp,y,z)
            if(axis=="Y"):
                pile_temp=val*(y-sum)/str
                create("cylinder",px,py+pile+pile_temp/2,pz,x,pile_temp,z)
            if(axis=="Z"):
                pile_temp=val*(z-sum)/str
                create("cylinder",px,py,pz+pile+pile_temp/2,x,y,pile_temp)
            pile+=pile_temp
