
/**
 * \author Patricio Adolfo Castillo Calderon
 */
#include "datamgr.h"

///global variables
dplist_t * list;

void* dataManager(void * thread_param_input){
    FILE* fileRoom;

    int resultBuffer, resultLock;
    sensor_data_t dataIn;
    thread_parameters_t* param = thread_param_input;


    list = dpl_create(element_copy_dta_magr, element_free_dta_magr, element_compare_dta_magr);
    fileRoom = fopen("room_sensor.map","r");
    FILE_OPEN_ERROR(fileRoom);
    datamgr_parse_sensor_files(fileRoom,NULL);
    fclose(fileRoom);

    while(1){
        resultLock = pthread_mutex_lock( param->data_mutex ); /// critical secction  
        SYNCRONIZATION_ERROR(resultLock);
        resultBuffer =sbuffer_remove(param->bufferHead ,&dataIn,DATA_MGR_FLAG);
        while(*(param->tcpOpenFlag )==TRUE && resultBuffer==SBUFFER_NO_DATA){
            //printf("\nwait Data\n");
            int resultLock = pthread_cond_wait(param->myConVar ,param->data_mutex );
            SYNCRONIZATION_ERROR(resultLock);
            //printf("\nawake Data\n");
            resultBuffer =sbuffer_remove(param->bufferHead,&dataIn,DATA_MGR_FLAG);
            #ifdef DEBUG_BUFFER  
                printf("buffer outcome code:%d\n",resultBuffer);
            #endif  
        }
        #ifdef DEBUG_DTA_MGR  
            printf("D:\t%d\t%f\t%ld\n",dataIn.id,dataIn.value,dataIn.ts);
        #endif
        resultLock = pthread_mutex_unlock( param->data_mutex  );
        SYNCRONIZATION_ERROR(resultLock);
        if(resultBuffer ==SBUFFER_SUCCESS){
            data_element_t* data = get_data_element(dataIn.id);
            if(data!=NULL)insertAndShift(dataIn,data,*(param->ptrToFilePtr));
            else LOG_INVALID_SENSOR(dataIn.id,(*(param->ptrToFilePtr)));
        }

        if(*(param->tcpOpenFlag )==FALSE && (resultBuffer ==SBUFFER_NO_DATA)){
            break;
        }
    }
    #ifdef DEBUG_DTA_MGR  
        printf("Data is closing\n");
    #endif
    datamgr_free();
    pthread_exit(NULL); 
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

void insertAndShift(sensor_data_t valueToInstert, data_element_t* data_element, FILE* fp){
    if(data_element->numRecAdded >=RUN_AVG_LENGTH){
        for(int i=0;i<RUN_AVG_LENGTH-1;i++){
            data_element->ptrToRecords[i] = data_element->ptrToRecords[i+1];
        }
        data_element->ptrToRecords[RUN_AVG_LENGTH-1]= valueToInstert.value;
        data_element->avg = calculateAvg(data_element->ptrToRecords);
        TEMPERATURE_ALARM(fp,data_element);
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


void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE* fp_sensor_data){ /// the signture is different on the 
   datamgr_parse_room(fp_sensor_map);// Nodes with sensorsID and roomId from data created.
   //process_sensor_data(fp_sensor_data);
}

/* void  process_sensor_data(FILE* fp_sensor_data){
    sensor_data_t dataIn;
  
    while(feof(fp_sensor_data)== 0){
        fread(&(dataIn.id),sizeof(sensor_id_t),1,fp_sensor_data);
        fread(&(dataIn.value),sizeof(sensor_value_t),1,fp_sensor_data);
        fread(&(dataIn.ts),sizeof(sensor_ts_t),1,fp_sensor_data);
        data_element_t* data = get_data_element(dataIn.id);
        if(data!=NULL)insertAndShift(dataIn,data);
    }
} */

void datamgr_parse_room (FILE *fp_sensor_map){
    sensor_id_t sensorIdIn;
    sensor_id_t roomIdIn;
    
    int count =0;
    while
    (fscanf(fp_sensor_map,"%hd %hd\n",&roomIdIn,&sensorIdIn)!= EOF)
    {     
        dpl_insert_at_index(list, createNewData(sensorIdIn,roomIdIn), count,false);
     #ifdef DEBUG_DTA_MGR   
        printf("\nSensor id: %hd\tRoom id: %hd\n",sensorIdIn,roomIdIn);
     #endif   
        count++;
    }
    #ifdef DEBUG_DTA_MGR   
        printf("The total count of valid sensors  added to the list is %d\n", count);
    #endif 
}

void* createNewData(sensor_id_t idS,sensor_id_t idT){
    data_element_t* data = malloc(sizeof (data_element_t));
    data -> idSensor = idS;
    data -> idRoom = idT;
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

uint16_t datamgr_get_room_id(sensor_id_t sensor_id){
    data_element_t* data = get_data_element(sensor_id);
    return (data!=NULL)? data->idRoom: 0;  
}
sensor_value_t datamgr_get_avg(sensor_id_t sensor_id){/// get avarage from sensor data
    data_element_t* data = get_data_element(sensor_id);
    return (data!=NULL)? data->avg: 0; 
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id){
    data_element_t* data = get_data_element(sensor_id);
    return (data!=NULL)? data->ts: 0;   
}

int datamgr_get_total_sensors(){
    return dpl_size(list); // this assumes that two nodes cannot have the same idSensor;
}

data_element_t* get_data_element(sensor_id_t sensor_id){
    if(list!=NULL){
        if(dpl_size(list)>0){
            dplist_node_t* dummy = dpl_get_first_reference(list);
            data_element_t* data = NULL;
            for(int count = 0;count < dpl_size(list); dummy = dpl_get_next_reference(list,dummy), count++){
                if(dpl_get_element_at_reference (list,dummy)!=NULL ){
                    data = (data_element_t*) dpl_get_element_at_reference (list,dummy);
                    if(data->idSensor == sensor_id)return data;
                }
            }
            return NULL;
        }
        EMPTY_LIST();
        return  NULL;
    }
    LIST_IS_NULL();
    return NULL;
}



//********************************************* Methods for list

void * element_copy_dta_magr(void * element) {
    data_element_t* copy = malloc(sizeof (data_element_t));
    copy -> idSensor = ((data_element_t*)element)->idSensor;
    copy -> idRoom = ((data_element_t*)element)->idRoom;
    copy -> avg = ((data_element_t*)element)->avg;
    copy -> ts = ((data_element_t*)element)->ts;
    copy -> numRecAdded = 0;
    copy -> ptrToRecords = createArray();
    return (void *) copy;
}

void element_free_dta_magr(void** ptrElemenet) {
    data_element_t* element = (data_element_t*)*ptrElemenet;
    free(element ->ptrToRecords);
    free(*ptrElemenet);
    *ptrElemenet = NULL;
}

int element_compare_dta_magr(void * x, void * y) {
    return ((((data_element_t*)x)->idSensor < ((data_element_t*)y)->idSensor) ? -1 : (((data_element_t*)x)->idSensor == ((data_element_t*)y)->idSensor) ? 0 : 1);
}


