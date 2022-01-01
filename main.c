
#include "sensor_db.h"

#define FILES_TO_READ 5
//__sig_atomic_t closingFlag;

int treadsHandler(void);
void freeThreadsParam(thread_parameters_t* thread_param);
thread_parameters_t* startThreadsParam();
void startsLogFileGenerator();



int main (void ){

    pid_t log_pid,pid;
    int logExitStatus;
    
    log_pid = fork();
    SYSCALL_ERROR(log_pid);
    if( log_pid == 0){ // code that will be executed by the child 
        startsLogFileGenerator();
    }
    else{   // Code that woul be executed by the parent
        int rs = treadsHandler();
        THREAD_ERROR(rs);
        printf("All threads are closed ..............");
    }

    printf("\n\n\nwaiting to log to close...\n");
    pid = wait(&logExitStatus);
    SYSCALL_ERROR(pid);
    if(WIFEXITED(logExitStatus)){ // check if child exited correctly
        printf("\nMain Process %d terminated with exit code %d\n",pid,logExitStatus);
    }
    else{
        printf("\nMain process %d terminated abnormally %d",pid, logExitStatus);

    }
    return 0;
}


void *f_tcpSocket (void * thread_param_input){
    sensor_data_t data;
    int resultLock;
    int count = FILES_TO_READ;
    FILE *fileSensor;
    thread_parameters_t* param = thread_param_input;

    fileSensor = fopen("filesAndData/sensor_data","r");//tSafe
    FILE_OPEN_ERROR(fileSensor);

    while(feof(fileSensor)== 0 && count--){ //for debug only 
    // while(feof(fileSensor)== 0 ){//tSafe


        fread(&(data.id),sizeof(sensor_id_t),1,fileSensor);//tSafe
        fread(&(data.value),sizeof(sensor_value_t),1,fileSensor);
        fread(&(data.ts),sizeof(sensor_ts_t),1,fileSensor);

        resultLock = pthread_mutex_lock( param->data_mutex ); /// critical secction  
        SYNCRONIZATION_ERROR(resultLock);

        sbuffer_insert(param->bufferHead,&data);
        printf("data added to buffer\n");
        resultLock = pthread_cond_broadcast(param->myConVar);        //// communicate that there is data available. 
        SYNCRONIZATION_ERROR(resultLock);

        resultLock = pthread_mutex_unlock(param->data_mutex );
        SYNCRONIZATION_ERROR(resultLock);
        //sleep(1);//

    }

    //printf("All data was added to buffer\n\n\n"); /// if no sensor sedn data within timeout we close. 
    fclose(fileSensor);
    printf("TCP wants to close\n");
    pthread_exit(NULL);
}

/* void *f_dataManager (void * buffer){
    FILE *fileData;
    int resultBuffer, resultLock;
    sensor_data_t dataInPtr;

    fileData = fopen("filesAndData/sensor_data_text.txt", "w");
    FILE_OPEN_ERROR(fileData);
    

    while(1){

        resultLock = pthread_mutex_lock( &data_mutex ); /// critical secction  
        SYNCRONIZATION_ERROR(resultLock);

        resultBuffer =sbuffer_remove(buffer,&dataInPtr,DATA_MGR_FLAG);
        while(tcpOpen==TRUE && resultBuffer==SBUFFER_NO_DATA){
            //printf("\nwait DATA\n");
            pthread_cond_wait(&myConVar,&data_mutex);
            //printf("\nawake DATA\n");
            resultBuffer =sbuffer_remove(buffer,&dataInPtr,DATA_MGR_FLAG);
        }

        resultLock = pthread_mutex_unlock( &data_mutex );
        SYNCRONIZATION_ERROR(resultLock);
        //printf("data found or TCP finish for Data with result %d\n", resultBuffer);
        if(resultBuffer ==SBUFFER_SUCCESS){
            fprintf(fileData,"%d\t%f\t%ld\n",dataInPtr.id,dataInPtr.value,dataInPtr.ts);
            printf("dataMgr---%d\t%f\t%ld\n",dataInPtr.id,dataInPtr.value,dataInPtr.ts);
            //sleep(3);
        }

        if(tcpOpen==FALSE && !(resultBuffer ==SBUFFER_SUCCESS))break;

    }
    
   
    fclose(fileData);
    printf("Data wants to close\n");
    pthread_exit(NULL);
} */


int treadsHandler(){

    thread_parameters_t* thread_param;
    int result;
    // pthread_t   t_tcp,t_dataManager, t_storageManager;
    pthread_t   t_tcp, t_storageManager;
    
    thread_param = startThreadsParam();
    ////*** CREATE THREADS
    result= pthread_create(&t_tcp,NULL,f_tcpSocket,(void*)thread_param);
    THREAD_ERROR(result);
/*     result= pthread_create(&t_dataManager,NULL,f_dataManager,(void*)thread_param);
    THREAD_ERROR(result); */
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
/*  result= pthread_join(t_dataManager, NULL);
    THREAD_ERROR(result);    */ 
    result= pthread_join(t_storageManager, NULL);
    THREAD_ERROR(result); 
    freeThreadsParam(thread_param);
    return 0;
}

thread_parameters_t* startThreadsParam( ){

    thread_parameters_t* thread_param = malloc(sizeof(thread_parameters_t));
    thread_param->ptrToFilePtr = malloc(sizeof(FILE*));
    MEMORY_ERROR( thread_param->ptrToFilePtr);

    int result = mkfifo(FIFO_NAME, 0666);
    CHECK_MKFIFO(result); 	
    *(thread_param->ptrToFilePtr) = fopen(FIFO_NAME, "w");
    FILE_OPEN_ERROR(*(thread_param->ptrToFilePtr));
    printf("\nsyncing with reader ok\n");
    printf("\n\ndouble\n\n");
    
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
    free(thread_param);
    sbuffer_free(&(thread_param->bufferHead));
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


