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
	float pi;
	int isFirstTime;
	int keep;
	int k;
	float percentage;
    GtkWidget *pgbThread;
    GtkWidget *lblPi;
};


struct Thread* thread;
int threadIndex;
int nThreads;
time_t t;
int last;

clock_t start_t, end_t, total_t=0;

void lottery ();
void setProgress(GtkWidget *pgbThread, float percentage, float pi, GtkWidget *lblPi);
void CreateScreen(GtkWidget *grid, int totalThreads, struct Thread* thread);
void readFile(char* fileName, GtkWidget *grid);
struct Thread* createThread(int* nWork,int* nTickets,char** names,long quantum,int modeExp,int nThreads);
int myRandom(long max);
int getFirst(struct Thread* thread,int nThreads);
int chooseThread(long ticketNumber);
int NisFinish ();
void calculatePi();
long sumTickets(struct Thread* thread,int nThreads,int oldIndex);
int getThreadIndex();
int findLast();


//Declare variables
static GtkWidget *title;
static GtkWidget *lblThread;
static GtkWidget *lblNumThreads;
static GtkWidget *lblMode;
static GtkWidget *lblPi;
static GtkWidget *pgbThread;
static GtkWidget *btnContinue;

GtkWidget *window, *grid;

int main(int argc, char **argv)
{
	/* code */
	float seed = 2;
	int max = 5;
	int i = 0;
    
    
    gtk_init(&argc, &argv);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    grid = gtk_grid_new();
    gtk_window_set_title (GTK_WINDOW(window), "Scheduler");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_container_add(GTK_CONTAINER(window), grid);
    
    readFile("threadFile", grid);
    
	return 0;
}

//Funcion que pinta los threads en la pantalla
void CreateScreen(GtkWidget *grid, int totalThreads, struct Thread* thread){
    int count = 1;
    title = gtk_label_new("Threads");
    gtk_grid_attach(GTK_GRID(grid), title, 1,0,1,1); //Grid, componente, columna, fila, width, height
    
    lblNumThreads = gtk_label_new("Total de Threads:");
    gtk_grid_attach(GTK_GRID(grid), lblNumThreads, 0,1,1,1);
    char buffer[25];
    snprintf(buffer, sizeof(buffer), "Numero de Threads: %d", totalThreads);
    gtk_label_set_text(GTK_LABEL(lblNumThreads), buffer);
    
    if(thread[threadIndex].isExp == 0){
        lblMode = gtk_label_new("Modo: No expropiativo");
        gtk_grid_attach(GTK_GRID(grid), lblMode, 2,1,1,1);
    }else{
        lblMode = gtk_label_new("Modo: Expropiativo");
        gtk_grid_attach(GTK_GRID(grid), lblMode, 2,1,1,1);
    }
    
    for (int i=0; i<totalThreads; i++){
        printf("Nombre del thread: %s\t count: %d",thread[i].name, i);
        //usleep(10000);
        lblThread = gtk_label_new("Tread:");
        pgbThread = gtk_progress_bar_new();
        thread[i].pgbThread = pgbThread;
        
        
        gtk_grid_attach(GTK_GRID(grid), lblThread, 0,count+1,1,1);
        char buffer1[25];
        snprintf(buffer1, sizeof(buffer1), "Thread: %i", i);
        gtk_label_set_text(GTK_LABEL(lblThread), buffer1);
        gtk_grid_attach(GTK_GRID(grid), pgbThread, 1,count+1,1,1);
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pgbThread), 0.0);
        
        lblPi = gtk_label_new("Valor de Pi:");
        gtk_grid_attach(GTK_GRID(grid), lblPi, 2,count+1,1,1);
        
        thread[i].lblPi= lblPi;
        
        count ++;
    }
    btnContinue = gtk_button_new_with_label("Continue");
    g_signal_connect(btnContinue, "clicked", G_CALLBACK(lottery), NULL);
    gtk_grid_attach(GTK_GRID(grid), btnContinue, 1,totalThreads+3,1,1);
    gtk_widget_show_all(window);
    gtk_main();
    
}

void readFile(char* fileName, GtkWidget *grid){
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
					printf("Lei \n");
					printf("%s\n", word );

					int compareExp = strcmp(word,"Exp");
					printf("Compare 1 \n");
					printf("%d\n", compareExp );

					int compareNoExp = strcmp(word,"NoExp");
					printf("Compare 2 \n");
					printf("%d\n", compareNoExp );
					printf("sleeping...\n");
					//usleep(10000);

					if (compareExp == 0){
						printf("Exp enabled\n");
						modeExp=1;
					}else if(compareNoExp == 0){
						printf("No expropiativo \n");
						modeExp=0;
					}
					printf("sleeping...\n");
					//usleep(4000000);
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
    
    CreateScreen(grid, nThreads, thread);
    
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
			threads[x].percentage =0.0;
            threads[x].pgbThread = NULL;
            threads[x].lblPi = NULL;
		}
		return threads;
	}
	return NULL;
}


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

