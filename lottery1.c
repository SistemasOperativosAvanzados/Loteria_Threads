#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <unistd.h>
#include <setjmp.h>
#include <time.h> 

struct Ticket {
	char name[50];
	long amount;
};

struct Currency {
	char name[50];
	struct Ticket* backingTicket;
	struct Ticket* issuedTicket;
	long activeAmount;
};

struct Thread{
	char name[50];
	long nTickets;
	int nWorks;
	int isWorking;
	int isFinish;
	long quantum;
	int isExp;
	jmp_buf buff;
	float pi;
	int isFirstTime;
	int keep;
	int k;


};
struct Thread* thread;
int threadIndex;
int nThreads;
time_t t;
int last;

clock_t start_t, end_t, total_t=0;

void lottery ();
void readFile(char* fileName);
struct Thread* createThread(int* nWork,int* nTickets,char** names,long quantum,int modeExp,int nThreads);
int myRandom(long max);
int getFirst(struct Thread* thread,int nThreads);
int chooseThread(long ticketNumber);
int NisFinish ();
void calculatePi();
long sumTickets(struct Thread* thread,int nThreads,int oldIndex);
int getThreadIndex();
int findLast();

// float parkmiller(float seed, float start, float end){
// 	float a = (end-start)/2147483947;
// 	float b = start;
// 	float result = fmod((16807 * seed), 2147483947);
// 	return a * result + b; 
 
// }


int main(int argc, char const *argv[])
{
	/* code */
	float seed = 2;
	int max = 5;
	int i = 0;
	// while (i<max){;
	// 	float result = parkmiller(seed,0,19);
	
	// 	printf("%f\n",result);
	// 	i++;
	// }
	// int tickets[6] = {10,2,5,4,3,1};
	// int random = myRandom(25);

    //printf("My random number = %d\n", random);
    //int result = chooseTicket(tickets,random);
   	//printf("winner is %d\n",result);

    readFile("threadFile");

	return 0;
}

void readFile(char* fileName){
	FILE *ifp;
	char *mode = "r";
	int c;
	int modeExp=0;
	nThreads=0;
	int* nTickets;
	int nt=0;
	int* nWork;
	int nw=0;
	int quantum=0;
	char *ptr;
	int quatumIndex=0;
	char** names;
	ifp = fopen(fileName, mode);
	threadIndex =-1;
	last=0;
		if (ifp!=NULL){
		c=fgetc(ifp);
		int wordSize = 1;
		char* word =(char*) calloc(wordSize,sizeof(char));
		int lineNumber = 1;
		while (c!=EOF){
			if (c!=';'){
				word[wordSize-1] = c;
				wordSize++;
				word=(char*)realloc(word,wordSize*sizeof(char));
			}else{
				if (lineNumber==1){
					printf("entre!1\n");
					if (!strcmp(word,"Exp")){
						printf("Exp enabled\n");
						modeExp=1;
					}else{
						printf("Exp disabled\n");
					}
					fgetc(ifp);
					

				}else if(lineNumber==2){
					//printf("entre!2\n");
					nThreads=atoi(word);
					printf("nThreads %d\n", nThreads);
					nTickets = (int*) calloc(nThreads,sizeof(int));
					nWork = (int*) calloc(nThreads,sizeof(int));
					names = calloc(nThreads,sizeof(char*));
					for (int i=0;i<nThreads;i++){
						names[i] = calloc(25,sizeof(char));
					}
					fgetc(ifp);
					quatumIndex=nThreads*2+3;
				
				}else if (lineNumber==quatumIndex){
					quantum = strtol(word,&ptr,10);
					printf("quantum=%d\n", quantum);
				
				}else{
					if (wordSize>1){
							printf("lineNumber %d\n",lineNumber );
							
							if(lineNumber%2!=0){
								
								strcpy(names[nt],strtok(word,"-"));
								char *sub = strtok(NULL,"-");
								//printf("sub= %s\n",sub );
								nTickets[nt]= strtol(sub,&ptr,10);
								printf("tickets[%d]-> %d\n",nt,nTickets[nt]);
								nt++;
							}else{
								
								strtok(word,">");
								char *sub = strtok(NULL,">");
								nWork[nw]= strtol(sub,&ptr,10);
								printf("works[%d]-> %d\n",nw,nWork[nw]);
								nw++;
							}
							
						}else{
							quantum = atoi(word);
						}
						
						fgetc(ifp);
				}
				
				lineNumber++;
				wordSize=1;
				word=(char*)calloc(wordSize,sizeof(char));
			}
   
			c=fgetc(ifp);
		}
	}
	thread=createThread(nWork,nTickets,names,quantum,modeExp, nThreads);
	//long totalTickets = sumTickets(nTickets,nThreads);

	lottery();
	for (int i = 0 ; i < nThreads;i++){
		printf("thread[%d]=%d\n",i,thread[i].isFinish );
	}
}

