
/**
 * \author Patricio Adolfo Castillo Calderon
 */
#include "datamgr.h"



void cleanup_handler_data_mgr(void *arg){
    #ifdef DEBUG_DTA_MGR  
        printf("Data Manager is closing\n");
    #endif
    exit_params_data_t* exit_params = arg;
    datamgr_free(exit_params->list);
    pthread_cancel(*(exit_params->param->tcp_thread));
    free(exit_params);
    pthread_exit(NULL); 
}

void* dataManager(void * thread_param_input){
    FILE* fileRoom;
    sensor_data_t dataIn;
    thread_parameters_t* param = thread_param_input;
    dplist_t * list;
    exit_params_data_t * exit_params;
    exit_params = malloc(sizeof(exit_params_data_t));
    MEMORY_ERROR(exit_params);
    exit_params ->param=param;

    list = dpl_create(element_copy_dta_magr, element_free_dta_magr, element_compare_dta_magr);
    exit_params ->list=list;
    pthread_cleanup_push(cleanup_handler_data_mgr, exit_params);
    fileRoom = fopen("room_sensor.map","r");
    FILE_OPEN_ERROR(fileRoom);
    datamgr_parse_sensor_files(fileRoom,list);
    fclose(fileRoom);

    while(1){
        int resultBuffer;

     
        resultBuffer =findBufferNode(param->bufferHead,DATA_MGR_FLAG,param->bufferLocks ,&dataIn);
        while(*(param->tcpOpenFlag )==TRUE && resultBuffer==SBUFFER_NO_DATA){
            #ifdef DEBUG_BUFFER  
                printf("\nwait Data\n");
            #endif 
            int resultLock = pthread_mutex_lock( param->data_mutex ); /// critical secction  
            SYNCRONIZATION_ERROR(resultLock);
            resultLock = pthread_cond_wait(param->myConVar ,param->data_mutex );
            SYNCRONIZATION_ERROR(resultLock);
            resultLock = pthread_mutex_unlock( param->data_mutex  );
            SYNCRONIZATION_ERROR(resultLock);
            #ifdef DEBUG_BUFFER  
                printf("\nawake Data\n");
            #endif  
            resultBuffer =findBufferNode(param->bufferHead,DATA_MGR_FLAG,param->bufferLocks ,&dataIn);
            #ifdef DEBUG_BUFFER  
                printf("buffer outcome code:%d\n",resultBuffer);
            #endif  
        }
    
        if(resultBuffer ==SBUFFER_SUCCESS){
            #ifdef DEBUG_DTA_MGR  
                printf("D:\t%d\t%f\t%ld\n",dataIn.id,dataIn.value,dataIn.ts);
            #endif
            data_element_t* data = get_data_element(dataIn.id,list);
            if(data!=NULL)insertAndShift(dataIn,data,*(param->ptrToFilePtr));
            else LOG_INVALID_SENSOR(dataIn.id,(*(param->ptrToFilePtr)));
        }

        if(*(param->tcpOpenFlag )==FALSE && (resultBuffer ==SBUFFER_NO_DATA)){
            break;
        }
    }
    pthread_cleanup_pop(TRUE);
    return NULL;
}



///////Methods to calculate avarage *************
sensor_value_t* createArray(){
    return (sensor_value_t*) malloc(sizeof(sensor_value_t)*RUN_AVG_LENGTH);
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


void datamgr_parse_sensor_files(FILE *fp_sensor_map, dplist_t * list){ 
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
    return  data;
}
void datamgr_free(dplist_t * list){
    dpl_free(&list,true);
    list =NULL;
}

data_element_t* get_data_element(sensor_id_t sensor_id,dplist_t * list){
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


