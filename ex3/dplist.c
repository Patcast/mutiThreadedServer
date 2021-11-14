/**
 * \author Patricio Adolfo Castillo Calderon
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"

/*
 * definition of error codes
 * */
#define DPLIST_NO_ERROR 0
#define DPLIST_MEMORY_ERROR 1 // error due to mem alloc failure
#define DPLIST_INVALID_ERROR 2 //error due to a list operation applied on a NULL list 

#ifdef DEBUG
#define DEBUG_PRINTF(...) 									                                        \
        do {											                                            \
            fprintf(stderr,"\nIn %s - function %s at line %d: ", __FILE__, __func__, __LINE__);	    \
            fprintf(stderr,__VA_ARGS__);								                            \
            fflush(stderr);                                                                         \
                } while(0)
#else
#define DEBUG_PRINTF(...) (void)0
#endif


#define DPLIST_ERR_HANDLER(condition, err_code)                         \
    do {                                                                \
            if ((condition)) DEBUG_PRINTF(#condition " failed\n");      \
            assert(!(condition));                                       \
        } while(0)


/*
 * The real definition of struct list / struct node
 */

struct dplist_node {
    dplist_node_t *prev, *next;
    void *element;
};

struct dplist {
    dplist_node_t* head;
    int size;
    void* (*element_copy)(void *src_element);
    void (*element_free)(void *element);
    int (*element_compare)(void *x, void *y);
};


dplist_t *dpl_create(// callback functions
        void *(*element_copy)(void *src_element),
        void (*element_free)(void *element),
        int (*element_compare)(void *x, void *y)
) {
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_MEMORY_ERROR);
    list->head = NULL;
    list->element_copy = element_copy;
    list->element_free = element_free;
    list->element_compare = element_compare;
    list->size = 0;
    return list;
}

void dpl_free(dplist_t **list, bool free_element) {
    if ( *list !=NULL){	
		if ((*list)->size > 0){
			dplist_node_t* dummy;
			dplist_node_t* oldDummy;
			dummy = (*list)->head;
			while(dummy->next != NULL){		
				oldDummy = dummy; 
				dummy = dummy -> next;
				deleteNode(*list,oldDummy,free_element);
            }
		deleteNode(*list,dummy,free_element);
		}
		free(*list);
		*list = NULL;
	}	
}

dplist_t *dpl_insert_at_index(dplist_t *list, void* element, int index, bool insert_copy) {
    dplist_node_t* ref_at_index;
	dplist_node_t* list_node;

	if (list == NULL) return NULL;
	list_node = malloc(sizeof(dplist_node_t));
	DPLIST_ERR_HANDLER(list_node == NULL, DPLIST_MEMORY_ERROR);
    if(insert_copy&&element!=NULL) list_node->element = (list->element_copy)(element);
    else list_node->element = element;

	//(*(list->createPtr))(element);

    if (list->head == NULL) { // covers case 1
        list_node->prev = NULL;
        list_node->next = NULL;
        list->head = list_node;
    } 
	else if (index <= 0) { // covers case 2
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
    } 
	else {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        if (index < dpl_size(list)) { // covers case 4
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;} 
		else { // covers case 3
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;}
    }
	list->size = (list -> size) +1;
    return list;
    

}

dplist_t* dpl_remove_at_index(dplist_t *list, int index, bool free_element) {
    if ( list !=NULL){
		dplist_node_t* dummy = dpl_get_reference_at_index(list,index); 
		
		if (dummy !=NULL){// list is not empty 
			if (dummy->prev == NULL){ // first node in the list 
				if (dummy->next == NULL)list->head = NULL;	
				else list->head = dummy->next;	
			}
			else if (dummy->next == NULL) dummy->prev->next = NULL;	
			
			else{
				dummy->prev->next = dummy -> next;
				dummy->next->prev = dummy -> prev;
			}
			deleteNode(list,dummy,free_element);
			list->size --;	
		}
	}
	return list;
}

int dpl_size(dplist_t *list) {
    if (list != NULL) return list-> size;
    else return -1;

}

void* dpl_get_element_at_index(dplist_t *list, int index) {

    //TODO: add your code here

}

int dpl_get_index_of_element(dplist_t *list, void *element) {

    //TODO: add your code here

}

dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index) {

    //DPLIST_ERR_HANDLER(list == NULL, DPLIST_INVALID_ERROR);
    dplist_node_t* dummy = NULL;
     int count;
    
    if (list != NULL && list->head != NULL){
    	if (index <0 ) index =0; 
    	for (dummy = list->head, count = 0; dummy->next != NULL; dummy = dummy->next, count++) {
        	if (count >= index) return dummy;
    	}
    }
    return dummy;
}

void* dpl_get_element_at_reference(dplist_t *list, dplist_node_t* reference) {

    //TODO: add your code here

}
void deleteNode (dplist_t* list,dplist_node_t* ptr,bool deleteElement){
	if (ptr!=NULL){
		if (ptr->element!=NULL && list->element_free != NULL){
			if(deleteElement&& ptr->element!=NULL){
                (list->element_free)(ptr->element);
            }
		}
		free(ptr);
		ptr = NULL;
	}

}