long sumTickets(struct Thread* thread,int nThreads,int oldIndex){
	if (nThreads>0){
		long sum=0;
		printf("inside sumTickets, oldIndex = %d\n",oldIndex );
		for (int i = 0; i < nThreads;i++){
			
			printf("thread.isFinish=%d, index = %d\n", thread[i].isFinish,i);

			if ((!thread[i].isFinish && i!=oldIndex)){
				sum+=thread[i].nTickets;
				printf("sumtickets %lu\n",sum );
			}
		}
		printf("my total tickets : %lu\n", sum);
		return sum;
	}else{
		printf("There are 0 threads");
		return 0;
	}

}

struct Thread* createThread(int* nWork,int* nTickets,char** names,long quantum,int modeExp,int nThreads){
	struct Thread *threads;
	int size = 1;
	char ptr[10];
	threads = calloc (nThreads,sizeof(struct Thread));  
	if (nThreads>0){
		for (int x= 0; x <nThreads;x++){

			
			strcpy(threads[x].name,names[x]);
			threads[x].nTickets = nTickets[x];
			threads[x].nWorks = 50*nWork[x]; // remember here!!!
			threads[x].isWorking =1;
			threads[x].isFinish=0;
			threads[x].quantum = quantum;
			threads[x].isExp = modeExp;
			threads[x].pi=0;
			threads[x].isFirstTime=1;
			threads[x].keep=1;
			threads[x].k=0;
				
		}
		return threads;
	}
	return NULL;
}
/*
def park_miller(seed,start,end):
    a = (end-start)/2147483947
    b = start
    while True:
        seed = (16807*seed) % 2147483947
        yield  a * seed +b
*/

int myRandom(long max){
	
	printf("long max= %lu\n",max );
	srand((unsigned) time(&t));
	int randn = rand();
	printf("rand->%d\n",randn );
	return randn%max;
}

int getFirst(struct Thread* thread,int nThreads){
	for (int i = 0; i < nThreads;i++){
		if (!thread[i].isFinish){
			return i;
		}
	}
}

int chooseThread(long ticketNumber){
	int ticketPointer = 0;
	//int firstThread = getFirst(thread,nThreads);
	// if (ticketNumber<= thread[firstThread].nTickets){
	// 	return firstThread;
	// }else{
		
	// }

	for (int i = 0 ; i < nThreads;i++){
			printf("Im at index= %d\n",i );
			if ((!thread[i].isFinish && i !=threadIndex)){
				int before = ticketPointer;
				printf("before=%d\n",before );
				ticketPointer+=thread[i].nTickets;
				printf("after %d\n",ticketPointer );
				if (before<=ticketNumber && ticketNumber<=ticketPointer){
					return i;
				}	
			}
			
		}

}


int NisFinish (){
	int count = 0;
	for (int i = 0; i < nThreads;i++){
		if (!thread[i].isFinish){
			count++ ;
		}
	}
	return count;
}

int findLast(){
	for (int i = 0; i < nThreads;i++){
		if (!thread[i].isFinish){
			return i;
		}
	}
}

