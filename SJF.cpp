/*
	Process  AT  BT  ST  CT  TAT  WT  RT
	   .      .   .   .   .   .    .   .
	   .      .   .   .   .   .    .   .
	   .      .   .   .   .   .    .   .
	   
	TAT =  CT  - AT		( C4 = C3 - C0 )
	WT  =  TAT - BT		( C5 = C4 - C1 )
	RT  =  ST  - AT		( C6 = C2 - C0 )
*/
#include<stdio.h>
#include<stdlib.h>

int minAT(int basic[][2], int noPro){
	int min = INT_MAX;
	for(int i=0; i<noPro; i++){
		if(basic[i][0]<min) min = basic[i][0];
	}
	return min;
}
	
void sjf(int basic[][2], int noPro){
	int rr[noPro][7], time = minAT(basic,noPro), processor, min, at, timeline[noPro], index = 0;
	float avgTAT = 0, avgWT = 0, avgRT = 0;
	bool f = true;
	
	// Initializing table rr
	for(int i=0;i<noPro;i++){
		for(int j=0;j<7;j++){
			if(j==0) rr[i][j] = basic[i][0];
			else if(j==1) rr[i][j] = basic[i][1];
			else rr[i][j] = -1;
		}
	}
		
	// Calculating ST and CT for each user
	while(f){
		// Finding process which is to be scheduled
		min = INT_MAX;
		processor = 0;
		at = 0;
		
		// Choose process
		for(int i=0; i<noPro; i++){ 
			// Find process having shortest burst time which is not scheduled yet
			if(rr[i][0]<=time && rr[i][2]==-1 && rr[i][1]<min){	
				min = basic[i][1];
				at = basic[i][0];
				processor = i;	
			}
			// If more than one process has same shortest BT then find process with min AT
			else if(rr[i][0]<=time && rr[i][2]==-1 && rr[i][1]==min && rr[i][0]<at){ 
				at = basic[i][0];
				processor = i;	
			}
		}
		
		// If CPU is ideal
		if(min == INT_MAX){
			for(int i=0; i<noPro; i++){ 
				// Find process which is next to be scheduled
				if((rr[i][0]>time && min==INT_MAX) || (rr[i][0]<at && rr[i][0]>time)){
					min = basic[i][1];
					at = basic[i][0];
					processor = i;	
				}
				// If one or more process has same AT then schedule that process which has min BT
				else if(min!=INT_MAX && rr[i][0]==at && rr[i][1]<min){
					min = basic[i][1];
					processor = i;	
				}
				if(i==noPro - 1)
					time = at;
			}
		}
		
		rr[processor][2] = time;    // Register ST for the process
		time += min;
		rr[processor][3] = time;   // Register CT for the process
		
		rr[processor][4] = rr[processor][3] - rr[processor][0] ;  // Register TAT for the process
		rr[processor][5] = rr[processor][4] - rr[processor][1] ;  // Register WT for the process
		rr[processor][6] = rr[processor][2] - rr[processor][0] ;  // Register RT for the process
		
		timeline[index++] = processor + 1;  // To maintain timeline
		
		// Checks whether all the processes are scheduled or not
		for(int i=0;i<noPro;i++){
			if(rr[i][2]==-1){
				f = true;
				break;
			}
			else{
				f = false;
			}
		}
	}
	
	// Calculate avgTAT, avgWT, avgRT
	for(int i=0;i<noPro;i++){
		avgTAT += rr[i][4];
		avgWT  += rr[i][5];
		avgRT  += rr[i][6];
	}
	
	avgTAT = avgTAT/noPro ;
	avgWT  = avgWT/noPro ;
	avgRT  = avgRT/noPro ;
		
	// Display the table and Avg TAT, Avg WT, Avg RT 
	printf("\nProcess\tAT\tBT\tST\tCT\tTAT\tWT\tRT\t");
	for(int i=0;i<noPro;i++){
		printf("\n");
		for(int j=0;j<=7;j++){
			if(j==0) printf("P%d\t",i+1) ;
			else printf("%d\t",rr[i][j-1]) ;
		}
	}
	printf("\nAvg TAT = %.2f",avgTAT) ;
	printf("\nAvg WT  = %.2f",avgWT) ;
	printf("\nAvg RT  = %.2f\n",avgRT) ;
	
	for(int i=0;i<noPro;i++)
		printf("P%d\t",timeline[i]);
}

int main(){
	int a[][2] = {{6,1},{5,2},{2,6},{4,1},{2,4},{3,3}};
	sjf(a,6);
	/*int a[][2] = {{1,3},{2,1},{20,4},{10,5},{10,3},{5,3}};
	sjf(a,6);*/
	
	return 0;
}
