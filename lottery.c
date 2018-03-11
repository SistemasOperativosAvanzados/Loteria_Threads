#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <unistd.h>
#include <setjmp.h>
#include <time.h>
#include <gtk/gtk.h>

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
	long pi;
	int isFirstTime;
};

//Declaration functions
void lottery (struct Thread* thread,int nThreads,int isExp, GtkWidget *grid);
void readFile(char* fileName, GtkWidget *grid);
struct Thread* createThread(int* nWork,int* nTickets,char** names,long quantum,int modeExp,int nThreads);
int myRandom(long max);
int getFirst(struct Thread* thread,int nThreads);
int chooseThread(struct Thread* thread,int nThreads, long ticketNumber,int oldIndex);
int isFinish (struct Thread* thread,int nThreads);
void calculatePi(struct Thread* thread,int index,int nThreads, int isExp);
long sumTickets(struct Thread* thread,int nThreads,int oldIndex);
int getThreadIndex(struct Thread* thread,int nThreads,int oldIndex);
void setScreen(float percentaje, int tid, GtkWidget *grid, int totalThreads, struct Thread* thread);


//Declare variables
static GtkWidget *title;
static GtkWidget *lblThread;
static GtkWidget *pgbThread;

// float parkmiller(float seed, float start, float end){
// 	float a = (end-start)/2147483947;
// 	float b = start;
// 	float result = fmod((16807 * seed), 2147483947);
// 	return a * result + b; 
 
// }


int main(int argc, char **argv)
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
    
    //Se crea e inicializa la parte grafica
    GtkWidget *window, *grid;
    gtk_init(&argc, &argv);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    grid = gtk_grid_new();
    gtk_window_set_title (GTK_WINDOW(window), "Scheduler");
    //gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
    gtk_container_add(GTK_CONTAINER(window), grid);

    readFile("threadFile", grid);

    
    gtk_widget_show_all(window);
    gtk_main();
    
	return 0;
}

void readFile(char* fileName, GtkWidget *grid){
	FILE *ifp;
	struct Thread *thread;
	char *mode = "r";
	int c;
	int modeExp=0;
	int nThreads=0;
	int* nTickets;
	int nt=0;
	int* nWork;
	int nw=0;
	int quantum=0;
	char *ptr;
	int quatumIndex=0;
	char** names;
	ifp = fopen(fileName, mode);
	
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

	lottery(thread,nThreads,modeExp, grid);
	for (int i = 0 ; i < nThreads;i++){
		printf("thread[%d]=%d\n",i,thread[i].isFinish );
	}
}

