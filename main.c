#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "sbuffer.h"

#include <fcntl.h>
#include <errno.h>
#include <string.h>



#define FILES_TO_READ 400



char tcpOpen = 1;
pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  myConVar = PTHREAD_COND_INITIALIZER;

void pthread_err_handler( int err_code, char *msg, char *file_name, char line_nr );
void closeProgram(sbuffer_t*  buffer,pthread_t t_tcp, pthread_t t_dataManager);

void *f_tcpSocket (void * buffer){
    sensor_data_t data;
    int presult;
    int count = FILES_TO_READ;
    FILE *fileSensor;
    fileSensor = fopen("sensor_data","r");//tSafe

    if (fileSensor == NULL){
         perror("My error is");// Set flag that cause program to start closing. //tSafe  
    }
     while(feof(fileSensor)== 0 && count--){ //for debug only 
    // while(feof(fileSensor)== 0 ){//tSafe


        fread(&(data.id),sizeof(sensor_id_t),1,fileSensor);//tSafe
        fread(&(data.value),sizeof(sensor_value_t),1,fileSensor);
        fread(&(data.ts),sizeof(sensor_ts_t),1,fileSensor);

        int resultLock = pthread_mutex_lock( &data_mutex );/// **************/// critical secction  
        pthread_err_handler( resultLock, "pthread_mutex_lock", __FILE__, __LINE__ );  

        sbuffer_insert(buffer,&data);
        printf("data added to buffer\n");
        presult = pthread_cond_broadcast(&myConVar);        //// communicate that there is data available. 
        pthread_err_handler( presult, "pthread_cond_broadcasted", __FILE__, __LINE__ );

        resultLock = pthread_mutex_unlock( &data_mutex );
        pthread_err_handler( resultLock, "pthread_mutex_unlock", __FILE__, __LINE__ );/// **************/// end of critical section 
        //sleep(1);//

    }

    //printf("All data was added to buffer\n\n\n"); /// if no sensor sedn data within timeout we close. 
    fclose(fileSensor);
    pthread_exit(NULL);
}

// void f_close_file (void * file){
//     printf("closing...FILE");
//     fclose((FILE *)file);
// }

void *f_dataManager (void * buffer){
    FILE *fileData;
    int resultBuffer, resultLock;
    sensor_data_t dataInPtr;

    fileData = fopen("sensor_data_text.txt", "w");
    if (fileData == NULL){
         perror("My error is");// Set flag that cause program to start closing.   
    }
    

    while(1){

        int resultLock = pthread_mutex_lock( &data_mutex ); /// critical secction  
        pthread_err_handler( resultLock, "pthread_mutex_lock", __FILE__, __LINE__ );

        resultBuffer =sbuffer_remove(buffer,&dataInPtr,DATA_MGR_FLAG);
        while(tcpOpen==TRUE && resultBuffer==SBUFFER_NO_DATA){
            //printf("\nwait DATA\n");
            pthread_cond_wait(&myConVar,&data_mutex);
            //printf("\nawake DATA\n");
            resultBuffer =sbuffer_remove(buffer,&dataInPtr,DATA_MGR_FLAG);
        }

        resultLock = pthread_mutex_unlock( &data_mutex );
        pthread_err_handler( resultLock, "pthread_mutex_unlock", __FILE__, __LINE__ );

        //printf("data found or TCP finish for Data with result %d\n", resultBuffer);
        if(resultBuffer ==SBUFFER_SUCCESS){
            fprintf(fileData,"%d\t%f\t%ld\n",dataInPtr.id,dataInPtr.value,dataInPtr.ts);
            printf("dataMgr---%d\t%f\t%ld\n",dataInPtr.id,dataInPtr.value,dataInPtr.ts);
            //sleep(3);
        }
        else if(tcpOpen==FALSE)break;
    }
    
    printf("Data wants to close\n");
    fclose(fileData);
    pthread_exit(NULL);
}

void *f_storageManager (void * buffer){

    FILE *fileStorage;
    int resultBuffer, resultLock;
    sensor_data_t dataInPtr;

    fileStorage = fopen("sensor_storage_text.txt", "w");
    if (fileStorage == NULL){
         perror("My error is");// Set flag that cause program to start closing.   
    }
    

    while(1){
        int resultLock = pthread_mutex_lock( &data_mutex ); /// critical secction  
        pthread_err_handler( resultLock, "pthread_mutex_lock", __FILE__, __LINE__ );

        resultBuffer =sbuffer_remove(buffer,&dataInPtr,STORAGE_MGR_FLAG);
        while(tcpOpen==TRUE && resultBuffer==SBUFFER_NO_DATA){
            //printf("\nwait Storage\n");
            pthread_cond_wait(&myConVar,&data_mutex);
            //printf("\nawake Storage\n");
            resultBuffer =sbuffer_remove(buffer,&dataInPtr,STORAGE_MGR_FLAG);
        }

        resultLock = pthread_mutex_unlock( &data_mutex );
        pthread_err_handler( resultLock, "pthread_mutex_unlock", __FILE__, __LINE__ );
        //printf("data found or TCP finish for Storage with result %d\n", resultBuffer);
        if(resultBuffer ==SBUFFER_SUCCESS){
            fprintf(fileStorage,"%d\t%f\t%ld\n",dataInPtr.id,dataInPtr.value,dataInPtr.ts);
            printf("Storage---%d\t%f\t%ld\n",dataInPtr.id,dataInPtr.value,dataInPtr.ts);
            resultBuffer ==SBUFFER_NO_DATA;
            //sleep(1);
        }
        else if(tcpOpen==FALSE)break;


    }
    
    printf("Storage wants to close\n");
    fclose(fileStorage);
    pthread_exit(NULL);
}




int main(void){

    pthread_t   t_tcp,t_dataManager, t_storageManager;
    sbuffer_t* bufferHead;
    sbuffer_init(&bufferHead);
    printf("\nHello I am the creator\n\n");

    pthread_create(&t_tcp,NULL,f_tcpSocket,(void*)bufferHead);
    pthread_create(&t_dataManager,NULL,f_dataManager,(void*)bufferHead);
    pthread_create(&t_storageManager,NULL,f_storageManager,(void*)bufferHead);

 
  	int result= pthread_join(t_tcp, NULL);
	pthread_err_handler( result, "pthread_join", __FILE__, __LINE__ );
    
    

    result = pthread_mutex_lock(&data_mutex);
    pthread_err_handler( result, "pthread_join", __FILE__, __LINE__ );
    tcpOpen = FALSE;
    printf("Wake up all waiting threads...\n");
    result = pthread_cond_broadcast(&myConVar);
    pthread_err_handler( result, "pthread_join", __FILE__, __LINE__ );

    result = pthread_mutex_unlock(&data_mutex);
    pthread_err_handler( result, "pthread_join", __FILE__, __LINE__ );





    result= pthread_join(t_dataManager, NULL);
	pthread_err_handler( result, "pthread_join", __FILE__, __LINE__ );

    result= pthread_join(t_storageManager, NULL);
	pthread_err_handler( result, "pthread_join", __FILE__, __LINE__ );

    result =  pthread_cond_destroy(&myConVar);
    pthread_err_handler( result, "pthread_mutex_destroy", __FILE__, __LINE__ );
    result = pthread_mutex_destroy( &data_mutex );
	pthread_err_handler( result, "pthread_mutex_destroy", __FILE__, __LINE__ );

    sbuffer_free(&bufferHead);
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
