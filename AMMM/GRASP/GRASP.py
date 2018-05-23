
import random

#example : Solve(0.4, 20, [ 2, 2, 1, 1, 1, 2, 2, 3, 4, 6, 6, 7, 5, 8, 8, 7, 6, 6, 4, 3, 4, 3, 3, 3], 5, 9, 3, 14)

def Solve(alpha, numNurses, demand, minHours, maxHours, maxConsec, maxPresence):
    
    Total_demand=sum(demand)
    Solution=[]
    workingNurses = [[0 for j in range(numNurses)] for i in range(24)]
    transpose = [[0 for j in range(24)] for i in range(numNurses)]
    used=[0 for i in range(numNurses)]




    #------------------------------------------ GRASP Algorithm
    
    while (Total_demand!=0):
        
        
        cset=construct_cs(numNurses, demand, workingNurses, maxHours)
        
        if len(cset) != 0:
            
                     
            #Generating RCL
            qmin=min(cset, key=lambda x: greedy_value(x, demand, workingNurses, used))
            qmax=max(cset, key=lambda x: greedy_value(x, demand, workingNurses, used))
    
            threshold=greedy_value(qmin,demand,workingNurses,used) + alpha*(greedy_value(qmax,demand,workingNurses,used)-greedy_value(qmin,demand,workingNurses,used))
    
            RCL=[]
            for x in cset:
                if (greedy_value(x,demand,workingNurses,used)<=threshold):
                    RCL.append(x)
    
            
            # Pick one randomly
            random_pick=random.randrange(len(RCL))
            candidate=RCL[random_pick]
            
    
            addCandidate(Solution,candidate,workingNurses,maxConsec,maxPresence,used,demand,transpose,minHours,numNurses)
    
            Total_demand=sum(demand)
            print(Total_demand)
            
        
        else:
            print ("GRASP: No more candidates but solution is not complete.")
            print(workingNurses)

        
        
    #------------------------------------------ Display solution
    for n in range(numNurses):
        for h in range(24):
            transpose[n][h] = workingNurses[h][n]

    for n in range(numNurses):
        print(n)
        print(' : ')
        print(transpose[n])

    #print(used)
    #print(workingNurses)





#-------------------------------------------------
# First : Determine starting hour of one nurse
#-------------------------------------------------
  
def First(N,workingNurses):
    first=0

    while (workingNurses[first][N]==0):
        first+=1

    return first


#-------------------------------------------------
# Last : Determine last working hour of one nurse
#-------------------------------------------------

def Last(N,workingNurses):

    last=23
    while (workingNurses[last][N]==0):
        last-=1

    return last

#-------------------------------------------------
# rest : Determine resting hours of one nurse
#-------------------------------------------------

def rest(N,workingNurses):

    first=First(N,workingNurses)
    last=Last(N,workingNurses)

    rest=[0 for i in range(24)]

    i=first+1
    while (i<last):
        if (workingNurses[i][N]==0):
            rest[i]=1
        i+=1

    return rest


#-------------------------------------------------
# miniHours : Check if all nurses are doing at 
# least minHours of working hours
# if not, it gives back the index of the first 
# not fitting nurse, otherwise it returns true
#-------------------------------------------------

def miniHours(numNurses,workingNurses,used,minHours):
    flagHours=True
    n=0
    while ((flagHours) & (n<numNurses)):
        if (used[n]):
            totalHours=0
            for h in range(24):
                totalHours+=workingNurses[h][n]
            if totalHours < minHours:
                flagHours=False
        n+=1
    if (flagHours):
        return flagHours
    else:
        return (n-1)


#-------------------------------------------------
# Resting : same idea as miniHours but check if 
# the nurses respect the not two consecutive 
# rest hour constraint
#-------------------------------------------------

def Resting(numNurses,workingNurses,used):
    flagRest=True
    n=0
    while ((flagRest) & (n<numNurses)):
        if (used[n]):
            Rest=rest(n,workingNurses)
            for h in range(23):
                if (Rest[h]+Rest[h+1]>1):
                    flagRest=False
        n+=1
    if (flagRest):
        return flagRest
    else:
        return (n-1)
    

#-------------------------------------------------
# maxiHours : Get the sum of working hours for
# a nurse
#-------------------------------------------------

def maxiHours(nurse,workingNurses,maxHours):
    somme=0
    for h in range(24):
        somme+=workingNurses[h][nurse]
    return somme



#-------------------------------------------------  // not working //
# is Feasible : to check if a solution is feasible
# we have to check if the nurses are working more
# than minHours. If not, we want to add not
# consecutive working hours

# We wanted to apply this function once the 
# algorithm had found one solution with the other
# constraints implemented through the greedy cost
#-------------------------------------------------

def isFeasible(numNurses,workingNurses,used,minHours,maxPresence,maxConsec,maxHours):
    
    n=miniHours(numNurses,workingNurses,used,minHours)
    while (type(n)==int):
        for h in range(24):
            if (maxPres(h,n,workingNurses,maxPresence,used)):
                if (maxConsecutive(h,n,workingNurses,maxConsec)):
                    if ((workingNurses[h-1][n]==0) & (workingNurses[h][n]==0)):
                        workingNurses[h][n]=1
        n=miniHours(numNurses,workingNurses,used,minHours)
        



