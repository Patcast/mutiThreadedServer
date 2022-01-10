/**
 * \author Patricio Adolfo Castillo Calderon
 */


#ifndef _CONFIG_H_
#define _CONFIG_H_

#define _GNU_SOURCE

#define DBCONN sqlite3

#define MAX_CHAR 100
#define COM_CODE 9876
#define FIFO_NAME logFifo 

#include <stdint.h>
#include <time.h>
#include <sqlite3.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <inttypes.h>
#include <poll.h>
#include <signal.h>  // not sure if I need it 
#include <check.h>

#include "errmacros.h"
#include "logGenerator.h"
#include "lib/dplist.h"
#include "lib/tcpsock.h"

#define TRUE             1
#define FALSE            0

#define REAL_TO_STRING(s) #s
#define TO_STRING(s) REAL_TO_STRING(s)    //force macro-expansion on s before stringify s

typedef uint16_t sensor_id_t;
typedef double sensor_value_t;
typedef time_t sensor_ts_t;         // UTC timestamp as returned by time() - notice that the size of time_t is different on 32/64 bit machine

/**
 * structure to hold sensor data
 */
typedef struct {
    sensor_id_t id;         /** < sensor id */
    sensor_value_t value;   /** < sensor value */
    sensor_ts_t ts;         /** < sensor timestamp */
} sensor_data_t;




#endif /* _CONFIG_H_ */