long sumTickets(struct Thread* thread,int nThreads,int oldIndex){
	if (nThreads>0){
		long sum=0;
		printf("inside sumTickets, oldIndex = %d\n",oldIndex );
		for (int i = 0; i < nThreads;i++){
			printf("thread.isFinish=%d\n", thread[i].isFinish);
			if (!thread[i].isFinish||i!=oldIndex){
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
			threads[x].nWorks = 50*nWork[x];
			threads[x].isWorking =1;
			threads[x].isFinish=0;
			threads[x].quantum = quantum;
			threads[x].isExp = modeExp;
			threads[x].pi=0;
			threads[x].isFirstTime=1;
				
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
	time_t t;
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

int chooseThread(struct Thread* thread,int nThreads, long ticketNumber,int oldIndex){
	int ticketPointer = 0;
	int firstThread = getFirst(thread,nThreads);
	// if (ticketNumber<= thread[firstThread].nTickets){
	// 	return firstThread;
	// }else{
		
	// }

	for (int i = 0 ; i < nThreads;i++){
			//
			if (!thread[i].isFinish||i !=oldIndex){
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


int isFinish (struct Thread* thread,int nThreads){
	for (int i = 0; i < nThreads;i++){
		if (!thread[i].isFinish){
			return 0 ;
		}
	}
	return 1;
}


void calculatePi(struct Thread* thread,int index,int nThreads, int isExp){
	float pi = 0;
	clock_t start_t, end_t, total_t=0;
	int r ;
	for (int k = 0 ; k < (thread[index].nWorks);k++){
		start_t = clock();
		pi = pi + (pow(-1,k))/(2*k+1);
		end_t = clock();
		total_t += (double)(end_t - start_t) / CLOCKS_PER_SEC;
		if (isExp){
			//printf("total time= %lu\n", total_t);
			if (total_t>=thread[index].quantum){

				r = setjmp(thread[index].buff);
				printf("r= %d\n",r );
				if (r==0){
					
					// if (fromThread==NULL) fromThread=threadIndex

					int threadIndex = getThreadIndex(thread,nThreads,index);
					printf("-------------threadIndex %d\n", threadIndex);
					if (thread[threadIndex].isFirstTime==0){
						printf("old=%d, new %d\n",index,threadIndex );
						longjmp(thread[threadIndex].buff,1000+k);
					}else{
						thread[threadIndex].isFirstTime=0;
						calculatePi(thread,threadIndex,nThreads,isExp);
					}
				}
			}
		}
	}
	 thread[index].pi = pi;
	 thread[index].isFinish=1;

	printf("pi for index, %d =%.10f\n",index,4* pi);
	// int threadIndex = getThreadIndex(thread,nThreads,index);
	// if (!isFinish(thread,nThreads)){

	// 	if (thread[threadIndex].isFirstTime==0){
	// 		printf("old1=%d, new1 %d\n",index,threadIndex );
	// 		longjmp(thread[threadIndex].buff,1000);
	// 	}
	// }
}

int getThreadIndex(struct Thread* thread,int nThreads,int oldIndex){
	while(1){
		long tt = sumTickets(thread,nThreads,oldIndex);
		printf("totalTickets ====> %lu\n",tt);
		long ticketNumber = myRandom(tt);
		
		printf("ticketNumber %ld\n",ticketNumber );
		return chooseThread(thread,nThreads,ticketNumber,oldIndex);
	
	}
}

//Funcion que pinta los threads en la pantalla
void setScreen(float percentaje, int tid, GtkWidget *grid, int totalThreads, struct Thread* thread){
    title = gtk_label_new("Threads");
    gtk_grid_attach(GTK_GRID(grid), title, 1,0,1,1); //Grid, componente, columna, fila, width, height
    
    for (int i=0; i<totalThreads; i++){
        lblThread = gtk_label_new("Tread:");
        pgbThread = gtk_progress_bar_new();
        
        
        gtk_grid_attach(GTK_GRID(grid), lblThread, 0,i+1,1,1);
        char buffer1[25];
        snprintf(buffer1, sizeof(buffer1), "Thread: %i", i);
        gtk_label_set_text(GTK_LABEL(lblThread), buffer1);
        gtk_grid_attach(GTK_GRID(grid), pgbThread, 1,i+1,1,1);
        if (tid == i){
            //gtk_progress_bar_set_text(GTK_PROGRESS_BAR(pgbThread),"Valor de pi: %f", 3.14151683378);
            //gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(data), TRUE);
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pgbThread), percentaje);
        }
        
    }
    
}

void lottery (struct Thread* thread,int nThreads,int isExp, GtkWidget *grid){
	if (nThreads>0){
		
		int threadIndex=-1;
		// int fromThread;
		// while (!isFinish(thread,nThreads)){
			if (threadIndex==-1){

				threadIndex = getThreadIndex(thread,nThreads,-1);
				printf("------------threadIndex %d\n", threadIndex);
			}
			//thread[threadIndex].isFinish = 1;
			
			thread[threadIndex].isFirstTime=0;
			calculatePi(thread,threadIndex,nThreads,isExp);
			setScreen(0.44,threadIndex,grid,nThreads, thread);
		

			// printf("entre!\n");
			// usleep(3000000);
		// }
	}
}
