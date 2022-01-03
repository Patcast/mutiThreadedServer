/**
 * \author Patricio Adolfo Castillo Calderon
 */

#include "sbuffer.h"

int nodesCreated;
int nodesDeleted;

int sbuffer_init(sbuffer_t **buffer) {
    *buffer = malloc(sizeof(sbuffer_t));
    if (*buffer == NULL) return SBUFFER_FAILURE;
    (*buffer)->head = NULL;
    (*buffer)->tail = NULL;
    return SBUFFER_SUCCESS;
}

int sbuffer_free(sbuffer_t **bufferPtr) {/// I have problem where it does not point to the correct address
    
    if ((bufferPtr == NULL) || (*bufferPtr == NULL)) {
        return SBUFFER_FAILURE;
    }
    sbuffer_node_t *dummy;
    int deletedNodes;
    while ((*bufferPtr) ->head != NULL) {
        dummy = (*bufferPtr)->head;
        (*bufferPtr)->head = (*bufferPtr)->head->next;
        free(dummy);
        deletedNodes++;
    }
    printf("\n\nnumber of nodes created %d\n\n",nodesCreated);
    printf("\n\nnumber of nodes Deleted %d\n\n",nodesDeleted);
    printf("\n\nnumber of nodes freed with sbuffer_free %d\n\n",deletedNodes);
    free((*bufferPtr));
    (*bufferPtr) = NULL;
    return SBUFFER_SUCCESS;
}



int sbuffer_remove(sbuffer_t *buffer, sensor_data_t* data,char manager_flag) {
    
    sbuffer_node_t *dummy;
    if (buffer == NULL) return SBUFFER_FAILURE;
    if (buffer->head == NULL) return SBUFFER_NO_DATA;
    // find dummy with no flag set;
    int findingResut = findBufferNode(&dummy, buffer, manager_flag);
    if(findingResut ==SBUFFER_NO_DATA)return SBUFFER_NO_DATA;
    
    else{
        *data = dummy->data;
        #ifdef DEBUG_BUFFER  
            printf("buffer found this: \t%f\n",dummy->data.value);
        #endif 
        if (findingResut ==SBUFFER_SUCCESS_AND_DELETE){
            ///steps to delete data.
            
            if (buffer->head == buffer->tail) // buffer has only one node
            {
                #ifdef DEBUG_BUFFER  
                    printf("\nremoving last element on buffer\n");
                #endif 
                buffer->head =  NULL;
                buffer->tail = NULL;
            } else  // buffer has many nodes empty
            {
                #ifdef DEBUG_BUFFER  
                    printf("\nremoving middle element on buffer\n");
                #endif 
                buffer->head = buffer->head->next;
            }
            free(dummy);
            nodesDeleted++;
            #ifdef DEBUG_BUFFER  
                printf("data deleted from buffer\n");
            #endif
        } 
        else if(findingResut ==SBUFFER_SUCCESS_AND_NO_DELETE){
            #ifdef DEBUG_BUFFER  
                printf("data only taken from buffer\n");
            #endif
        } 
        else MEMORY_ERROR(NULL);
        return SBUFFER_SUCCESS;
    }
}

int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data) {
    sbuffer_node_t *dummy;
    if (buffer == NULL) return SBUFFER_FAILURE;
    dummy = malloc(sizeof(sbuffer_node_t));
    MEMORY_ERROR(dummy);
    nodesCreated++;
    dummy->data = *data;
    dummy->next = NULL;
    dummy ->readByDataMgr = FALSE;
    dummy ->readByStorageMgr = FALSE;
    if (buffer->tail == NULL) // buffer empty (buffer->head should also be NULL
    {
        buffer->head = buffer->tail = dummy;
    } else // buffer not empty
    {
        buffer->tail->next = dummy;
        buffer->tail = buffer->tail->next;
    }
    return SBUFFER_SUCCESS;
}

int isBufferEmpty(sbuffer_t *buffer){
    int value;
    if(buffer->head ==NULL)value = SBUFFER_NO_DATA;
    else value = SBUFFER_SUCCESS;
    return value;
}

int findBufferNode(sbuffer_node_t ** ptrDummy, sbuffer_t* buffer,char manager_flag){
    sbuffer_node_t *dummy  = buffer ->head;

    while(dummy!=NULL){

        if(manager_flag==DATA_MGR_FLAG && dummy->readByDataMgr ==FALSE){
            dummy->readByDataMgr = TRUE;
            break;
        }
        else if(manager_flag==STORAGE_MGR_FLAG && dummy->readByStorageMgr ==FALSE){
            dummy->readByStorageMgr = TRUE;
            break;
        }
        dummy = dummy ->next;   
    } /// end dummy == NULL if not found 

    if(dummy!= NULL){
        *ptrDummy = dummy;
        
        if(dummy->readByStorageMgr== TRUE && dummy->readByDataMgr==TRUE)return SBUFFER_SUCCESS_AND_DELETE;
        else return SBUFFER_SUCCESS_AND_NO_DELETE;
    }
    else return SBUFFER_NO_DATA;

}


