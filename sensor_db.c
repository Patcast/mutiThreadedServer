#define _GNU_SOURCE
#include "sensor_db.h"




///////* Main Method***************///////

void cleanup_handler_storage(void *arg){
    /// there is not release of the mutex_lock.
    #ifdef DEBUG_STORAGE_MGR  
         printf("Storage is closing...\n");
    #endif 
    thread_parameters_t* param = arg;
    if (param->db != NULL){
        disconnect(param ->db);
        param ->db = NULL;
        printf("\n\n\ndb closed correctly\n\n");
    }
    else {
        printf("\n\n\ndb closed incorrectly\n\n");
        SERVER_FAIL_DB_CONNECTION((*(param->ptrToFilePtr)));
        pthread_cancel(*(param->tcp_thread));
    } 
    DISCONNECT_DB((*(param->ptrToFilePtr))); 
}


void* storageManager(void * thread_param_input){

    int resultBuffer, resultLock;
    sensor_data_t dataInPtr;
    thread_parameters_t* param = thread_param_input;
    DBCONN *db;
    pthread_cleanup_push(cleanup_handler_storage, thread_param_input);
    db = init_connection(MAKE_NEW_TABLE);
    param->db = db;
    CONNECT_DB((db!=NULL),(*(param->ptrToFilePtr)));
    NEW_TABLE_ON_DB(MAKE_NEW_TABLE,*(param->ptrToFilePtr));
    while(1){
        

        resultLock = pthread_mutex_lock( param->data_mutex ); /// critical secction  
        SYNCRONIZATION_ERROR(resultLock);

        resultBuffer =sbuffer_remove(param->bufferHead ,&dataInPtr,STORAGE_MGR_FLAG);
        while(*(param->tcpOpenFlag )==TRUE && resultBuffer==SBUFFER_NO_DATA){
            //printf("\nwait Storage\n");
            int resultLock = pthread_cond_wait(param->myConVar ,param->data_mutex );
            SYNCRONIZATION_ERROR(resultLock);
            //printf("\nawake Storage\n");
            resultBuffer =sbuffer_remove(param->bufferHead,&dataInPtr,STORAGE_MGR_FLAG);
        }
        #ifdef DEBUG_STORAGE_MGR  
                printf("S:\t%d\t%f\t%ld\n",dataInPtr.id,dataInPtr.value,dataInPtr.ts);
        #endif
        resultLock = pthread_mutex_unlock( param->data_mutex  );
        SYNCRONIZATION_ERROR(resultLock);
        if(resultBuffer ==SBUFFER_SUCCESS){
            insert_sensor(db,dataInPtr.id,dataInPtr.value,dataInPtr.ts);
        }
        if(*(param->tcpOpenFlag )==FALSE && !(resultBuffer ==SBUFFER_SUCCESS))break;
    } 
    pthread_cleanup_pop(TRUE);
    return NULL;
}



////////********* UTILITY METHODS **************///////////////



int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    // arg: number of columns
    // argv: array of strings representing fields in the row
    // azColName: array of column names 
    NotUsed = 0;
    for (int i = 0; i < argc; i++) {
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


DBCONN* init_connection(char clear_up_flag){
    DBCONN* db;
    char count;
    // while(sqlite3_open( TO_STRING(DB_NAME), &db)!=SQLITE_OK && count<3){  
    while(sqlite3_open( TO_STRING(DB_NAME), &db)!=SQLITE_OK && count<3){  
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        db = NULL;
        count ++;
        sleep(2);
    }
    if(db==NULL) pthread_exit(NULL);

    
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

int insert_sensor_from_file(DBCONN *conn, FILE *sensor_data){
    sensor_data_t dataIn;
    int rc=0;
    while(feof(sensor_data)== 0){
        fread(&(dataIn.id),sizeof(sensor_id_t),1,sensor_data);
        fread(&(dataIn.value),sizeof(sensor_value_t),1,sensor_data);
        fread(&(dataIn.ts),sizeof(sensor_ts_t),1,sensor_data);
        rc = insert_sensor(conn,dataIn.id,dataIn.value,dataIn.ts);
        if (rc!=0)return rc;
    }
    return rc;
}

int find_sensor_all(DBCONN *conn, callback_t f){
    char* sql;
    asprintf(&sql,"SELECT * FROM %s;",TO_STRING(TABLE_NAME)); 
    return executeSQL(&conn,sql,f);
}

int find_sensor_by_value(DBCONN *conn, sensor_value_t value, callback_t f){
    
    char* sql;
    //  This is limited to only using table SensorsData. 
    asprintf(&sql,"SELECT*FROM %s WHERE sensor_value =%f;",TO_STRING(TABLE_NAME),value); 
    return executeSQL(&conn,sql,f);

}
int find_sensor_exceed_value(DBCONN *conn, sensor_value_t value, callback_t f){
    char* sql;
    //  This is limited to only using table SensorsData. 
    asprintf(&sql,"SELECT*FROM %s WHERE sensor_value > %f;",TO_STRING(TABLE_NAME),value); 
    return executeSQL(&conn,sql,f);
}

int find_sensor_by_timestamp(DBCONN *conn, sensor_ts_t ts, callback_t f){
    char* sql;
    //  This is limited to only using table SensorsData. 
    asprintf(&sql,"SELECT*FROM %s WHERE timestamp = %ld;",TO_STRING(TABLE_NAME),ts); 
    return executeSQL(&conn,sql,f);
}

int find_sensor_after_timestamp(DBCONN *conn, sensor_ts_t ts, callback_t f){
    char* sql;
    //  This is limited to only using table SensorsData. 
    asprintf(&sql,"SELECT*FROM %s WHERE timestamp > %ld;",TO_STRING(TABLE_NAME),ts); 
    return executeSQL(&conn,sql,f);

}








