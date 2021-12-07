#include "config.h"
#include "sensor_db.h"



int callback(void *, int, char **, char **);

int main (void ){
    DBCONN *db;
    db = init_connection(0);
    if(db==NULL) return 1;
    // FILE* fileSensor = fopen("sensor_data","r");
    // if (fileSensor == NULL ){// I need to add includes for errors
    //       perror("My error is");
    // }
    // insert_sensor_from_file(db,fileSensor );
    //find_sensor_by_value(db,15.0,callback);
    //find_sensor_exceed_value(db,24,callback);
    find_sensor_after_timestamp(db,1638097863,callback);
    // fclose(fileSensor);
    disconnect(db); 
    return 0;
}

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

