#define LOG_ERROR(condition)                         \
    do {                                                                \
            if ((condition)) {                      \
                  fprintf( stderr, "Error writing data to fifo\n"); \
                  exit( EXIT_FAILURE );                         \
            }                     \
        } while(0)                

#define CONNECT_DB(condition, fp)                         \
    do {                                                                \
            if ((condition)) {            \
                LOG_ERROR( fputs( "Connected successfully to db.\n", fp ) == EOF );       \
            } \
            else{ \
                LOG_ERROR( fputs( "Failure trying to connect to db.\n", fp ) == EOF );    \
            }                                                           \
            FFLUSH_ERROR(fflush(fp));                                     \
            } while(0)

#define NEW_TABLE_ON_DB(condition,fp)                         \
    do {                                                                \
            if ((condition)) {            \
            LOG_ERROR( fputs( "A new table was made on the DB.\n", fp ) == EOF );    \
            FFLUSH_ERROR(fflush(fp));                                     \
            }                       \
            } while(0)
            
#define DISCONNECT_DB(fp)                         \
    do {                                                                \
            LOG_ERROR( fputs("Connection with db is closed.\n", fp ) == EOF );    \
            FFLUSH_ERROR(fflush(fp));                                     \
            } while(0)

#define TEMPERATURE_ALARM(fp,data_element)                         \
    do {                                                                \
            if ((data_element->avg>SET_MAX_TEMP)) {                     \
                char* msg;                                   \
                asprintf(&msg,"The room %d is too HOT. Sensor_id %d, Temp avarage %f\n",data_element->idRoom,data_element->idSensor,data_element->avg);        \
                LOG_ERROR( fputs(msg, fp ) == EOF );       \
            } \
            else if ((data_element->avg<SET_MIN_TEMP)){ \
                char* msg;                           \
                asprintf(&msg,"The room %d with Sensor ID %d at time %ld is too COLD. Temp avarage => %f\n",data_element->idRoom,data_element->idSensor,data_element->ts,data_element->avg);        \
                LOG_ERROR( fputs(msg, fp ) == EOF );       \
                }                                                           \
            FFLUSH_ERROR(fflush(fp));                                     \
            } while(0)

#define LOG_INVALID_SENSOR(dataInId,fp)                         \
    do {                                                                \
                char* msg;                                   \
                asprintf(&msg,"Data received from an invalid Sensor. Sensor id: %d.\n",dataInId);        \
                LOG_ERROR( fputs(msg, fp ) == EOF );       \
            } while(0) 

#define NEW_SENSOR(dataInId,fp)                         \
    do {                                                                \
                char* msg;                                   \
                asprintf(&msg,"New sensor connected to server. Sensor id: %d.\n",dataInId);        \
                LOG_ERROR( fputs(msg, fp ) == EOF );       \
            } while(0)

#define SENSOR_DISCONECTED(dataInId,fp)                         \
    do {                                                                \
                char* msg;                                   \
                asprintf(&msg,"Sensor disconnected to server. Sensor id: %d.\n",dataInId);        \
                LOG_ERROR( fputs(msg, fp ) == EOF );       \
            } while(0)
#define SERVER_OPENS_CONNECTION(port,fp)                         \
    do {                                                                \
                char* msg;                                   \
                asprintf(&msg,"TCP socket started and lisening on PORT %d.\n",port);        \
                LOG_ERROR( fputs(msg, fp ) == EOF );       \
            } while(0)



           
            