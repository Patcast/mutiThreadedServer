/**
 * \author Patricio Adolfo Castillo Calderon
 */
// #include <stdbool.h>
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

void deleteNode (dplist_t* list,dplist_node_t* ptr,bool deleteElement);


struct dplist_node {
    dplist_node_t *prev, *next;
    void* element;
};

struct dplist {
    dplist_node_t* head;
    int size;
    void* (*element_copy)(void *src_element);
    void (*element_free)(void* *element);
    int (*element_compare)(void *x, void *y);
};



dplist_t *dpl_create(// callback functions
        void *(*element_copy)(void *src_element),
        void (*element_free)(void **element),
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
				else {
                    list->head = dummy->next;
                    dummy ->next ->prev = NULL;	
                    }
			}
			else if (dummy->next == NULL) dummy->prev->next = NULL;	// this is the last element
			
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

    void* ptrToData = NULL;
	if ( list !=NULL){
		dplist_node_t* dummy = dpl_get_reference_at_index(list,index); 
		if (dummy !=NULL) ptrToData = (dummy-> element);
	}
	return ptrToData;
}

int dpl_get_index_of_element(dplist_t *list, void *element) {//no possible to return NULL if list is null, it returns -1.

    int index = -1;
	dplist_node_t* dummy = dpl_get_reference_at_index(list,0); 
		
	if (dummy !=NULL && element!=NULL){//if list is null or empty this will be false. 
            for (int count = 0; count < (list ->size); dummy = dummy->next, count++) {
        		if ((list->element_compare)(element,(dummy->element))==0){
        			index = count;
        			return index;
        		}
    		}
	}	
    return index;
}

dplist_node_t* dpl_get_reference_at_index(dplist_t *list, int index) {
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
     bool referenceExist = false;
     if(list!=NULL&& reference != NULL){
        if(list->size>0){
            dplist_node_t* dummy = list->head;
            for (int count = 0; count < (list ->size); dummy = dummy->next, count++) {
                    if(dummy==reference){
                        referenceExist=true;
                        break;
                        }
       		}
        }
     }
     if(referenceExist&&reference->element!=NULL)return  reference->element ;
     else return NULL;
}


void deleteNode (dplist_t* list,dplist_node_t* dummy,bool deleteElement){
	if (dummy!=NULL){
		if ((dummy->element!=NULL && list->element_free != NULL)&&deleteElement){

             (list->element_free)(&(dummy ->element));
        }
		free(dummy);
	}
}

//***** EXTRA MEHTHODS ********************************///

// dplist_node_t* dpl_get_first_reference(dplist_t *list){
//     return dpl_get_reference_at_index(list,0);      
// }
// dplist_node_t* dpl_get_last_reference(dplist_t *list){
//     int size = dpl_size(list);
//     if (size>0){
//         dplist_node_t* ref = dpl_get_reference_at_index(list,size-1); 
//         return ref;
//     }
//     else return NULL;
// }

// dplist_node_t* dpl_get_next_reference(dplist_t *list, dplist_node_t* reference){
//     int index = dpl_get_index_of_reference(list,reference);
//     if(index>=0)return reference->next;
//     else return NULL;
// }
// dplist_node_t *dpl_get_previous_reference(dplist_t *list, dplist_node_t *reference){
//     int index = dpl_get_index_of_reference(list,reference);
//     if(index>=0)return reference->prev;
//     else return NULL;
// }
// dplist_node_t *dpl_get_reference_of_element(dplist_t *list, void *element){
//     dplist_node_t* node =NULL;
//     if (element!=NULL){    
//         dplist_node_t* dummy = dpl_get_reference_at_index(list,0); 	
//         if (dummy !=NULL){
//                 for (int count = 0; count < (list ->size); dummy = dummy->next, count++) {
//                     if ((list->element_compare)(element,(dummy->element))==0){
//                         node = dummy;
//                         break;
//                     }
//                 }
//         }
//     }
//     return node;
// }

// int dpl_get_index_of_reference(dplist_t *list, dplist_node_t *reference){
//     int index = -1;
// 	dplist_node_t* dummy = dpl_get_reference_at_index(list,0); 
		
// 	if (dummy != NULL && reference != NULL){ 
//             for (int count = 0; count < (list ->size); dummy = dummy->next, count++) {
//         		if (dummy == reference){
//         			index = count;
//         			return index;
//         		}
//     		}
// 	}	
//     return index;
// }
// // To be implemented later if needed...
// //dplist_t *dpl_insert_sorted(dplist_t *list, void *element, bool insert_copy);
// //dplist_t *dpl_insert_at_reference(dplist_t *list, void *element, dplist_node_t *reference, bool insert_copy);

// dplist_t *dpl_remove_at_reference(dplist_t *list, dplist_node_t *reference, bool free_element){
//     dplist_t* returnList = NULL;
//     if (list!=NULL && reference!=NULL){
//         returnList = list;
//         int index = dpl_get_index_of_reference(list,reference);
//         if (index>=0){
//             dpl_remove_at_index(list,index,free_element);
//         }
//     }
//     return returnList;
// }
// dplist_t *dpl_remove_element(dplist_t *list, void *element, bool free_element){
//     dplist_t* returnList = NULL;
//     if (list!=NULL && element!=NULL){
//         returnList = list;
//         int index = dpl_get_index_of_element(list,element);
//         if (index>=0){
//             dpl_remove_at_index(list,index,free_element);
//         }
//     }
//     return returnList;
// }

