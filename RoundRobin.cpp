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

class Queue{
	private:
	
	class Node{
		public:
		
		Node *prev;
		int data;
		Node *next;
	};
	
	Node *head;
	Node *tail;
	
	Node *create(int n){
		Node *x = (Node *)malloc(sizeof(Node));
		x->data = n;
		x->prev = NULL;
		x->next = NULL;
		return x;
	}
		
	public:
	
	Queue(): head(NULL), tail(NULL) {}
	
	void dequeue(){
		if(head!=NULL && head->next!=NULL){
			Node *temp;
			head->next->prev = NULL;
			temp = head->next;
			head->next = NULL;
			head = temp;
		}
		else if(head!=NULL && head->next==NULL)
			head = tail = NULL;
		else
			printf("\nEmpty List");
	}
	
	void enqueue(int n){
		Node *x = create(n);
		if(head!=NULL){
			tail->next = x;
			x->prev = tail;
			tail = x;
		}
		else
			head = tail = x;
	}
	
	int getProcess(){
		return head->data;
	}
	
	bool isEmpty(){
		return head == NULL;
	}
	
	void display(){
		Node *travs = head;
		printf("\n");
		while(travs!=NULL){
			if(travs == tail || travs->data != travs->next->data)
				printf("P%d\t",travs->data);
			travs = travs->next;
		}
	}
};

// Find min AT
int minAT(int basic[][2], int noPro){
	int min = INT_MAX;
	for(int i=0; i<noPro; i++){
		if(basic[i][0]<min) min = basic[i][0];
	}
	return min;
}

// Find max AT
int maxAT(int basic[][2], int noPro){
	int max = INT_MIN;
	for(int i=0; i<noPro; i++){
		if(basic[i][0]>max) max = basic[i][0];	
	}
	return max;
}

// Adds process if it arrives between old time(t1) and new time(t2)
void proArr(int oldTime, int newTime, int basic[][2], int noPro, Queue *q, Queue *tl){
	if(oldTime <= maxAT(basic,noPro)){
		for(int time=oldTime+1;time<=newTime;time++){
			for(int i=0;i<noPro;i++){
				if(time==basic[i][0]){
					q->enqueue(i);
					tl->enqueue(i+1);
				} 
			}
		}
	}
}

// Jumps to the next process if CPU is idle
void nextPro(int rr[][7], int noPro, int *time, Queue *q, Queue *tl){
	bool f = false;
	int arrTime = INT_MAX , index;
	for(int i=0;i<noPro;i++){
		if(rr[i][2] == -1 && rr[i][0] < arrTime){
			index = i;
			f = true;
			arrTime = rr[i][0];
		}
	}
	if(f){
		q->enqueue(index);
		tl->enqueue(index+1);
		*time += (arrTime - *time);
	}
}

void roundRobin(int basic[][2], int noPro, const int tq){
	int rr[noPro][7], time = minAT(basic,noPro), oldTime;
	float avgTAT = 0, avgWT = 0, avgRT = 0;
	bool f = true;
	Queue q, timeline;
	
	// Initializing table rr
	for(int i=0;i<noPro;i++){
		for(int j=0;j<7;j++){
			if(j==0) rr[i][j] = basic[i][0];
			else if(j==1) rr[i][j] = basic[i][1];
			else rr[i][j] = -1;
		}
	}
		
	// Check whether a new process has arrived or not
	if(time <= maxAT(basic,noPro)){
		for(int i=0;i<noPro;i++){
			if(time==basic[i][0]){
				q.enqueue(i);
				timeline.enqueue(i+1);
			} 
		}
	}
	
	// Calculating ST and CT for each user
	while(f){
		// Register ST for the process
		if(rr[q.getProcess()][2] == -1){
			rr[q.getProcess()][2] = time;	
		}
		
		// Check whether the process has completed or not
		if(basic[q.getProcess()][1] <= tq){
			proArr(time, time + basic[q.getProcess()][1], basic, noPro, &q, &timeline);  // Checks if any process has arrived btwn these two times
			oldTime = time;
			time += basic[q.getProcess()][1];
			rr[q.getProcess()][3] = time;      // Register CT for the process
			basic[q.getProcess()][1] = 0;
				
			rr[q.getProcess()][4] = rr[q.getProcess()][3] - rr[q.getProcess()][0] ;  // Register TAT for the process
			rr[q.getProcess()][5] = rr[q.getProcess()][4] - rr[q.getProcess()][1] ;  // Register WT for the process
			rr[q.getProcess()][6] = rr[q.getProcess()][2] - rr[q.getProcess()][0] ;  // Register RT for the process
			q.dequeue();
		}
		else{
			proArr(time, time + tq, basic, noPro, &q, &timeline);  // Checks if any process has arrived btwn these two times
			time += tq;
			basic[q.getProcess()][1] -= tq;
			q.enqueue(q.getProcess());
			timeline.enqueue(q.getProcess()+1);
			q.dequeue();
		}
		
		// If CPU is idle aftersome time then jump to the next process	
		if(q.isEmpty()){
			time = oldTime;
			nextPro(rr,noPro,&time,&q,&timeline);
		}
		
		// Checks whether all the processes are scheduled or not
		for(int i=0;i<noPro;i++){
			if(basic[i][1]!=0){
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
	printf("\nAvg RT  = %.2f",avgRT) ;
	
	timeline.display();
}

int main(){
	/*int a[][2]={{1,2},{5,1},{4,7}};
	roundRobin(a,3,2);*/
	/*int a[][2]={{1,2},{15,1},{4,7}};
	roundRobin(a,3,2);*/
	/*int a[][2]={{1,2},{5,1},{4,7},{3,5},{4,5}};
	roundRobin(a,5,2);*/
	int a[][2]={{6,3},{7,6},{3,9},{4,4},{2,5},{5,2}};
	roundRobin(a,6,2);
	
	return 0;
}
