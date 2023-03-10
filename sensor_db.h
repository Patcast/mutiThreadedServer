/**
 * \author Patricio Adolfo Castillo Calderon
 */



#ifndef _SENSOR_DB_H_
#define _SENSOR_DB_H_

#include "sbuffer.h"

// stringify preprocessor directives using 2-level preprocessor magic
// this avoids using directives like -DDB_NAME=\"some_db_name\"
// #define REAL_TO_STRING(s) #s
// #define TO_STRING(s) REAL_TO_STRING(s)    //force macro-expansion on s before stringify s

#ifndef DB_NAME
#define DB_NAME Sensor.db
#endif

#ifndef MAKE_NEW_TABLE
#define MAKE_NEW_TABLE FALSE
#endif

#ifndef TABLE_NAME
#define TABLE_NAME SensorData
#endif

typedef struct paramsStorageExit{
    DBCONN * db;
    thread_parameters_t* param;
}exit_params_t;



void cleanup_handler_storage(void *arg);
typedef int (*callback_t)(void *, int, char **, char **);

/**
 * Make a connection to the database server
 * Create (open) a database with name DB_NAME having 1 table named TABLE_NAME  
 * \param clear_up_flag if the table existed, clear up the existing data when clear_up_flag is set to 1
 * \return the connection for success, NULL if an error occurs
 */
DBCONN *init_connection(char clear_up_flag);

/**
 * Disconnect from the database server
 * \param conn pointer to the current connection
 */
void disconnect(DBCONN *conn);

/**
 * Write an INSERT query to insert a single sensor measurement
 * \param conn pointer to the current connection
 * \param id the sensor id
 * \param value the measurement value
 * \param ts the measurement timestamp
 * \return zero for success, and non-zero if an error occurs
 */
int insert_sensor(DBCONN *conn, sensor_id_t id, sensor_value_t value, sensor_ts_t ts);

int executeSQL(DBCONN** db,char* sql,callback_t f);

int callback(void *, int, char **, char **);

void *storageManager(void * thread_param_input);

#endif /* _SENSOR_DB_H_ */



