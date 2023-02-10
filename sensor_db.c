#define _GNU_SOURCE
#include "sensor_db.h"

void cleanup_handler_storage(void *arg){
    #ifdef DEBUG_STORAGE_MGR  
         printf("Storage Manager is closing...\n");
    #endif 
    exit_params_t* exit_params = arg;
    if (exit_params->db != NULL){
        disconnect(exit_params ->db);
        exit_params ->db = NULL;
        DISCONNECT_DB((*(exit_params->param->ptrToFilePtr)));   
    }
    else {
        SERVER_FAIL_DB_CONNECTION((*(exit_params->param->ptrToFilePtr)));
    } 
    pthread_cancel(*(exit_params->param->tcp_thread));
    free(exit_params);
    pthread_exit(NULL);   
}

///////* Main Method***************///////
void* storageManager(void * thread_param_input){

    sensor_data_t dataInPtr;
    thread_parameters_t* param = thread_param_input;
    DBCONN *db;
    exit_params_t * exit_params;
    exit_params = malloc(sizeof(exit_params_t));
    MEMORY_ERROR(exit_params);
    exit_params ->param=param;


    pthread_cleanup_push(cleanup_handler_storage, exit_params);
    db = init_connection(MAKE_NEW_TABLE);
    CONNECT_DB((db!=NULL),(*(param->ptrToFilePtr)));
    exit_params->db = db;
    NEW_TABLE_ON_DB(MAKE_NEW_TABLE,*(param->ptrToFilePtr));
    while(1){
        
        int resultBuffer, resultLock;
       

        resultBuffer =findBufferNode(param->bufferHead,STORAGE_MGR_FLAG,param->bufferLocks ,&dataInPtr);
        while(*(param->tcpOpenFlag )==TRUE && resultBuffer==SBUFFER_NO_DATA){
            #ifdef DEBUG_STORAGE_MGR  
                printf("\nwait Storage\n");             
            #endif

            resultLock = pthread_mutex_lock( param->data_mutex ); /// critical secction  
            SYNCRONIZATION_ERROR(resultLock);
            int resultLock = pthread_cond_wait(param->myConVar ,param->data_mutex );
            SYNCRONIZATION_ERROR(resultLock);
            resultLock = pthread_mutex_unlock( param->data_mutex  );
            SYNCRONIZATION_ERROR(resultLock);

            #ifdef DEBUG_STORAGE_MGR  
                printf("\nawake Storage\n");             
            #endif
            resultBuffer =findBufferNode(param->bufferHead,STORAGE_MGR_FLAG,param->bufferLocks ,&dataInPtr);
        }
     
        if(resultBuffer ==SBUFFER_SUCCESS){
            #ifdef DEBUG_STORAGE_MGR  
                printf("S:\t%d\t%f\t%ld\n",dataInPtr.id,dataInPtr.value,dataInPtr.ts);
            #endif
            insert_sensor(db,dataInPtr.id,dataInPtr.value,dataInPtr.ts);
            #ifdef DEBUG_BUFFER  
                sleep(3);
            #endif
        }


        if(*(param->tcpOpenFlag )==FALSE && !(resultBuffer ==SBUFFER_SUCCESS))break;
        
    } 
    pthread_cleanup_pop(TRUE);
    return NULL;
}



////////********* UTILITY METHODS **************///////////////

DBCONN* init_connection(char clear_up_flag){
    DBCONN* db;
    char count = 0;
    // while(sqlite3_open( TO_STRING(DB_NAME), &db)!=SQLITE_OK && count<3){  
    while(sqlite3_open( TO_STRING(DB_NAME), &db)!=SQLITE_OK && count<3){  
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        db = NULL;
        count ++;
        sleep(2);
    }
    if(db==NULL) return NULL;

    
    char* sql;
    if(clear_up_flag==1){

        asprintf(&sql,"DROP TABLE IF EXISTS %s;" "CREATE TABLE %s (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,sensor_id sINTEGER NOT NULL,sensor_value	DECIMAL(4,2) NOT NULL,timestamp	TIMESTAMP NOT NULL);",TO_STRING(TABLE_NAME),TO_STRING(TABLE_NAME));
    }
    else{
        asprintf(&sql,"CREATE TABLE IF NOT EXISTS %s (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,sensor_id sINTEGER NOT NULL,sensor_value	DECIMAL(4,2) NOT NULL,timestamp	TIMESTAMP NOT NULL);",TO_STRING(TABLE_NAME)); 
    }
    if (executeSQL(&db, sql,NULL)!=0){
           db=NULL;
       } 
    return db;
}

void disconnect(DBCONN *conn){
    sqlite3_close(conn);        
}


int executeSQL(DBCONN** db,char* sql,callback_t f){
    char *err_msg = 0;
    int rc;
    if(f!=NULL){
        rc = sqlite3_exec(*db, sql, f, 0, &err_msg);
    }
    else {
        rc = sqlite3_exec(*db, sql, 0, 0, &err_msg);
    }

    
    if (rc != SQLITE_OK ) {   
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);      
    } 
    free(sql);
    return rc;
}

int insert_sensor(DBCONN *conn, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    char* sql;
    asprintf(&sql,"INSERT INTO %s (sensor_id,sensor_value,timestamp) VALUES( %d, %f,%ld);",TO_STRING(TABLE_NAME),id,value,ts); 
    return executeSQL(&conn,sql,NULL);
}