void calculatePi(){
	float pi = 0;
	
	int r ;
	
	//for (int k = 0 ; k < (thread[threadIndex].nWorks);k++){
	start_t = start_t + clock();
	r = setjmp(thread[threadIndex].buff);
	printf("merong\n");
	printf("nWorks===>%d\n",thread[threadIndex].nWorks );
	printf("k===>%d\n",thread[threadIndex].k);
	while(1){
		if (thread[threadIndex].k< thread[threadIndex].nWorks){
			
			thread[threadIndex].pi = thread[threadIndex].pi + (pow(-1,thread[threadIndex].k ))/(2*thread[threadIndex].k +1);
			printf("im in thread %d\n", threadIndex );
			printf("MY PI==>%.10f\n", thread[threadIndex].pi );
			
			//total_t += (double)(end_t - start_t) / CLOCKS_PER_SEC;
			thread[threadIndex].k =thread[threadIndex].k+1;
			if (thread[threadIndex].isExp){
				//printf("total time= %lu\n", total_t);
				end_t = end_t + clock();
				total_t += (double)(end_t - start_t) / CLOCKS_PER_SEC;
				printf("timer = %lu\n",total_t);
				if (total_t>=thread[threadIndex].quantum ){
					printf("sleeping...\n");
					start_t,total_t,end_t=0;
					//usleep(4000000);
				//if ( threadIndex!=-1){
					printf("r= %d\n",r );
					printf("r index %d\n", threadIndex);
					// if (fromThread==NULL) fromThread=threadIndex
					if (NisFinish()!=1){
						threadIndex = getThreadIndex();
					}

					printf("-------------2threadIndex %d\n", threadIndex);
					
					if (thread[threadIndex].isFirstTime==0){

						//printf("old=%d, new %d\n",threadIndex,threadIndex );
					//	usleep(7000000);
						longjmp(thread[threadIndex].buff,thread[threadIndex].k);
					}else{
						thread[threadIndex].isFirstTime=0;
						calculatePi();
					}
				}		
			}
		}else{
			break;
		}
	}
	 // thread[threadIndex].pi = pi;
	 thread[threadIndex].isFinish=1;
	 printf("thread[%d]=%d\n",threadIndex, thread[threadIndex].isFinish);
	printf("pi for index, %d =%.10f\n",threadIndex,4* thread[threadIndex].pi);
	
	if (NisFinish()>1){
		threadIndex = getThreadIndex();
		printf("entre!!!!\n");
		printf("-------------3threadIndex %d\n", threadIndex);
		if (thread[threadIndex].isFirstTime==0){
			//printf("old out=%d, new out %d\n",index,threadIndex );
			printf("1\n");
			//usleep(100000000);
			longjmp(thread[threadIndex].buff,thread[threadIndex].k );
		}else{
			printf("2\n");
			//usleep(100000000);
			thread[threadIndex].isFirstTime=0;
			calculatePi();
		}
	}else if (NisFinish()==0){
		printf("termine!\n");
	}else{
		printf("~~~~~~~~~~~~~~stop!!\n");
		//last=1;
		threadIndex = findLast();
		printf("findlast= %d\n",threadIndex );
		//threadIndex=-1;
		
		if (thread[threadIndex].isFirstTime==0){
			//printf("old out=%d, new out %d\n",index,threadIndex );
			printf("1\n");
			//usleep(100000000);
			longjmp(thread[threadIndex].buff,thread[threadIndex].k );
		}else{
			printf("2\n");
			//usleep(100000000);
			thread[threadIndex].isFirstTime=0;
			calculatePi();
		}

	}
}

int getThreadIndex(){
	while(1){
		long tt = sumTickets(thread,nThreads,threadIndex);
		printf("totalTickets ====> %lu\n",tt);
		long ticketNumber = myRandom(tt);
		
		printf("----------------ticketNumber %ld\n",ticketNumber );
		return chooseThread(ticketNumber);
	
	}
}

void lottery (){
	if (nThreads>0){
		
		//int threadIndex=-1;
		// int fromThread;
		// while (!isFinish(thread,nThreads)){
			if (threadIndex==-1){

				threadIndex = getThreadIndex();
				printf("------------1threadIndex %d\n", threadIndex);
			}
			//thread[threadIndex].isFinish = 1;
			
			thread[threadIndex].isFirstTime=0;
			calculatePi();
			// float pi = 0;
			// int r;
			// r = setjmp(thread[threadIndex].buff);
			// while (r<50*thread[threadIndex].nWorks){
			// 	pi = pi + (pow(-1,r))/(2*r+1);
			// 	if (r>=thread[threadIndex].nWorks*50*0.1){
			// 		threadIndex = getThreadIndex(thread,nThreads,threadIndex);
			// 		printf("-------------2threadIndex %d\n", threadIndex);
			// 		if (thread[threadIndex].isFirstTime==0){
			// 			//printf("old=%d, new %d\n",index,threadIndex );
			// 			longjmp(thread[threadIndex].buff,r);
			// 		}else{
			// 			thread[threadIndex].isFirstTime=0;
			// 			r =0;
			// 		}
			// 	}
			// } 
			
		
		printf("me sali del while loop\n");
			
	}
}