#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "sbuffer.h"

#include <fcntl.h>
#include <errno.h>
#include <string.h>

/* void *f (void * t_id){
    int count = LOOP;
    int idOfThread = *(int *)t_id;
    while(count --){
        printf("Hello, I am thread %d\n", idOfThread);
        sleep(1);
    }
}
 */
void  process_sensor_data(FILE* fp_sensor_data){
    sensor_data_t dataIn;
    
    while(feof(fp_sensor_data)== 0){
        fread(&(dataIn.id),sizeof(sensor_id_t),1,fp_sensor_data);
        fread(&(dataIn.value),sizeof(sensor_value_t),1,fp_sensor_data);
        fread(&(dataIn.ts),sizeof(sensor_ts_t),1,fp_sensor_data);
        printf("\n%d\t%f\t%ld\n",dataIn.id,dataIn.value,dataIn.ts);
    }
}


int main(void){
    FILE* fileSensor = fopen("sensor_data","r");
    if (fileSensor == NULL ){
         perror("My error is");
    }
    process_sensor_data(fileSensor);
    fclose(fileSensor);




    return 0;
}

/* int main(void){
    pthread_t   t1,t2;
    int id1=100,id2=200;
    int * result;

    printf("\nHello I am the creator\n\n");

    pthread_create(&t1,NULL,f,(void*)&id1);
    pthread_create(&t2,NULL,f,(void*)&id2);
    int count = LOOP;
    while(count --){
        printf("Hello, I am thread Master\n");
        sleep(1);
    }

    pthread_join(t1,(void**)(&result));
    printf("Thread 1 ended with status %d", *result);
    free(result);

    pthread_join(t2,(void**)(&result));
    printf("Thread 1 ended with status %d", *result);
    free(result);

    return 0;
} */