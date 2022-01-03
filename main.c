
#include "sensor_db.h"
#include "connmgr.h"
#include "datamgr.h"

//__sig_atomic_t closingFlag;

int treadsHandler(char* port);
void freeThreadsParam(thread_parameters_t* thread_param);
thread_parameters_t* startThreadsParam(int port );
void startsLogFileGenerator();

int main ( int argc, char *argv[]  ){

    pid_t log_pid,pid;
    int logExitStatus;
    
    log_pid = fork();
    SYSCALL_ERROR(log_pid);
    if( log_pid == 0){ // code that will be executed by the child 
        startsLogFileGenerator();
    }
    else{   // Code that woul be executed by the parent
        int rs = treadsHandler(argv[1]);
        THREAD_ERROR(rs);
        printf("All threads are closed ..............");
    }

    pid = wait(&logExitStatus);
    SYSCALL_ERROR(pid);
    if(WIFEXITED(logExitStatus)){ // check if child exited correctly
        printf("\nLog Process %d terminated with exit code %d\n",pid,logExitStatus);
    }
    else{
        printf("\nLog process %d terminated abnormally %d",pid, logExitStatus);

    }
    return 0;
}

int treadsHandler(char* portString){
    thread_parameters_t* thread_param;
    int result;
    pthread_t   t_tcp,t_dataManager, t_storageManager;
    int port;
    sscanf(portString, "%d", &port);  
    thread_param = startThreadsParam(port);
    ////*** CREATE THREADS
    result= pthread_create(&t_tcp,NULL,connmgr_listen,(void*)thread_param);
    THREAD_ERROR(result);
    result= pthread_create(&t_dataManager,NULL,dataManager,(void*)thread_param);
    THREAD_ERROR(result);
    result= pthread_create(&t_storageManager,NULL,storageManager,(void*)thread_param);
    THREAD_ERROR(result);

    ////*** CLOSE WRTINIG THREAD 
  	result= pthread_join(t_tcp, NULL);
    THREAD_ERROR(result);    
    
    ////*** NOTIFY READING THREADS TO CLOSE 
    result = pthread_mutex_lock(thread_param->data_mutex);
    SYNCRONIZATION_ERROR(result);
    *(thread_param->tcpOpenFlag) = FALSE;
    printf("Wake up all waiting threads...\n");
    result = pthread_cond_broadcast(thread_param->myConVar);
    SYNCRONIZATION_ERROR(result);
    result = pthread_mutex_unlock(thread_param->data_mutex);
    SYNCRONIZATION_ERROR(result);

    ////*** WAIT FOR READING THREADS TO CLOSE 
    result= pthread_join(t_dataManager, NULL);
    THREAD_ERROR(result);
    result= pthread_join(t_storageManager, NULL);
    THREAD_ERROR(result); 
    freeThreadsParam(thread_param);
    return 0;
}

thread_parameters_t* startThreadsParam(int port ){

    thread_parameters_t* thread_param = malloc(sizeof(thread_parameters_t));
    thread_param->ptrToFilePtr = malloc(sizeof(FILE*));
    MEMORY_ERROR( thread_param->ptrToFilePtr);

    int result = mkfifo(FIFO_NAME, 0666);
    CHECK_MKFIFO(result); 	
    *(thread_param->ptrToFilePtr) = fopen(FIFO_NAME, "w");
    FILE_OPEN_ERROR(*(thread_param->ptrToFilePtr));
    printf("\nsyncing with reader ok\n");
    
    MEMORY_ERROR(thread_param);
    thread_param->data_mutex = malloc(sizeof(pthread_mutex_t));
    MEMORY_ERROR(thread_param->data_mutex);
    thread_param->myConVar = malloc(sizeof(pthread_cond_t));
    MEMORY_ERROR(thread_param->myConVar);
    thread_param->tcpOpenFlag = malloc(sizeof(char)); 
    MEMORY_ERROR(thread_param->tcpOpenFlag);
    *(thread_param->data_mutex) =( pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    *(thread_param->myConVar) = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    *(thread_param->tcpOpenFlag) = 1; 
    sbuffer_init(&(thread_param->bufferHead));
    thread_param->portNumber = port;
    return thread_param;
}

void freeThreadsParam(thread_parameters_t* thread_param){
    int result =  pthread_cond_destroy(thread_param->myConVar);
    SYNCRONIZATION_ERROR(result);
    result = pthread_mutex_destroy( thread_param->data_mutex );
    SYNCRONIZATION_ERROR(result);
    free(thread_param->data_mutex);
    free(thread_param->myConVar);
    free(thread_param->tcpOpenFlag);
    result = fclose(*(thread_param->ptrToFilePtr));
    FILE_CLOSE_ERROR(result);
    free(thread_param->ptrToFilePtr);
    // if(thread_param->bufferHead->head==NULL)printf("\n\nOn closing buffer is empty\n\n");
    free((thread_param->bufferHead));
    //free(thread_param->bufferHead);// assumes that buffer is empty at this point. 
    free(thread_param);
}

void startsLogFileGenerator(){
    FILE* fp;
    int recordNumber=0;
    FILE *fp_log;
    char recv_buf[MAX_CHAR];
    char *str_result;
    int result;

    printf("\n\nchild process has started\n\n");
    result = mkfifo(FIFO_NAME, 0666);
    CHECK_MKFIFO(result); 
    fp = fopen(FIFO_NAME, "r"); 
    printf("syncing with writer ok\n");
    FILE_OPEN_ERROR(fp);
    fp_log = fopen("filesAndData/gateway.log", "w");
    FILE_OPEN_ERROR(fp_log);

    do 
    {
        str_result = fgets(recv_buf, MAX_CHAR, fp);
        if ( str_result != NULL )
        { 
        printf("message from storageManager :\t%d\t%lu\t%s\n",recordNumber,time(NULL),str_result);
        fprintf(fp_log,"%d\t%lu\t%s\n",recordNumber,time(NULL),str_result);
        recordNumber++;
        }
    } while ( str_result != NULL ); 

    result = fclose( fp );
    FILE_CLOSE_ERROR(result);
    result = fclose(fp_log);
    FILE_CLOSE_ERROR(result);
    printf("\nlog process is about to exit...\n");
    exit(EXIT_SUCCESS);
}