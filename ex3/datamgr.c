
/**
 * \author Patricio Adolfo Castillo Calderon
 */

#define _GNU_SOURCE
#include "config.h"  //tools to handle tada from sensors
#include "datamgr.h" 
#include "dplist.h" 
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

///global variables
dplist_t * list;
char error_message [250];
////************

void error_massage_generator(err_code_t error);
void* element_copy(void * element);
void element_free(void** element);
/** Uses the property id to compare to my_element_t object.
 * \param x a pointer to the list
 * \param y a pointer to the list
 * \return An integer, if x(id) SMALLER than y(id) returns -1, 0 if equal and 1 if x(id) is BIGGER than y(id).
 */
int element_compare(void * x, void * y);
void* createNewData(std_int_t idS,std_int_t idT);

sensor_value_t* createArray();
void printArray();
sensor_value_t* insertAndShift(sensor_data_t valueToInstert, data_element_t* data_element);
sensor_value_t calculateAvg(sensor_value_t* array);

//********************************************* Methods for list

void * element_copy(void * element) {
    data_element_t* copy = malloc(sizeof (data_element_t));
    copy -> idSensor = ((data_element_t*)element)->idSensor;
    copy -> idTemp = ((data_element_t*)element)->idTemp;
    copy -> avg = ((data_element_t*)element)->avg;
    copy -> ts = ((data_element_t*)element)->ts;
    copy -> numRecAdded = 0;
    copy -> ptrToRecords = createArray();
    return (void *) copy;
}

void element_free(void** ptrElemenet) {
    data_element_t* element = (data_element_t*)*ptrElemenet;
    free(element ->ptrToRecords);
    free(*ptrElemenet);
    *ptrElemenet = NULL;
}

int element_compare(void * x, void * y) {
    return ((((data_element_t*)x)->idSensor < ((data_element_t*)y)->idSensor) ? -1 : (((data_element_t*)x)->idSensor == ((data_element_t*)y)->idSensor) ? 0 : 1);
}

 void error_massage_generator(err_code_t error)
{   
  switch( error ) {
    case  ERR_EMPTY:
        ERROR_HANDLER(true, "Can't execute this operation while the list is empty.");
        break;
    case ERR_MEM:
        ERROR_HANDLER(true, "Memory problem occured while executing this operation on the list. The list is NULL");
        break;  
     case ERR_INVALID_SEN_ID:
        ERROR_HANDLER(false,"The sensor id does not exist in the database");
        break;          
    default: // should never come here
      assert( 1==0 );
  }
} 


///////Methods to calculate avarage *************
sensor_value_t* createArray(){
    return (sensor_value_t*) malloc(sizeof(sensor_value_t)*RUN_AVG_LENGTH);
}
void printArray(sensor_value_t* array){
    for(int count =0;count <RUN_AVG_LENGTH; count++){
        printf("\nValue #%d is: %f",count,array[count]);;
    }
    printf("\n************\n");
}
sensor_value_t* insertAndShift(sensor_data_t valueToInstert, data_element_t* data_element){
    if(data_element->numRecAdded >=RUN_AVG_LENGTH){
        for(int i=0;i<RUN_AVG_LENGTH-1;i++){
            data_element->ptrToRecords[i] = data_element->ptrToRecords[i+1];
        }
        data_element->ptrToRecords[RUN_AVG_LENGTH-1]= valueToInstert.value;
        data_element->avg = calculateAvg(data_element->ptrToRecords);
        if((data_element->avg)>SET_MAX_TEMP)fprintf(stderr, "\nThe room (RoomId =%d) at time %ld is too hot. Temp avarage => %f"
        ,data_element->idTemp
        ,data_element->ts
        ,data_element->avg);
        else if((data_element->avg)<SET_MIN_TEMP)fprintf(stderr, "\nThe room (RoomId =%d) at time %ld is too cold. Temp avarage => %f"
        ,data_element->idTemp
        ,data_element->ts
        ,data_element->avg);
    }
    else {
        data_element->ptrToRecords[data_element->numRecAdded ] = valueToInstert.value; 
        if(data_element->numRecAdded <=RUN_AVG_LENGTH)(data_element->numRecAdded) ++;
    }
    data_element->ts = valueToInstert.ts;   
}

