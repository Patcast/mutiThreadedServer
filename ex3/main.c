#define _GNU_SOURCE
#define MAX 50

#include "config.h"
#include "sensor_db.h"



int callback(void *, int, char **, char **);

void storageManager(int pfds[]){
    pid_t my_parent_pid, my_child_pid;
    my_child_pid = getpid();
    my_parent_pid = getppid();
    printf("\nhello, I am a child and my id is %d. My dad is %d\n", my_child_pid,my_parent_pid);
    int numLoops = 5;
    char* message;

    close(pfds[0]); // child will not read. 
    while(--numLoops){
        asprintf(&message, "Hello");
        write(pfds[1],message,strlen(message)+1); // similar to write a file, remeber to add 1 to message for null value at the end of the string.
        free(message);
        sleep(1);
    }
    close(pfds[1]);// this will tell the parent that no more messages will be sent. 
    exit(EXIT_SUCCESS);
}

void logFileGenerator(int pfds[]){
    int result = close(pfds[1]); // parent won't write
    SYSCALL_ERROR(result);
    int recordNumber=0;
    FILE *fp_log;
    char recBuffer[MAX];
      do{
            result = read(pfds[0],recBuffer,MAX);
            SYSCALL_ERROR(result);
            if(result>0){
                printf("\nI got this message from the storageManager :\t%d\t%lu\t%s\n",recordNumber,time(NULL),recBuffer);
                recordNumber++;
            }
        }while(result>0);
    result =close(pfds[0]); // parent won't read anymore. 
    SYSCALL_ERROR(result);  
}


int main (void ){
    pid_t storage_pid,pid;
    int storageExitStatus;
    int pipeFileDescriptors[2];/// index [0] reads 
    int result;
    

    result = pipe(pipeFileDescriptors); 
    SYSCALL_ERROR(result);
    storage_pid = fork();
    SYSCALL_ERROR(storage_pid);
    if( storage_pid == 0){ // code that will be executed by the child 
        storageManager(pipeFileDescriptors);
    }
    else{   // Code that woul be executed by the parent
        logFileGenerator(pipeFileDescriptors);
    }

    pid = wait(&storageExitStatus);
    SYSCALL_ERROR(pid);
    if(WIFEXITED(storageExitStatus)){ // check if child exited correctly
        printf("\nStorage Manager %d terminated with exit code %d\n",pid,storageExitStatus);
    }
    else{
        printf("\nStorage Manager %d terminated abnormally %d",pid, storageExitStatus);

    }

    //DBCONN *db;
    //db = init_connection(0);
    //if(db==NULL) return 1;
    // FILE* fileSensor = fopen("sensor_data","r");
    // if (fileSensor == NULL ){// I need to add includes for errors
    //       perror("My error is");
    // }
    // insert_sensor_from_file(db,fileSensor );
    //find_sensor_by_value(db,15.0,callback);
    //find_sensor_exceed_value(db,24,callback);
    //find_sensor_after_timestamp(db,1638097863,callback);
    // fclose(fileSensor);
    //disconnect(db); 
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

