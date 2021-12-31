
#include "./headers/config.h"
#include "./headers/log_generation.h"
#include "./headers/storageManager.h"


int main (void ){
    pid_t storage_pid,pid;
    int storageExitStatus;
    
    storage_pid = fork();
    SYSCALL_ERROR(storage_pid);
    if( storage_pid == 0){ // code that will be executed by the child 
        storageManager();
    }
    else{   // Code that woul be executed by the parent
        startsLogFileGenerator();
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



