/**
 * \author Patricio Adolfo Castillo Calderon
 */

#include "sbuffer.h"

#ifdef DEBUG_BUFFER
    int nodesCreated=0;
    int nodesDeleted=0;
#endif 


int sbuffer_init(sbuffer_t **buffer) {
    *buffer = malloc(sizeof(sbuffer_t));
    if (*buffer == NULL) return SBUFFER_FAILURE;
    (*buffer)->head = NULL;
    (*buffer)->tail = NULL;
    (*buffer)->size = 0;
    return SBUFFER_SUCCESS;
}

int sbuffer_free(sbuffer_t **bufferPtr) {/// I have problem where it does not point to the correct address
    
    if ((bufferPtr == NULL) || (*bufferPtr == NULL)) {
        return SBUFFER_FAILURE;
    }
    #ifdef DEBUG_BUFFER
         int deletedNodes = 0;
    #endif
   
    while ((*bufferPtr) ->head != NULL) {
        sbuffer_node_t *dummy;
        dummy = (*bufferPtr)->head;
        (*bufferPtr)->head = (*bufferPtr)->head->next;
        free(dummy);
        #ifdef DEBUG_BUFFER
            deletedNodes++;
        #endif 
    }
    #ifdef DEBUG_BUFFER  
        printf("\n\nnumber of nodes created %d\n\n",nodesCreated);
        printf("\n\nnumber of nodes Deleted %d\n\n",nodesDeleted);
        printf("\n\nnumber of nodes freed with sbuffer_free %d\n\n",deletedNodes);
    #endif 
   
    free((*bufferPtr));
    (*bufferPtr) = NULL;
    return SBUFFER_SUCCESS;
}


int sbuffer_remove(sbuffer_t *buffer, sbuffer_lock_t* buffer_locks) {
    if (buffer == NULL) return SBUFFER_FAILURE;
    if (buffer->head == NULL) return SBUFFER_NO_DATA;
    
    if(buffer->head->readByStorageMgr== TRUE && buffer->head->readByDataMgr==TRUE){



            #ifdef DEBUG_BUFFER  
                nodesDeleted++;
            #endif
            sbuffer_node_t* node = buffer->head;

            if (buffer->head != buffer->tail){ // buffer has only one node
                #ifdef DEBUG_BUFFER  
                    printf("removing.... buffer has many nodes::\t\t%f\n",(node -> data).value);
                #endif 
                buffer->head = buffer->head->next;
                free(node);
                buffer->size --;
                return SBUFFER_SUCCESS;
            }

            int resultLock = pthread_mutex_lock( buffer_locks->tail_lock); /// critical secction  
            SYNCRONIZATION_ERROR(resultLock);
            if (buffer->head->next == NULL)
            {
                #ifdef DEBUG_BUFFER  
                    printf("removing...buffer has only one node :: \t\t%f\n",(node -> data).value);
                #endif 
                buffer->tail = NULL;
                buffer->head =  NULL;

            } 
            else buffer->head = buffer->head->next; // buffer has many nodes empty      

            free(node);
            buffer->size --;
            resultLock = pthread_mutex_unlock(buffer_locks->tail_lock);
            SYNCRONIZATION_ERROR(resultLock);
            return SBUFFER_SUCCESS;
    }

    return -1;
}

int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data, sbuffer_lock_t* buffer_locks) {
    sbuffer_node_t *dummy;
    if (buffer == NULL) return SBUFFER_FAILURE;
    dummy = malloc(sizeof(sbuffer_node_t));
    MEMORY_ERROR(dummy);    
    dummy->data = *data;
    dummy->next = NULL;
    dummy ->readByDataMgr = FALSE;
    dummy ->readByStorageMgr = FALSE;

    int resultLock = pthread_mutex_lock( buffer_locks->tail_lock); /// critical secction  
    SYNCRONIZATION_ERROR(resultLock);
    if (buffer->tail == NULL) // buffer empty (buffer->head should also be NULL
    {
        resultLock = pthread_rwlock_wrlock(buffer_locks->rw_head_lock);
        SYNCRONIZATION_ERROR(resultLock);
        buffer->head = buffer->tail = dummy;
        resultLock = pthread_rwlock_unlock(buffer_locks->rw_head_lock);
        SYNCRONIZATION_ERROR(resultLock);

    } else // buffer not empty
    {
        buffer->tail->next = dummy;
        buffer->tail = buffer->tail->next;
    }
    resultLock = pthread_mutex_unlock(buffer_locks->tail_lock);
    SYNCRONIZATION_ERROR(resultLock);
    buffer->size++;

    #ifdef DEBUG_BUFFER  
        printf("data added to buffer :: \t\t%f\n",(dummy -> data).value);
        nodesCreated++;
    #endif
    return SBUFFER_SUCCESS;
}

int findBufferNode(sbuffer_t* buffer,char manager_flag, sbuffer_lock_t* buffer_locks, sensor_data_t*data){

    #ifdef DEBUG_BUFFER  
        printf("the size of the buffer when reading thread is finding nodes is %ld -- flag %d \n", buffer->size,manager_flag);
    #endif
    int resultLock = pthread_rwlock_rdlock(buffer_locks->rw_head_lock);
    SYNCRONIZATION_ERROR(resultLock);
    sbuffer_node_t *dummy  = buffer ->head;
    while(dummy!=NULL){
        //printf("i did a loop\n");

        if(manager_flag==DATA_MGR_FLAG && dummy->readByDataMgr ==FALSE){
            dummy->readByDataMgr = TRUE;
            break;
        }
        else if(manager_flag==STORAGE_MGR_FLAG && dummy->readByStorageMgr ==FALSE){
            dummy->readByStorageMgr = TRUE;
            break;
        }
        dummy = dummy ->next;   
    } 
    resultLock = pthread_rwlock_unlock(buffer_locks->rw_head_lock);
    SYNCRONIZATION_ERROR(resultLock);
    if(dummy!= NULL){
        #ifdef DEBUG_BUFFER  
            printf("useful data found\n");
        #endif

        *data = dummy->data;
        if(dummy->readByStorageMgr== TRUE && dummy->readByDataMgr==TRUE){
            resultLock = pthread_rwlock_wrlock(buffer_locks->rw_head_lock);
            SYNCRONIZATION_ERROR(resultLock);
            sbuffer_remove(buffer,buffer_locks);
            
            resultLock = pthread_rwlock_unlock(buffer_locks->rw_head_lock);
            SYNCRONIZATION_ERROR(resultLock);
        } 
        return SBUFFER_SUCCESS;
    }
    else return SBUFFER_NO_DATA;
}


