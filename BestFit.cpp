/*
		pro[] => Array of process size			pro_size => No. of processes
		mem[] => Array of memory(block) size	mem_size => No. of memory(block)
		
		pro_mem:
			Process size		Block no.
				.					.	
				.					.
				.					.
		
		mem_pro:
			Memory(Block) size		Process size		IF(Memory size-Process size)
					.					.							.
					.					.							.
					.					.							.
		
		Waiting Processes: Such processes for which "Block no. = -1" in pro_mem
		Remaining Blocks: Such blocks for which "Process size = -1" in mem_pro
		
		sum_wait_pro = sum(Waiting Processes)	
		sum_rem_block = sum(Remaining Blocks)	
		total_if = sum(IF)	
		(ef = sum_rem_block - sum_wait_pro) >= 0
*/

#include<iostream>
#include<cstdio>
#include<cstdlib>

using namespace std;

void firstFit(int pro[],int pro_size,int mem[],int mem_size){
	int pro_mem[pro_size][2],mem_pro[mem_size][3],sum_wait_pro=0,sum_rem_block=0,total_if=0,ef=0,min,index; 
	bool assign;
	
	//Intializing process-memory and memory-process arrays(tables)
	for(int i=0;i<pro_size;i++){
		pro_mem[i][0]=pro[i];pro_mem[i][1]=-1;
	}
	for(int i=0;i<mem_size;i++){
		mem_pro[i][0]=mem[i];mem_pro[i][1]=-1;mem_pro[i][2]=-1;
	}
	
	//Mapping process with memory
	for(int i=0;i<pro_size;i++){
		min=INT_MAX;
		assign=false;
		for(int j=0;j<mem_size;j++){
			if(mem_pro[j][1]==-1 && pro[i]<=mem[j] && min>mem[j]){
				min=mem[j];
				index=j;
				assign=true;
			}
		}
		if(assign){
			pro_mem[i][1]=index;
			mem_pro[index][1]=pro[i];
			mem_pro[index][2]=mem[index]-pro[i];
		}
	}
	
	for(int i=0;i<pro_size;i++){
		if(pro_mem[i][1]!=-1)
			printf("\nProcess: %d\tBlock: %d",pro_mem[i][0],pro_mem[i][1]);
	}
	
	//Finding waiting processes
	printf("\nWaiting Processes: ");
	for(int i=0;i<pro_size;i++){
		if(pro_mem[i][1]==-1){
			printf("%d\t",pro_mem[i][0]);
			sum_wait_pro+=pro_mem[i][0];
		}
	}
	
	//Finding total IF
	for(int i=0;i<mem_size;i++){
		if(mem_pro[i][2]!=-1){
			printf("\nBlock: %d\tIF: %d",i,mem_pro[i][2]);
			total_if+=mem_pro[i][2];
		}
		else if(mem_pro[i][1]==-1)
			sum_rem_block+=mem_pro[i][0];
	}
	printf("\nTotal IF: %d",total_if);
	
	//Finding EF if it exists
	if(sum_rem_block>=sum_wait_pro)
		ef=sum_rem_block-sum_wait_pro;
	printf("\nEF: %d",ef);
}

int main(){
	int a[]={2,34,56,78,10,20};
	int b[]={3,69,45,12,80,12,8,9,10};

	firstFit(a,sizeof(a)/sizeof(int),b,sizeof(b)/sizeof(int));
	return 0;
}
