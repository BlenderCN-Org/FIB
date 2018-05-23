/*********************************************

 * OPL 12.5.1.0 Model
 * Author: emeline.got
 * Creation Date: 16/10/2017 at 10:18:44
 *********************************************/

//Data

 int numNurses = ...;
 int hours = ...;
 range N = 1..numNurses;
 range H = 1..hours;
 
 int demand [h in H]= ...;
 int minHours = ...;
 int maxHours = ...;
 int maxConsec = ...;
 int maxPresence = ...;

//Decision variables
dvar boolean working_hn[h in H, n in N];
dvar boolean used[n in N];
dvar boolean workingBf_hn[h in H, n in N];
dvar boolean workingAf_hn[h in H, n in N];
dvar boolean resting_hn[h in H, n in N ];
//Objective

minimize sum(n in N) used[n];

subject to {

   //Working Before Matrix 
   forall (n in N, h in H)
    workingBf_hn[h,n]*24 >= sum(c in H: c < h) working_hn[c,n];
   forall (n in N, h in H)
    workingBf_hn[h,n] <= sum(c in H: c < h) working_hn[c,n];	
	//Working After Matrix
   forall (n in N, h in H)
    workingAf_hn[h,n]*24 >= sum(c in H: c > h) working_hn[c,n];
   forall (n in N, h in H)
    workingAf_hn[h,n] <= sum(c in H: c > h) working_hn[c,n];
    //Rest Matrix	
   forall(n in N, h in H)
     resting_hn[h,n] + working_hn[h,n] + 1 >= workingAf_hn[h,n] + workingBf_hn[h,n];
   forall(n in N, h in H)
     resting_hn[h,n] <= workingBf_hn[h,n];
   forall(n in N, h in H)
     resting_hn[h,n] <= workingAf_hn[h,n];
  
  //Constraint 0
  //defining used or not used nurse
  forall (n in N)
    used[n] >= (1/24)*sum(h in H) working_hn[h,n];
  
  //Constraint 1
  //respect the demand per hour
  forall(h in H)
    sum(n in N) working_hn[h,n] >= demand[h]; 
    
  //Constraint 2
  //more then minimum hours
  forall(n in N)
    sum(h in H) working_hn[h,n] >= minHours*used[n]; 
    
  //Constraint 3
  //less the maximum hours
  forall(n in N)
    sum(h in H) working_hn[h,n] <= maxHours; 
    	
  //Constraint 4
  //working less then max consec
  forall( n in N, h in H)
	sum(c in (h..(h+maxConsec)) : (h + maxConsec <= hours))working_hn[c][n] <= maxConsec;

  //Constraint 5
  //max presence
  forall( n in N, h in H)
	sum(c in ((h+maxPresence)..hours) )working_hn[c][n]<=(1-working_hn[h][n])* maxHours;

  //Constraint 6
  //no consecutive resting hours

  forall(n in N, h in H : h<24)
    resting_hn[h,n] + resting_hn[h+1,n] <= 1;
  
    



     
     
  	
  
 
}







