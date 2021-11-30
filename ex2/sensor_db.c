#include "sensor_db.h"

DBCONN* init_connection(char clear_up_flag){
    DBCONN* db;
    int rc = sqlite3_open( TO_STRING(DB_NAME), &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        ///normally program should be shut down if error. Here does not happen.
    }
    return db;
}

void disconnect(DBCONN *conn){
    sqlite3_close(conn);
}