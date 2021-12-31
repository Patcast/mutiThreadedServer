#include "../headers/log_generation.h"

void log_code_generator (log_code_t log,FILE* fp)
{ 
    char* send_buf;  
    switch( log ) {
        case  CONNECTED_DB:
            asprintf(&send_buf, "Connected successfully to db.\n");
            
            break;
        case NOT_CONNECTED:
            asprintf(&send_buf, "Failure trying to connect to db.\n");
            exit(EXIT_FAILURE);
            break;   
        case  FILE_OPEN:
            asprintf(&send_buf, "File open successfully.\n");
            
            break;
        case FILE_FAILURE:
            asprintf(&send_buf, "Failure trying to open file.\n");
            perror("My error is");
            break;           
        default: // should never come here
            
        assert( 1==0 );
    }
    if ( fputs( send_buf, fp ) == EOF )
    {
        fprintf( stderr, "Error writing data to fifo\n");
        exit( EXIT_FAILURE );
    } 
    fflush(fp);
    FFLUSH_ERROR(fflush(fp));
    printf("Message send: %s\n", send_buf); 
    free( send_buf );
    sleep(1);
}

void startsLogFileGenerator(){
    FILE* fp;
    int recordNumber=0;
    FILE *fp_log;
    char recv_buf[MAX_CHAR];
    char *str_result;
    int result;

    result = mkfifo(FIFO_NAME, 0666);
    CHECK_MKFIFO(result); 
    fp = fopen(FIFO_NAME, "r"); 
    printf("syncing with writer ok\n");
    FILE_OPEN_ERROR(fp);

    fp_log = fopen("filesAndData/gateway.log", "w");
    FILE_OPEN_ERROR(fp_log);

    do 
    {
        str_result = fgets(recv_buf, MAX_CHAR, fp);
        if ( str_result != NULL )
        { 
        printf("message from storageManager :\t%d\t%lu\t%s\n",recordNumber,time(NULL),str_result);
        fprintf(fp_log,"message from storageManager :\t%d\t%lu\t%s\n",recordNumber,time(NULL),str_result);
        recordNumber++;
        }
    } while ( str_result != NULL ); 

    result = fclose( fp );
    FILE_CLOSE_ERROR(result);
    result = fclose(fp_log);
    FILE_CLOSE_ERROR(result);
    SYSCALL_ERROR(result);  
}