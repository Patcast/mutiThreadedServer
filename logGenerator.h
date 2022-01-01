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
            