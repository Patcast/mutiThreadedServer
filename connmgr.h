#include "sbuffer.h"


#define MAX	80
#define MAX_CONN 30
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

void* connmgr_listen(void * thread_params);
void connmgr_free(dplist_t* mylist, pollfd_t** ptrToFds);
void* element_copy(void * element);
void element_free(void** element);
int element_compare(void * x, void * y);
int addNewFd(pollfd_t** ptrToFds,dplist_t * list,socket_sensor_t* socket);
int removeFd(pollfd_t** ptrToFds,dplist_t * list, int index);
int validateSockets(dplist_t* list,pollfd_t** ptrToFds);