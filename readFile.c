#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void readFile(char* fileName){
	FILE *ifp;
	
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
					fgetc(ifp);
					quatumIndex=nThreads*2+3;
				
				}else if (lineNumber==quatumIndex){
					quantum = strtol(word,&ptr,10);
					printf("quantum=%d\n", quantum);
				
				}else{
					if (wordSize>1){
							printf("lineNumber %d\n",lineNumber );
							
							if(lineNumber%2!=0){
								
								strtok(word,"-");
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
}



int main(int argc, char const *argv[])
{
	/* code */
	readFile("threadFile");
	return 0;
}