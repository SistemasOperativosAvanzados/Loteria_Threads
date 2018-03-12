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

void readFile(char* fileName);
long sumTickets(struct Thread* thread,int nThreads,int oldIndex);
struct Thread* createThread(int* nWork,int* nTickets,char** names,long quantum,int modeExp,int nThreads);

int myRandom(long max);
int getFirst(struct Thread* thread,int nThreads);
int chooseThread(long ticketNumber);

int NisFinish ();
int findLast();
void calculatePi();
int getThreadIndex();
void lottery ();

void createScreen(GtkWidget *grid, int totalThreads, struct Thread* thread, int modeExp);
void starProcessLottery(int isExpropiative, int numThreads, int quantumUI,int* nWork, int* nTickets,char** names);