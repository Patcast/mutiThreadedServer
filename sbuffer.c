/**
 * \author Patricio Adolfo Castillo Calderon
 */

#include <stdlib.h>
#include <stdio.h>
#include "sbuffer.h"

int sbuffer_init(sbuffer_t **buffer) {
    *buffer = malloc(sizeof(sbuffer_t));
    if (*buffer == NULL) return SBUFFER_FAILURE;
    (*buffer)->head = NULL;
    (*buffer)->tail = NULL;
    return SBUFFER_SUCCESS;
}

int sbuffer_free(sbuffer_t **buffer) {
    sbuffer_node_t *dummy;
    if ((buffer == NULL) || (*buffer == NULL)) {
        return SBUFFER_FAILURE;
    }
    while ((*buffer)->head) {
        dummy = (*buffer)->head;
        (*buffer)->head = (*buffer)->head->next;
        free(dummy);
    }
    free(*buffer);
    *buffer = NULL;
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
        //printf("----------------dummy read\n");
        if (findingResut ==SBUFFER_SUCCESS_AND_DELETE){
            ///steps to delete data.
            
            if (buffer->head == buffer->tail) // buffer has only one node
            {
                buffer->head = buffer->tail = NULL;
            } else  // buffer has many nodes empty
            {
                buffer->head = buffer->head->next;
            }
            //printf("--------------dummey deleted\n");
            free(dummy);
        } 
        return SBUFFER_SUCCESS;
    }
}

int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data) {
    sbuffer_node_t *dummy;
    if (buffer == NULL) return SBUFFER_FAILURE;
    dummy = malloc(sizeof(sbuffer_node_t));
    if (dummy == NULL) return SBUFFER_FAILURE;
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
        
        if(dummy->readByStorageMgr==dummy->readByDataMgr)return SBUFFER_SUCCESS_AND_DELETE;
        else return SBUFFER_SUCCESS_AND_NO_DELETE;
    }
    else return SBUFFER_NO_DATA;

}


