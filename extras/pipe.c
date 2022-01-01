#define _GNU_SOURCE
#define MAX 50

#include "config.h"
#include "sensor_db.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <assert.h>

#define COM_CODE 9876
#define FIFO_NAME 	"MYFIFO" 


typedef enum log_code { 
    CONNECTED_DB = 0, 
    NOT_CONNECTED,
    FILE_OPEN,
    FILE_FAILURE}log_code_t;

void log_code_generator (log_code_t log,FILE* fp)
{ 
    char* send_buf; 
    printf("fuuuuu"); 
    switch( log ) {
        case  CONNECTED_DB:
            asprintf(&send_buf, "Connected successfully to db.");
            
            break;
        case NOT_CONNECTED:
            asprintf(&send_buf, "Failure trying to connect to db.");
            exit(EXIT_FAILURE);
            break;   
        case  FILE_OPEN:
            asprintf(&send_buf, "File open successfully.");
            
            break;
        case FILE_FAILURE:
            asprintf(&send_buf, "Failure trying to open file.");
            perror("My error is");
            break;           
        default: // should never come here
            
        assert( 1==0 );
  }
  //write(pfds[1],message,strlen(message)+1);

    if ( fputs( send_buf, fp ) == EOF )
    {
      fprintf( stderr, "Error writing data to fifo\n");
      exit( EXIT_FAILURE );
    } 
    fflush(fp);
    //FFLUSH_ERROR(fflush(fp));
    printf("Message send: %s", send_buf); 
    free( send_buf );
    sleep(1);
    free(send_buf);
} 

int callback(void *, int, char **, char **);

void storageManager(){
    FILE *fp; 
    pid_t my_parent_pid, my_child_pid;

    /* Create the FIFO if it does not exist */ 
    // int result = mkfifo(FIFO_NAME, 0666);
    mkfifo(FIFO_NAME, 0666);

    //CHECK_MKFIFO(result); 	
    fp = fopen(FIFO_NAME, "w"); 
    printf("syncing with reader ok\n");
    //FILE_OPEN_ERROR(fp);


    my_child_pid = getpid();
    my_parent_pid = getppid();
    printf("\nhello, I am a child and my id is %d. My dad is %d\n", my_child_pid,my_parent_pid);
    //int numLoops = 5;
    //char* message;
    //close(pfds[0]); // child will not read. 
    DBCONN *db;
    db = init_connection(0);
    if(db==NULL) log_code_generator(NOT_CONNECTED,fp);
    else log_code_generator(CONNECTED_DB,fp);
    FILE* fileSensor = fopen("sensor_data","r");
    if (fileSensor == NULL ) log_code_generator(FILE_FAILURE,fp);
    else log_code_generator(FILE_OPEN,fp);
    find_sensor_by_value(db,15.0,callback);
    
    fclose( fp );
    // result = fclose( fp );
    //FILE_CLOSE_ERROR(result);
    fclose(fileSensor);
    disconnect(db); 
    //close(pfds[1]);// this will tell the parent that no more messages will be sent. 
    exit(EXIT_SUCCESS);
}

void logFileGenerator(){
    //int result = close(pfds[1]); // parent won't write
    //SYSCALL_ERROR(result);
    FILE* fp;
    int recordNumber=0;
    FILE *fp_log;
    char recv_buf[MAX];
    char *str_result;
    int result;

    result = mkfifo(FIFO_NAME, 0666);
    //CHECK_MKFIFO(result); 
    fp = fopen(FIFO_NAME, "r"); 
    printf("syncing with writer ok\n");
    //FILE_OPEN_ERROR(fp);

    fp_log = fopen("gateway.log", "w");
    FILE_ERROR(fp_log, "Couldn't create gateway.log\n");
    //   do{   
    //         result = read(pfds[0],recBuffer,MAX);
    //         SYSCALL_ERROR(result);
    //         if(result>0){
    //             printf("message from storageManager :\t%d\t%lu\t%s\n",recordNumber,time(NULL),recBuffer);
    //             fprintf(fp_log,"message from storageManager :\t%d\t%lu\t%s\n",recordNumber,time(NULL),recBuffer);
    //             recordNumber++;
    //         }
    //     }while(result>0);
    do 
    {
        str_result = fgets(recv_buf, MAX, fp);
        if ( str_result != NULL )
        { 
        printf("message from storageManager :\t%d\t%lu\t%s\n",recordNumber,time(NULL),str_result);
        fprintf(fp_log,"message from storageManager :\t%d\t%lu\t%s\n",recordNumber,time(NULL),str_result);
        recordNumber++;
        }
    } while ( str_result != NULL ); 

    result = fclose( fp );
    //FILE_CLOSE_ERROR(result);
    //result =close(pfds[0]); // parent won't read anymore. 
    fclose(fp_log);
    SYSCALL_ERROR(result);  
}


int main (void ){
    pid_t storage_pid,pid;
    int storageExitStatus;
    
    //int pipeFileDescriptors[2];/// index [0] reads 
    //int result;
    

    //result = pipe(pipeFileDescriptors); 
    //SYSCALL_ERROR(result);
    storage_pid = fork();
    SYSCALL_ERROR(storage_pid);
    if( storage_pid == 0){ // code that will be executed by the child 
        storageManager();
    }
    else{   // Code that woul be executed by the parent
        logFileGenerator();
    }
    pid = wait(&storageExitStatus);
    SYSCALL_ERROR(pid);
    if(WIFEXITED(storageExitStatus)){ // check if child exited correctly
        printf("\nStorage Manager %d terminated with exit code %d\n",pid,storageExitStatus);
    }
    else{
        printf("\nStorage Manager %d terminated abnormally %d",pid, storageExitStatus);

    }
    return 0;
}

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    // arg: number of columns
    // argv: array of strings representing fields in the row
    // azColName: array of column names 
    NotUsed = 0;
    for (int i = 0; i < argc; i++) {
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    
    return 0;
}