#-------------------------------------------------
# construct_cs : Create the candidate list which
# is made of couples (nurse,hour) if a nurse is
# not already working too many hours
#-------------------------------------------------

def construct_cs(numNurses, demand, workingNurses, maxHours):

    cs_n=[]  #candidate list of nurses
    cs_h=[i for i in range(24)]  #candidate list of hours

    cs=[]

    #add all the not used nurses (working more than maxHours)
    for n in range(numNurses):
        total=0
        for h in range(24):
            total+=workingNurses[h][n]
        if total != maxHours :
            cs_n.append(n)


    #all the combinations  "possible"
    for n in cs_n:
        for h in cs_h:
            if (workingNurses[h][n] == 0):
                cs.append((h,n))

    return cs



#-------------------------------------------------
# greedy_value : Calculate the greedy cost of a 
# nurse N at time H  (a candidate)
#-------------------------------------------------

def greedy_value(candidate, demand, workingNurses, used):

    H=candidate[0]
    N=candidate[1]

    #Hour Cost
    HourCost = -demand[H]

    #Working Nurse Cost
    NurseCost =  -1* used[N]    

    #Resting Nurse Cost
    RestingCost=0;
    if (used[N]):
        Rest=rest(N,workingNurses)
        for k in range(23):
            if (((Rest[k]==1) & (Rest[k+1]==1)) | ((Rest[k-1]==1) & (Rest[k]==1))):
                RestingCost=1;

    RestingCost=-100*RestingCost

    return (HourCost + NurseCost + RestingCost)



#-------------------------------------------------
# maxConsecutive : make sure than if a nurse N is 
# working at hour H then they do not work more
# than maxConsec hours
#-------------------------------------------------

def maxConsecutive(H,N,workingNurses,maxConsec):

    flagHours=False
    #maximum consecutive Hours
    consec=0


    j=H+1
    if (j<23):
        while ((j<23) & (workingNurses[j][N]==1)) :
            consec+=1
            j+=1


    i=H-1
    if (i>0):
        while ((i>(-1)) & (workingNurses[i][N]==1)) :
            consec+=1
            i-=1

    
    if (consec<maxConsec):
        flagHours=True
    else:
        flagHours=False


    return (flagHours)



#-------------------------------------------------
# maxPres : make sure than if a nurse N is 
# working at hour H then they do not stay at the
# hospital more than maxPres hours
#-------------------------------------------------


def maxPres(H,N,workingNurses,maxPresence,used):

    flagPresence=True

    #maximum consecutive presence
    if (used[N]):
        first=First(N,workingNurses)
        last=Last(N,workingNurses)

        if H<first :
            if (last-H < maxPresence):
                flagPresence=True
            else:
                flagPresence=False
        if H>last :
            if (H-first < maxPresence):
                flagPresence=True
            else:
                flagPresence=False


    return (flagPresence)



#-------------------------------------------------
# addCandidate : evaluate if a candidate can be 
# added in the Solution set by checking the 
# constraints on consecutive hours worked and
# presence at the hospital as well as the 
# consecutive resting hours
#-------------------------------------------------

def addCandidate(Solution,candidate,workingNurses,maxConsec,maxPresence,used,demand,transpose,minHours,numNurses):

    H=candidate[0]
    N=candidate[1]

    flagHours=False
    flagPresence=True
    flagRestHours=True


    #maximum consecutive Hours
    consec=0


    j=H+1
    if (j<24):
        while ((j<23) & (workingNurses[j][N]==1)) :
            consec+=1
            j+=1


    i=H-1
    if (i>0):
        while ((i>(-1)) & (workingNurses[i][N]==1)) :
            consec+=1
            i-=1

    
    if (consec<maxConsec):
        flagHours=True
    else:
        flagHours=False

        

    #maximum consecutive presence
    if (used[N]):
        first=First(N,workingNurses)
        last=Last(N,workingNurses)

        if H<first :
            if (last-H < maxPresence):
                flagPresence=True
            else:
                flagPresence=False
        if H>last :
            if (H-first < maxPresence):
                flagPresence=True
            else:
                flagPresence=False

    
    #Rest hours
    if (used[N]):
        Rest=rest(N,workingNurses)
        first=First(N,workingNurses)
        last=Last(N,workingNurses)
        if (first != last):
            if H<(first-1):
                flagRestHours=True
            elif H>(last+1):
                flagRestHours=True
            elif H<23:
                if (Rest[H]+Rest[H+1]>1):
                    flagRestHours= True
                if (Rest[H-1]+Rest[H]>1):
                    flagRestHours=True
                else:
                    flagRestHours=False
            

    if ((flagHours) & (flagPresence) & (flagRestHours)):
        workingNurses[H][N]=1
        used[N]=1
        Solution.append((H,N))
        if demand[H]>0:
            demand[H]-=1
    else:
        workingNurses[H][N]=0


