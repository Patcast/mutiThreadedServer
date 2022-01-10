/**
 * \author Patricio Adolfo Castillo Calderon
 */

#ifndef DATAMGR_H_
#define DATAMGR_H_

#include "sbuffer.h"

#ifndef RUN_AVG_LENGTH
#define RUN_AVG_LENGTH 5
#endif

#ifndef SET_MAX_TEMP
#error SET_MAX_TEMP not set
#endif

#ifndef SET_MIN_TEMP
#error SET_MIN_TEMP not set
#endif


typedef struct {
    sensor_id_t idSensor;        
    sensor_id_t idRoom; 
    sensor_value_t avg; 
    sensor_ts_t ts;
    sensor_id_t numRecAdded;
    sensor_value_t * ptrToRecords;
} data_element_t;

typedef struct paramsDataExit{
    dplist_t* list;
    thread_parameters_t* param;
}exit_params_data_t;



/*
 *  This method holds the core functionality of your datamgr. It takes in 2 file pointers to the sensor files and parses them. 
 *  When the method finishes all data should be in the internal pointer list and all log messages should be printed to stderr.
 *  \param fp_sensor_map file pointer to the map file
 *  \param fp_sensor_data file pointer to the binary data file
 */
void datamgr_parse_sensor_files(FILE *fp_sensor_map,dplist_t * list);

/**
 * This method should be called to clean up the datamgr, and to free all used memory. 
 * After this, any call to datamgr_get_room_id, datamgr_get_avg, datamgr_get_last_modified or datamgr_get_total_sensors will not return a valid result
 */
void datamgr_free(dplist_t * list);

data_element_t* get_data_element(sensor_id_t sensor_id,dplist_t * list);

void startManager();
void insertAndShift(sensor_data_t valueToInstert, data_element_t* data_element,FILE* fp);
sensor_value_t calculateAvg(sensor_value_t* array);
void* dataManager(void * thread_param_input);
void* element_copy_dta_magr(void * element);
void element_free_dta_magr(void** element);
int element_compare_dta_magr(void * x, void * y);
void* createNewData(sensor_id_t idS,sensor_id_t idR);
sensor_value_t* createArray();

#endif  //DATAMGR_H_
