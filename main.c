#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "sbuffer.h"

#include <fcntl.h>
#include <errno.h>
#include <string.h>


#define FALSE  0
#define TRUE  1
#define FILES_TO_READ 3



char tcpOpen = TRUE;
//pthread_rwlock_t rwlock;
pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t end_flag_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  myConVar = PTHREAD_COND_INITIALIZER;

void pthread_err_handler( int err_code, char *msg, char *file_name, char line_nr );
void closeProgram(sbuffer_t*  buffer,pthread_t t_tcp, pthread_t t_dataManager);

void *f_tcpSocket (void * buffer){
    sensor_data_t data;
    int count = FILES_TO_READ;
    FILE *fileSensor;
    fileSensor = fopen("sensor_data","r");//tSafe

    if (fileSensor == NULL){
         perror("My error is");// Set flag that cause program to start closing. //tSafe  
    }
    while(feof(fileSensor)== 0 && count--){//tSafe

        fread(&(data.id),sizeof(sensor_id_t),1,fileSensor);//tSafe
        fread(&(data.value),sizeof(sensor_value_t),1,fileSensor);
        fread(&(data.ts),sizeof(sensor_ts_t),1,fileSensor);

        //// add data to buffer
        int presult = pthread_mutex_lock( &data_mutex ); /// critical secction  
        pthread_err_handler( presult, "pthread_mutex_lock", __FILE__, __LINE__ );
        sbuffer_insert(buffer,&data);
        printf("data added to buffer\n");
        //// communicate that there is data available. 
        presult = pthread_cond_broadcast(&myConVar);
        pthread_err_handler( presult, "pthread_mutex_unlock", __FILE__, __LINE__ );
        
        presult = pthread_mutex_unlock( &data_mutex );
        pthread_err_handler( presult, "pthread_mutex_unlock", __FILE__, __LINE__ );
        sleep(1);
    }
    tcpOpen = FALSE;
    printf("All data was added to buffer\n"); /// if no sensor sedn data within timeout we close. 
    fclose(fileSensor);
    pthread_exit( NULL);
}

void *f_dataManager (void * buffer){
    FILE *fileData;
    sensor_data_t dataInPtr;
    fileData = fopen("sensor_data_text.txt", "w");
    if (fileData == NULL){
         perror("My error is");// Set flag that cause program to start closing.   
    }
    sbuffer_t* bufferHead = buffer;


    while((tcpOpen == TRUE )||(bufferHead ->head != NULL)){
        /// starts lock 
        int presult = pthread_mutex_lock( &data_mutex ); /// critical secction  
        pthread_err_handler( presult, "pthread_mutex_lock", __FILE__, __LINE__ );
        
        while (bufferHead -> head== NULL){
            if (tcpOpen == TRUE){
                pthread_cond_wait(&myConVar,&data_mutex);
            }
            else break;  
        }

        sbuffer_remove(buffer,&dataInPtr,0,0);
        presult = pthread_mutex_unlock( &data_mutex );
        pthread_err_handler( presult, "pthread_mutex_unlock", __FILE__, __LINE__ );
        /// finishes lock 

        fprintf(fileData,"%d\t%f\t%ld\n",dataInPtr.id,dataInPtr.value,dataInPtr.ts);
        printf("%d\t%f\t%ld\n",dataInPtr.id,dataInPtr.value,dataInPtr.ts);
        sleep(1);
    }

    fclose(fileData);
}




int main(void){

    pthread_t   t_tcp,t_dataManager;
    sbuffer_t* bufferHead;
    sbuffer_init(&bufferHead);
    printf("\nHello I am the creator\n\n");
    //pthread_rwlock_init(&rwlock, NULL);

    pthread_create(&t_tcp,NULL,f_tcpSocket,(void*)bufferHead);
    pthread_create(&t_dataManager,NULL,f_dataManager,(void*)bufferHead);

  	int presult= pthread_join(t_tcp, NULL);
	pthread_err_handler( presult, "pthread_join", __FILE__, __LINE__ );

    presult= pthread_join(t_dataManager, NULL);
	pthread_err_handler( presult, "pthread_join", __FILE__, __LINE__ );
    presult = pthread_mutex_destroy( &data_mutex );
	pthread_err_handler( presult, "pthread_mutex_destroy", __FILE__, __LINE__ );

    sbuffer_free(&bufferHead);
    //closeProgram(bufferHead,t_tcp,t_dataManager);
    printf("\nAll closed\n");
    pthread_exit(NULL);
}



void pthread_err_handler( int err_code, char *msg, char *file_name, char line_nr )
{
	if ( 0 != err_code )
	{
		fprintf( stderr, "\n%s failed with error code %d in file %s at line %d\n", msg, err_code, file_name, line_nr );
		//errno = err_code;
		//perror("Error message: ");
	}
}
