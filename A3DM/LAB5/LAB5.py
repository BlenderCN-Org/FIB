import bpy
from math import radians
import random


#Translate a shape
def translate(ob,x,y,z):
    ob.location=(x,y,z)

#Rotate a shape
def rotate(ob,x,y,z):
    ob.rotation_euler=(radians(x),radians(y),radians(z))

#Scale a selected shape
def scale(x,y,z):
    bpy.ops.transform.resize(value=(x/2,y/2,z/2))


#Extrude one shape on one axis
def extrude(Scope,axis,val):
    copy=Scope.copy()
    for e in copy:
        if isinstance(e,str):
            type=e
        else:
            if(axis=="X"):
                create(type,e[0],e[1],e[2],e[3],val,e[5],e[6])
            if(axis=="Y"):
                create(type,e[0],e[1],e[2],e[3],e[4],val,e[6])
            if(axis=="Z"):
                create(type,e[0],e[1],e[2],e[3],e[4],e[5],val)

            pop(type,e)




#Create a shape and add it to the scope
def create(type,shape,px,py,pz,sx,sy,sz):
    if(shape=="cube"):
        bpy.ops.mesh.primitive_cube_add()
        ob=bpy.context.object
        ob.name="cube"
        scale(sx,sy,sz)
        translate(ob,px,py,pz)
        push(["cube",px,py,pz,sx,sy,sz],type)

    if(shape=="cylinder"):
        bpy.ops.mesh.primitive_cylinder_add()
        ob=bpy.context.object
        ob.name="cylinder"
        scale(sx,sy,sz)
        translate(ob,px,py,pz)
        push(["cylinder",px,py,pz,sx,sy,sz],type)


#Push an element into the right scope or create a new scope if it doesn't already exist
def push(e,type):
    if (len(Lot) == 0):
        Lot.append([type,e])
    else:
        flag=False
        for i in range(len(Lot)):
            if (Lot[i][0]==type):
                if not(e in Lot[i]):
                    Lot[i].append(e)
                flag=True
        if not(flag):
            Lot.append([type,e])


#Pop out
def pop(type,e):

    if(len(Lot)==0):
        print("Empty Lot")
    else:
        i=0
        flag=True
        while (i<len(Lot) and flag):
        #for i in range(len(Lot)):
            if (Lot[i][0]==type):
                k=1
                while(k<len(Lot[i]) and flag):
                    if(set(e)==set(Lot[i][k])):
                        print(Lot[i].pop(k))
                        flag=False
                    k+=1

            else:
                i+=1

            if(len(Lot[i])==1):
                    Lot.pop(i)

        if flag:
            print("The element is not in the lot")



#L1 = list of dimensions  (int/float for normal dimensions, string "1" for relative ones)
#L2 = list of types (string)
def subdivide(Scope,axis,L1,L2):

    copy=Scope.copy()

    for e in copy:
        if isinstance(e,str):
            type=e
        else:
            #Object data
            px,py,pz = e[1],e[2],e[3]  #Object location
            x,y,z = e[4],e[5],e[6] #Object dimensions
            pop(type,e)  #Delete initial object from scope

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
            if(len(L1)!=len(L2)):
                print("Error on lists' lengths")
                exit()

            #Test if subdivision is possible on this axis
            sum=0
            tstr=0
            for i in L1:
                if(isinstance(i,int) or isinstance(i,float)):
                    sum+=i
                else:
                    tstr+=float(i[0])
            if(sum!=dim and tstr==0):
                print("Error on dimensions' list")
                exit()


            #Create new cubes of each dimension and pile them up
            for i in range(len(L1)):
                if(isinstance(L1[i],float) or isinstance(L1[i],int)):
                    if(axis=="X"):
                        create(L2[i],e[0],px+pile+L1[i]/2,py,pz,L1[i],y,z)
                    if(axis=="Y"):
                        create(L2[i],e[0],px,py+pile+L1[i]/2,pz,x,L1[i],z)
                    if(axis=="Z"):
                        create(L2[i],e[0],px,py,pz+pile+L1[i]/2,x,y,L1[i])
                    pile+=L1[i]
                else:
                    #in case it it a string, we get the value of the first character
                    val=float(L1[i][0])
                    if(axis=="X"):
                        pile_temp = val*(x-sum)/tstr   #We use the formula given in the paper
                        create(L2[i],e[0],px+pile+pile_temp/2,py,pz,pile_temp,y,z)
                    if(axis=="Y"):
                        pile_temp=val*(y-sum)/tstr
                        create(L2[i],e[0],px,py+pile+pile_temp/2,pz,x,pile_temp,z)
                    if(axis=="Z"):
                        pile_temp=val*(z-sum)/tstr
                        create(L2[i],e[0],px,py,pz+pile+pile_temp/2,x,y,pile_temp)
                    pile+=pile_temp



#print actual value of Lot in the console
def print_lot():
    print(Lot)

#display each element of the Lot in blender
def display_lot():
    bpy.ops.object.select_all(action='TOGGLE')
    bpy.ops.object.select_all(action='TOGGLE')
    bpy.ops.object.delete()
    for i in Lot:
        for j in i:
            if (isinstance(j,str)):
                type = j
            else:
                create(type,j[0],j[1],j[2],j[3],j[4],j[5],j[6])



#list of all the scopes = lot
Lot=[['A', ['cube', 0, 0, 0, 4, 3, 0], ['cube', 0, 5, 0, 2, 3, 0]]]


def config1():
    extrude(Lot[0],"Z",7)
    subdivide(Lot[0],"X",[2*round(random.uniform(0.3,0.5),1),"1"],["facades","sideways"])
    display_lot()
    print_lot()
