#include "config.h"
#include "sensor_db.h"
#include <stdlib.h>
#include <stdio.h>

int main (void ){
    DBCONN *db;
    sqlite3_stmt *res;
    
    db = init_connection(1);
    
    int rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);    
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }    
    
    rc = sqlite3_step(res);
    
    if (rc == SQLITE_ROW) {
        printf("%s\n", sqlite3_column_text(res, 0));
    }
    
    sqlite3_finalize(res);
    disconnect(db);
    return 0;
}