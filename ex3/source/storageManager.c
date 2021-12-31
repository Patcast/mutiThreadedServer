
#include "../headers/storageManager.h"
#include "../headers/log_generation.h"



void storageManager(){
    FILE *fp; 

    int result = mkfifo(FIFO_NAME, 0666);
    CHECK_MKFIFO(result); 	
    fp = fopen(FIFO_NAME, "w"); 
    printf("syncing with reader ok\n");
    //FILE_OPEN_ERROR(fp);

    DBCONN *db;
    db = init_connection(0);
    if(db==NULL) log_code_generator(NOT_CONNECTED,fp);
    else log_code_generator(CONNECTED_DB,fp);
    FILE* fileSensor = fopen("filesAndData/sensor_data","r");
    if (fileSensor == NULL ) log_code_generator(FILE_FAILURE,fp);
    else log_code_generator(FILE_OPEN,fp);
    find_sensor_by_value(db,15.0,callback);
    
    fclose( fp );
    // result = fclose( fp );
    //FILE_CLOSE_ERROR(result);
    fclose(fileSensor);
    disconnect(db); 
    //close(pfds[1]);// this will tell the parent that no more messages will be sent. 
    exit(EXIT_SUCCESS);
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