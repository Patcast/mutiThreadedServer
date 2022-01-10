/**
 * \author Patricio Adolfo Castillo Calderon
 */

#ifndef _SBUFFER_H_
#define _SBUFFER_H_

#include "config.h"

#define SBUFFER_FAILURE -1
#define SBUFFER_SUCCESS 0
#define SBUFFER_NO_DATA 1
#define SBUFFER_SUCCESS_AND_DELETE 2
#define SBUFFER_SUCCESS_AND_NO_DELETE 3

#define STORAGE_MGR_FLAG 111
#define DATA_MGR_FLAG 120
#define TRUE 1
#define FALSE 0

/**
 * basic node for the buffer, these nodes are linked together to create the buffer
 */
typedef struct sbuffer_node {
    struct sbuffer_node *next;  /**< a pointer to the next node*/
    sensor_data_t data;         /**< a structure containing the data */
    char readByStorageMgr;      //Flags use to determine if both threads have read the data. 
    char readByDataMgr;
} sbuffer_node_t;

typedef struct sbuffer_locks{
    pthread_rwlock_t * rw_head_lock;
    pthread_mutex_t *  tail_lock;
} sbuffer_lock_t;

/**
 * a structure to keep track of the buffer
 */
typedef struct sbuffer {
    sbuffer_node_t *head;       /**< a pointer to the first node in the buffer */
    sbuffer_node_t *tail;       /**< a pointer to the last node in the buffer */
    long int size;
}sbuffer_t;

typedef struct {
    pthread_mutex_t* data_mutex;         
    pthread_cond_t*  myConVar;
    char* tcpOpenFlag; 
    sbuffer_t* bufferHead;
    FILE ** ptrToFilePtr;
    int portNumber;
    pthread_t* tcp_thread;
    sbuffer_lock_t* bufferLocks;
} thread_parameters_t;

/**
 * Allocates and initializes a new shared buffer
 * \param buffer a double pointer to the buffer that needs to be initialized
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occurred
 */
int sbuffer_init(sbuffer_t **buffer);

/**
 * All allocated resources are freed and cleaned up
 * \param buffer a double pointer to the buffer that needs to be freed
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occurred
 */
int sbuffer_free(sbuffer_t **buffer);

/**
 * Removes the first sensor data in 'buffer' (at the 'head') and returns this sensor data as '*data'
 * If 'buffer' is empty, the function doesn't block until new sensor data becomes available but returns SBUFFER_NO_DATA
 * \param buffer a pointer to the buffer that is used
 * \param data a pointer to pre-allocated sensor_data_t space, the data will be copied into this structure. No new memory is allocated for 'data' in this function.
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occurred
 */
int sbuffer_remove(sbuffer_t *buffer, sbuffer_lock_t* buffer_locks);

/**
 * Inserts the sensor data in 'data' at the end of 'buffer' (at the 'tail')
 * \param buffer a pointer to the buffer that is used
 * \param data a pointer to sensor_data_t data, that will be copied into the buffer
 * \return SBUFFER_SUCCESS on success and SBUFFER_FAILURE if an error occured
*/
int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data, sbuffer_lock_t* buffer_locks);

int findBufferNode(sbuffer_t* buffer,char manager_flag, sbuffer_lock_t* buffer_locks, sensor_data_t*data);


#endif  //_SBUFFER_H_
