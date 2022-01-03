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



/*
 *  This method holds the core functionality of your datamgr. It takes in 2 file pointers to the sensor files and parses them. 
 *  When the method finishes all data should be in the internal pointer list and all log messages should be printed to stderr.
 *  \param fp_sensor_map file pointer to the map file
 *  \param fp_sensor_data file pointer to the binary data file
 */
void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data);

/**
 * This method should be called to clean up the datamgr, and to free all used memory. 
 * After this, any call to datamgr_get_room_id, datamgr_get_avg, datamgr_get_last_modified or datamgr_get_total_sensors will not return a valid result
 */
void datamgr_free();

/**
 * Gets the room ID for a certain sensor ID
 * Use ERROR_HANDLER() if sensor_id is invalid
 * \param sensor_id the sensor id to look for
 * \return the corresponding room id
 */
uint16_t datamgr_get_room_id(sensor_id_t sensor_id);

/**
 * Gets the running AVG of a certain senor ID (if less then RUN_AVG_LENGTH measurements are recorded the avg is 0)
 * Use ERROR_HANDLER() if sensor_id is invalid
 * \param sensor_id the sensor id to look for
 * \return the running AVG of the given sensor
 */
sensor_value_t datamgr_get_avg(sensor_id_t sensor_id);

/**
 * Returns the time of the last reading for a certain sensor ID
 * Use ERROR_HANDLER() if sensor_id is invalid
 * \param sensor_id the sensor id to look for
 * \return the last modified timestamp for the given sensor
 */
time_t datamgr_get_last_modified(sensor_id_t sensor_id);

/**
 *  Return the total amount of unique sensor ID's recorded by the datamgr
 *  \return the total amount of sensors
 */
int datamgr_get_total_sensors();

data_element_t* get_data_element(sensor_id_t sensor_id);

void startManager();
void datamgr_parse_room (FILE *fp_sensor_map);
void process_sensor_data(FILE * fp_sensor_data);
void insertAndShift(sensor_data_t valueToInstert, data_element_t* data_element,FILE* fp);
sensor_value_t calculateAvg(sensor_value_t* array);
void* dataManager(void * thread_param_input);
void* element_copy_dta_magr(void * element);
void element_free_dta_magr(void** element);
int element_compare_dta_magr(void * x, void * y);
void* createNewData(sensor_id_t idS,sensor_id_t idR);
sensor_value_t* createArray();
void printArray();

#endif  //DATAMGR_H_