sensor_value_t calculateAvg(sensor_value_t* array){
    sensor_value_t sum = 0;
    for(int i=0;i<RUN_AVG_LENGTH;i++){
        sum +=array[i];
    }
    return sum/RUN_AVG_LENGTH;
}
//// Methods specific for MANAGER ************************************************


void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE* fp_sensor_data){
   datamgr_parse_room(fp_sensor_map);// Nodes with sensorsID and roomId from data created.
   process_sensor_data(fp_sensor_data);
}

void  process_sensor_data(FILE* fp_sensor_data){
    sensor_data_t dataIn;
  
    while(feof(fp_sensor_data)== 0){
        fread(&(dataIn.id),sizeof(std_int_t),1,fp_sensor_data);
        fread(&(dataIn.value),sizeof(sensor_value_t),1,fp_sensor_data);
        fread(&(dataIn.ts),sizeof(std_time_t),1,fp_sensor_data);
        data_element_t* data = get_data_element(dataIn.id);
        if(data!=NULL)insertAndShift(dataIn,data);
    }
}
void datamgr_parse_room (FILE *fp_sensor_map){
    std_int_t sensorIdIn;
    std_int_t roomIdIn;
    list = dpl_create(element_copy, element_free, element_compare);
    int count =0;
    while
    (fscanf(fp_sensor_map,"%hd %hd\n",&roomIdIn,&sensorIdIn)!= EOF)
    {   
        dpl_insert_at_index(list, createNewData(sensorIdIn,roomIdIn), count,false);
     #ifdef DEBUG   
        printf("\nSensor id: %hd\tRoom id: %hd",sensorIdIn,roomIdIn);
     #endif   
        count++;
    }
}
void* createNewData(std_int_t idS,std_int_t idT){
    data_element_t* data = malloc(sizeof (data_element_t));
    data -> idSensor = idS;
    data -> idTemp = idT;
    data -> avg = 0;
    data -> ts = 0;
    data -> numRecAdded = 0;
    data -> ptrToRecords = createArray();
    return (void*) data;
}
void datamgr_free(){
    dpl_free(&list,true);
    list =NULL;
}

uint16_t datamgr_get_room_id(std_int_t sensor_id){
    if(get_data_element(sensor_id)!=NULL) return get_data_element(sensor_id)->idTemp;
    (error_massage_generator(ERR_INVALID_SEN_ID));
    return 0;   
}
sensor_value_t datamgr_get_avg(std_int_t sensor_id){/// get avarage from sensor data
    if(get_data_element(sensor_id)!=NULL) return get_data_element(sensor_id)->avg;
    (error_massage_generator(ERR_INVALID_SEN_ID));
    return 0;   
}

time_t datamgr_get_last_modified(std_int_t sensor_id){
    if(get_data_element(sensor_id)!=NULL) return get_data_element(sensor_id)->ts;
    (error_massage_generator(ERR_INVALID_SEN_ID));
    return 0;     
}

int datamgr_get_total_sensors(){
    return dpl_size(list); // this assumes that two nodes cannot have the same idSensor;
}

data_element_t* get_data_element(std_int_t sensor_id){
    if(list!=NULL){
        if(dpl_size(list)>0){
            dplist_node_t* dummy = dpl_get_first_reference(list);
            for(int count = 0;count < dpl_size(list); dummy = dpl_get_next_reference(list,dummy), count++){
                if(dpl_get_element_at_reference (list,dummy)!=NULL ){
                    data_element_t* data = (data_element_t*) dpl_get_element_at_reference (list,dummy);
                    if(data->idSensor == sensor_id)return data;
                }
            }
            error_massage_generator(ERR_INVALID_SEN_ID);
            return NULL;
        }
        error_massage_generator(ERR_EMPTY);
        return  NULL;
    }
    error_massage_generator(ERR_MEM);
    return NULL;
}
