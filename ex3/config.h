/**
 * \author Patricio Adolfo Castillo Calderon
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include <time.h>
/// data element 
typedef uint16_t std_int_t;
typedef time_t std_time_t;  // UTC timestamp as returned by time() - notice that the size of time_t is different on 32/64 bit machine

/// extra for sensor data 
typedef double sensor_value_t;

/**
 * structure to hold sensor data
 */
typedef struct {
    std_int_t id;         /** < sensor id */
    sensor_value_t value;   /** < sensor value */
    std_time_t ts;         /** < sensor timestamp */
} sensor_data_t;

/**
 * structure to hold data elements
 */
typedef struct {
    std_int_t idSensor;        
    std_int_t idTemp; 
    sensor_value_t avg; 
    std_time_t ts;
    std_int_t numRecAdded;
    sensor_value_t * ptrToRecords;
} data_element_t;



#endif /* _CONFIG_H_ */