void setProgress(GtkWidget *pgbThread, float percentage, float pi, GtkWidget *lblPi){
    printf("The percentage is: %.4f \n",percentage);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR (pgbThread), percentage);
    //usleep(1000000);
    
    
    char buffer2[25];
    snprintf(buffer2, sizeof(buffer2), "Valor de Pi: %f", 4 * pi);
    gtk_label_set_text(GTK_LABEL(lblPi), buffer2);
    
    gtk_widget_show_all(window);
}

void calculatePi(){
	float pi = 0;
	
	int r ;
	
	//for (int k = 0 ; k < (thread[threadIndex].nWorks);k++){
	start_t = start_t + clock();
	r = setjmp(thread[threadIndex].buff);
	printf(" Calculate Pi\n");
	printf(" nWorks ===>%d\n",thread[threadIndex].nWorks );
	printf(" k ===>%d\n",thread[threadIndex].k);
	printf(" percentage ===>%.4f \n",thread[threadIndex].percentage);
    
    //Pintar el thread activo
    setProgress(thread[threadIndex].pgbThread,thread[threadIndex].percentage, thread[threadIndex].pi,thread[threadIndex].lblPi);

	while(1){
		if (thread[threadIndex].k< thread[threadIndex].nWorks){
			
			thread[threadIndex].pi = thread[threadIndex].pi + (pow(-1,thread[threadIndex].k ))/(2*thread[threadIndex].k +1);
			thread[threadIndex].percentage = ((100 * (float)thread[threadIndex].k) / (float)thread[threadIndex].nWorks)/100;
            
            
            
            
			printf(" im in thread %d\n", threadIndex );
			printf(" MY PI==>%.10f\n", thread[threadIndex].pi );
			printf(" Ticketes del thread %ld\n", thread[threadIndex].nTickets );
			printf(" Numero del tickete ==> %d\n", thread[threadIndex].k );
			printf(" percentage ===> %.2f \n",thread[threadIndex].percentage);
			//total_t += (double)(end_t - start_t) / CLOCKS_PER_SEC;
			thread[threadIndex].k =thread[threadIndex].k+1;
			if (thread[threadIndex].isExp == 1){
				//El modo expropiativo los threads no ceden el procesador.
				//Expropiativo	
				printf("Expropiativo\n");
				end_t = end_t + clock();
				total_t += (double)(end_t - start_t) / CLOCKS_PER_SEC;		
				// Pero al agotarse el quantum, el scheduler seleccionará otro thread para que
				// corra.
				if (total_t >=thread[threadIndex].quantum ){			
					start_t,total_t,end_t=0;
					printf("sleeping...\n");
					usleep(10000);
					printf("r= %d\n",r );
					printf("r index %d\n", threadIndex);					
					if (NisFinish()!=1){
						threadIndex = getThreadIndex();
					}
					printf("-------------2 - threadIndex %d\n", threadIndex);					
					if (thread[threadIndex].isFirstTime==0){

						//printf("old=%d, new %d\n",threadIndex,threadIndex );
						//	usleep(7000000);
						longjmp(thread[threadIndex].buff,thread[threadIndex].k);
					}else{
						thread[threadIndex].isFirstTime=0;
						calculatePi();
					}
				}		
			}else if(thread[threadIndex].isExp == 0){
				//No expropiativo		
				// Calculate percentage
				printf("No expropiativo\n");
				long threadPercentageQuantum = thread[threadIndex].quantum / 100;
				if (thread[threadIndex].percentage <= threadPercentageQuantum && thread[threadIndex].percentage <= 1.0 ){	
					thread[threadIndex].quantum += thread[threadIndex].quantum;
					//En el modo no expropiativo, los threads cederán voluntariamente
					// el procesador después de realizar una fracción de su trabajo.						
					printf(" r = %d\n",r );
					printf(" r index %d\n", threadIndex);					
					if (NisFinish()!=1){
						threadIndex = getThreadIndex();
					}
					printf("-------------2 - threadIndex %d\n", threadIndex);					
					if (thread[threadIndex].isFirstTime==0){						
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
		if (threadIndex==-1){

			threadIndex = getThreadIndex();
			printf("------------1threadIndex %d\n", threadIndex);
		}
		//thread[threadIndex].isFinish = 1;			
		thread[threadIndex].isFirstTime=0;
		calculatePi();
		printf("me sali del while loop\n");
			
	}
}
