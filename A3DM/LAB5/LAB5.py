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


#Extrude all the scopes in one category in one direction
def extrude(Scope_name,axis,val):

    for Scope in Lot:
        if Scope[0]==Scope_name:
            copy=Scope.copy()

    for e in copy:
        if isinstance(e,str):
            type=e
        else:
            extrude_obj(type,e,axis,val)



#Extrude one object in one direction
def extrude_obj(type,e,axis,val):
    if(axis=="X"):
        create(type,e[0],e[1]+val/2-e[4]/2,e[2],e[3],val,e[5],e[6])
    if(axis=="Y"):
        create(type,e[0],e[1],e[2]+val/2-e[5]/2,e[3],e[4],val,e[6])
    if(axis=="Z"):
        create(type,e[0],e[1],e[2],e[3]+val/2-e[6]/2,e[4],e[5],val)

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



#Create and rotate a shape and add it to the scope
def createandrotate(type,shape,px,py,pz,sx,sy,sz,rx,ry,rz):
    if(shape=="cube"):
        bpy.ops.mesh.primitive_cube_add()
        ob=bpy.context.object
        ob.name="cube"
        scale(sx,sy,sz)
        rotate(ob,rx,ry,rz)
        translate(ob,px,py,pz)
        push(["cube",px,py,pz,sx,sy,sz,rx,ry,rz],type)

    if(shape=="cylinder"):
        bpy.ops.mesh.primitive_cylinder_add()
        ob=bpy.context.object
        ob.name="cylinder"
        scale(sx,sy,sz)
        rotate(rx,ry,rz)
        translate(ob,px,py,pz)
        push(["cylinder",px,py,pz,sx,sy,sz,rx,ry,rz],type)



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


#Pop out one element
def pop(type,e):

    if(len(Lot)==0):
        print("Empty Lot")

    else:
        i=0
        flag=True
        while (i<len(Lot) and flag):
            if (Lot[i][0]==type):
                k=1
                while(k<len(Lot[i]) and flag):
                    if(set(e)==set(Lot[i][k])):
                        Lot[i].pop(k)
                        flag=False
                    k+=1

            else:
                i+=1

            if(len(Lot[i])==1):
                    Lot.pop(i)

        if flag:
            print("The element is not in the lot")


#Pop out one category
def pop_scope(type):

    if(len(Lot)==0):
        print("Empty Lot")

    else:
        i=0
        flag=True
        while (i<len(Lot) and flag):
            if (Lot[i][0]==type):
                Lot.pop(i)
                flag=False
            else:
                i+=1

        if flag:
            print("Category not in Lot")


#L1 = list of dimensions  (int/float for normal dimensions, string "1" for relative ones)
#L2 = list of types (string)
#Subdivide all the elements in one category
def subdivide(Scope_name,axis,L1,L2):

    for Scope in Lot:
        if Scope[0]==Scope_name:
            copy=Scope.copy()

    for e in copy:
        if isinstance(e,str):
            type=e
        else:
            subdivide_obj(type,e,axis,L1,L2)


#L1 = list of dimensions  (int/float for normal dimensions, string "1" for relative ones)
#L2 = list of types (string)
#Subdivide one single object
def subdivide_obj(type,e,axis,L1,L2):

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
                if(len(j)==7):
                    create(type,j[0],j[1],j[2],j[3],j[4],j[5],j[6])
                else:
                    createandrotate(type,j[0],j[1],j[2],j[3],j[4],j[5],j[6],j[7],j[8],j[9])


#Give the index of the desired category in Lot
def find_id(str):
    for i in range(len(Lot)):
        if Lot[i][0]==str:
            return i



#-------------------------------------------------------------------------------
#------------------------------ Configurations ---------------------------------
#-------------------------------------------------------------------------------



#List of all the scopes = lot
Lot=[['A', ['cube', 0, 0, 0, 4, 3, 0], ['cube', 0, 5, 0, 2, 3, 0]]]   #Config1



def config1():

    #Rule 1
    Scope=Lot[find_id("A")].copy()
    for i in Scope:
        if not(isinstance(i,str)):
            extrude_obj("A",i,"Z",random.randint(3,5))

    Scope=Lot[find_id("A")].copy()
    for i in Scope:
        if not(isinstance(i,str)):
            subdivide_obj("A",i,"X",[i[4]*round(random.uniform(0.3,0.5),1),"1"],["facades","sidewings"])


    #Rule 2
    Scope=Lot[find_id("sidewings")].copy()
    for i in Scope:
        if not(isinstance(i,str)):
            subdivide_obj("sidewings",i,"Y",[i[5]*round(random.uniform(0.2,0.6),1),"1"],["sidewings","epsilon"])

    for i in Scope:
        if not(isinstance(i,str)):
            push(i,"sidewings")
            subdivide_obj("sidewings",i,"Y",["1",i[5]*round(random.uniform(0.2,0.6),1)],["epsilon","sidewings"])


    #Rule 3
    Scope=Lot[find_id("sidewings")].copy()
    for i in Scope:
        if not(isinstance(i,str)):
            R = random.randint(1,100)

            if (R<=50):
                extrude_obj("sidewings",i,"X",i[4]*round(random.uniform(0.4,1.0),1))

            if(50<R and R<=80):
                extrude_obj("sidewings",i,"Z",i[6]*round(random.uniform(0.2,0.9),1))

            if (R>80):
                pop("sidewings",i)
                push(Lot[find_id("epsilon")][random.randint(1,len(Lot[find_id("epsilon")])-1)],"sidewings")

    pop_scope("epsilon")

    #Display & print Lot
    display_lot()
    print_lot()




def config2():

    create("B","cylinder",-4,-2,0,1,1,5)
    subdivide("B","Z",[2,3],["C","D"])
    extrude("A","Z",2)

    display_lot()
    print_lot()




def config3():

    pop_scope("A")

    createandrotate("R","cube",0,0,1.5,2.3,2.3,3,0,0,45)

    create("A","cube",0,0,0,2,2,0)

    create("B","cylinder",0.4,-1,0,0.4,0.4,0)
    create("B","cylinder",-0.4,-1,0,0.4,0.4,0)
    create("B","cylinder",0.4,1,0,0.4,0.4,0)
    create("B","cylinder",-0.4,1,0,0.4,0.4,0)

    create("B","cylinder",-1,0.4,0,0.4,0.4,0)
    create("B","cylinder",-1,-0.4,0,0.4,0.4,0)
    create("B","cylinder",1,0.4,0,0.4,0.4,0)
    create("B","cylinder",1,-0.4,0,0.4,0.4,0)

    create("C","cube",0,0,0,1,1,0)
    create("D","cube",0,0,0,0.5,0.5,0)

    extrude("A","Z",4)
    extrude("B","Z",4)
    extrude("C","Z",6)
    extrude("D","Z",7)

    display_lot()


    print_lot()
