import numpy as np
import math, sys

def getChromosomeLength(data):
    return int(data["numNurses"])*24

def decode(population, data):
    for ind in population:
        solution, fitness=decoder_assignment(data,ind['chr'])
        ind['solution']=solution
        ind['fitness']=fitness
    return(population)



def test(data,working):

    Demand=list(map(float,data["Demand"][:]))
    numNurses=int(data["numNurses"])
    minHours=int(data["minHours"])
    maxHours=int(data["maxHours"])
    maxConsec=int(data["maxConsec"])
    maxPresence=int(data["maxPresence"])

    #--------------------------------- Number of nurses working
    used=[0]*numNurses

    #--------------------------------- Demand
    flagDemand=True

    for i in range(24):
        somme=0
        for j in range(numNurses):
            somme+=working[j][i]

        if (somme==Demand[i]):
            flagDemand=True
        else:
            flagDemand=False

    #--------------------------------- minHours & maxHours
    flagminHours=True
    flagmaxHours=True

    for i in range(numNurses):
        somme=0
        for j in range(24):
            somme+=working[i][j]

        if (somme>0):
            used[i]=1

        if ((used[i]==1) & (somme>minHours)):
            flagminHours=True
        else:
            flagminHours=False

        if (somme<maxHours):
            flagmaxHours=True
        else:
            flagmaxHours=False


    #--------------------------------- maxConsec
    flagmaxConsec=True

    for i in range(numNurses):
        h=0
        consec=0
        while (h<24):
            if (working[i][h]==1):
                h+=1
                consec+=1

            if (consec<=maxConsec):
                flagmaxConsec=True
            else:
                flagmaxConsec=False

            h+=1




    #--------------------------------- maxPresence
    flagmaxPresence=True

    for i in range(numNurses):
        if (used[i]==1) :
            first=First(i,working)
            last=Last(i,working)

            if ((last-first)<maxPresence):
                flagmaxPresence=True
            else:
                flagmaxPresence=False



    #--------------------------------- maxResting
    flagmaxResting=True

    for i in range(numNurses):
        if (used[i]==1):
            first=First(i,working)
            last=Last(i,working)
            for h in range(first,last):
                if (working[i][h]+working[i][h]>=1):
                    flagmaxResting=True
                else:
                    flagmaxResting=False



    Flag=((flagDemand) & (flagminHours) & (flagmaxHours) & (flagmaxConsec) & (flagmaxPresence) & (flagmaxResting))


    return used, Flag



def First(n,working):
    first=0
    while (working[n][first]==0):
        first+=1
    return first


def Last(n,working):
    last=23
    while (working[n][last]==0):
        last-=1
    return last







def decoder_assignment(data,chromosome):

 
    numNurses=int(data["numNurses"])


    working=np.zeros((numNurses,24))
    solution=[None]*(getChromosomeLength(data))


    for i in range(getChromosomeLength(data)):

        n=i//24
        h=i%24

        if chromosome[i]>=0.5:
            working[n][h]=1
        else:
            working[n][h]=0
        
        solution[i]=working[n][h]

        
    #CONSTRAINTS
    used,Flag=test(data,working)
    
        
    if (Flag):
        fitness=sum(used)
    else:
        fitness=99999999




    return solution, fitness

