/**
 * \author Patricio Adolfo Castillo Calderon
 */

#include "config.h"  //tools to handle tada from sensors
#include "datamgr.h"  
#include <stdlib.h>
#include <stdio.h>
// error includes 
#include <fcntl.h>
#include <errno.h>
#include <string.h>
//char numRecAdded = 0;
// end of error includes 
// sensor_value_t* createArray();
// void printArray();
// sensor_value_t* InsertAndShift(sensor_value_t valueToInstert, sensor_value_t* array);
// sensor_value_t calculateAvg(sensor_value_t* array);
// void freeArray(sensor_value_t* array);

int main(void) {    
    FILE* fileRoom = fopen("room_sensor.map","r");
    FILE* fileSensor = fopen("sensor_data","r");
    if (fileRoom == NULL || fileSensor == NULL ){
         perror("My error is");
    }

    datamgr_parse_sensor_files(fileRoom,fileSensor);
    
    fclose(fileRoom);
    fclose(fileSensor);
    datamgr_free(); 

    /* sensor_value_t* ptrArray = createArray();
    InsertAndShift(1.0000,ptrArray);
    InsertAndShift(2.0000,ptrArray);
    InsertAndShift(3.0000,ptrArray);
    InsertAndShift(4.0000,ptrArray);
    InsertAndShift(5.0000,ptrArray);
    InsertAndShift(6.0000,ptrArray);
    InsertAndShift(7.0000,ptrArray);
    InsertAndShift(8.0000,ptrArray);
    InsertAndShift(9.0000,ptrArray);
    InsertAndShift(10.0000,ptrArray);
    freeArray(ptrArray);
    printf("\n\n This is the number of elements added  %d\n",numRecAdded); */
    return  0; 
}


