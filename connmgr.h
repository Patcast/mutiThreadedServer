#include "sbuffer.h"


#define INDEX_OF_SERVER 0


#ifndef TIMEOUT    
#error TIMEOUT not set
#endif


typedef  struct pollfd pollfd_t;
typedef struct socket_sensor{
    sensor_ts_t ts;
    tcpsock_t*  tcpPointer;
    sensor_id_t id; 
}socket_sensor_t;

typedef struct paramsConExit{
    dplist_t** list;
    pollfd_t** fds;
}exit_params_conn_t;

void* connmgr_listen(void * thread_params);
void* element_copy(void * element);
void element_free(void** element);
int element_compare(void * x, void * y);
int addNewFd(pollfd_t** ptrToFds,dplist_t * list,socket_sensor_t* socket);
int removeFd(pollfd_t** ptrToFds,dplist_t * list, int index);
int validateSockets(dplist_t* list,pollfd_t** ptrToFds);