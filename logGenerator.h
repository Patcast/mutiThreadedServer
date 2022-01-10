#define LOG_ERROR(condition)                         \
    do {                                                                \
            if ((condition)) {                      \
                  fprintf( stderr, "Error writing data to fifo\n"); \
                  pthread_exit(NULL);                         \
            }                     \
        } while(0)                

#define CONNECT_DB(condition, fp)                         \
    do {                                                                \
            if ((condition)) {            \
                LOG_ERROR( fputs( "Connected successfully to db.\n", fp ) == EOF );       \
            } \
            else{ \
                LOG_ERROR( fputs( "Failure trying to connect to db.\n", fp ) == EOF );    \
                pthread_exit(NULL);\
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
            char* msg1;                                   \
            if ((data_element->avg>SET_MAX_TEMP)) {                     \
                asprintf(&msg1,"The room %d is too HOT. Sensor_id %d, Temp avarage %f\n",data_element->idRoom,data_element->idSensor,data_element->avg);        \
                LOG_ERROR( fputs(msg1, fp ) == EOF );       \
                free(msg1);\
            } \
            else if ((data_element->avg<SET_MIN_TEMP)){                           \
                asprintf(&msg1,"The room %d with Sensor ID %d at time %ld is too COLD. Temp avarage => %f\n",data_element->idRoom,data_element->idSensor,data_element->ts,data_element->avg);        \
                LOG_ERROR( fputs(msg1, fp ) == EOF );       \
                free(msg1);\
            }                                                 \
            FFLUSH_ERROR(fflush(fp));                                     \
            } while(0)

#define LOG_INVALID_SENSOR(dataInId,fp)                         \
    do {                                                                \
                char* msg2;                                   \
                asprintf(&msg2,"Data received from an invalid Sensor. Sensor id: %d.\n",dataInId);        \
                LOG_ERROR( fputs(msg2, fp ) == EOF );       \
                free(msg2); \
            FFLUSH_ERROR(fflush(fp));                                     \
            } while(0) 

#define NEW_SENSOR(dataInId,fp)                         \
    do {                                                                \
                char* msg3;                                   \
                asprintf(&msg3,"New sensor connected to server. Sensor id: %d.\n",dataInId);        \
                LOG_ERROR( fputs(msg3, fp ) == EOF );       \
                free(msg3);\
                FFLUSH_ERROR(fflush(fp));   \
            } while(0)

#define SENSOR_DISCONECTED(dataInId,fp)                         \
    do {                                                                \
                char* msg4;                                   \
                asprintf(&msg4,"Sensor disconnected to server. Sensor id: %d.\n",dataInId);        \
                LOG_ERROR( fputs(msg4, fp ) == EOF );       \
                free(msg4);\
                FFLUSH_ERROR(fflush(fp));   \
            } while(0)
#define SERVER_OPENS_CONNECTION(port,fp)                         \
    do {                                                                \
                char* msg5;                                   \
                asprintf(&msg5,"TCP socket started and lisening on PORT %d.\n",port);        \
                LOG_ERROR( fputs(msg5, fp ) == EOF );       \
                free(msg5);\
                FFLUSH_ERROR(fflush(fp));   \
            } while(0)
#define SERVER_FAIL_DB_CONNECTION(fp)                         \
    do {                                                                \
                LOG_ERROR( fputs("Program ended, since the server could not connect with the data base.\n", fp ) == EOF );       \
                FFLUSH_ERROR(fflush(fp));   \
            } while(0)
            



           
            