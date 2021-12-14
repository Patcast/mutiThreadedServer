// CONCURRENCY // 

// Each process is independent of each other. Different memory. 
// Context switch -> when you change from one process to another. 

#define _GNU_SOURCE
#define MAX 50

#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h> 
#include <string.h>

// The backlashes must be the last thing on that line 
#define SYSCALL_ERROR(err) \
        do{ \
            if((err)==-1){\
                perror("error executiong syscall");\
                exit(EXIT_FAILURE);\
                 } \
        }while(0)    





void child(int pfds[]){
    pid_t my_parent_pid, my_child_pid;
    my_child_pid = getpid();
    my_parent_pid = getppid();
    printf("\nhello, I am a child and my id is %d. My dad is %d\n", my_child_pid,my_parent_pid);
    int numLoops = 5;
    char* message;

    close(pfds[0]); // child will not read. 
    while(--numLoops){
        asprintf(&message, "Loop number %d",numLoops);
        write(pfds[1],message,strlen(message)+1); // similar to write a file, remeber to add 1 to message for null value at the end of the string.
        free(message);
        sleep(1);
    }
    close(pfds[1]);// this will tell the parent that no more messages will be sent. 
    exit(EXIT_SUCCESS);
}






int main (void){

pid_t my_pid,pid, child_pid;
int childExitStatus;
int pipeFileDescriptors[2];/// index [0] reads 
int result;


my_pid= getpid();
printf("\nparent id = %d\n\n",my_pid);

result = pipe(pipeFileDescriptors); // you must pipe before the forking, so both processes have access to the pfds
// Bear in mind that pipes will almos always be atomic. see man 7 pipe. 
SYSCALL_ERROR(result);


child_pid = fork(); // cretes two identicall processes
SYSCALL_ERROR(child_pid);
if( child_pid == 0){ // code that will be executed by the child 
    child(pipeFileDescriptors);
}
else{   // Code that woul be executed by the parent
    char recBuffer[MAX];
    printf("\nhello, I am a the parent and my id is %d and I created %d\n",my_pid, child_pid); 
    close(pipeFileDescriptors[1]); // parent won't write
    do{
        result = read(pipeFileDescriptors[0],recBuffer,MAX);
        SYSCALL_ERROR(result);
        if(result>0){
            printf("\nI got this message from my child %d:\t%s\n",child_pid,recBuffer);
        }
    }while(result>0);
    close(pipeFileDescriptors[1]); // parent won't read anymore. 

}
/*  The wait() system call suspends execution of the calling thread  until  one
       of its children terminates. */

//pid = wait(&childExitStatus);// additionally waitpid would wait for an specific child 
pid = waitpid(child_pid,NULL,0);
SYSCALL_ERROR(pid);




if(WIFEXITED(childExitStatus)){ // check if child exited correctly
    printf("\n\nChild %d terminated with exit code %d",pid,WIFEXITED(childExitStatus));
}
else{
    printf("\n\nchild %d terminated weirdly",pid);
}
printf("\n\nseems that all childs are gone\n");
exit(EXIT_SUCCESS);//  Normally parent should wait for childs 
}
