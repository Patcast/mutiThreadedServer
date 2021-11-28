
/**
 * \author Patricio Adolfo Castillo Calderon
 */

#define _GNU_SOURCE
#include "config.h"  //tools to handle tada from sensors
#include "datamgr.h"  
#include "dplist.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


void* element_copy(void * element);
void element_free(void** element);
/** Uses the property id to compare to my_element_t object.
 * \param x a pointer to the list
 * \param y a pointer to the list
 * \return An integer, if x(id) SMALLER than y(id) returns -1, 0 if equal and 1 if x(id) is BIGGER than y(id).
 */
int element_compare(void * x, void * y);
void* createNewData(int id, char name);

//********************************************* Methods 

void* createNewData(std_int_t idS,std_int_t idT, std_int_t avgInput, std_time_t tsInput){
    data_element_t* data = malloc(sizeof (data_element_t));
    data -> idSensor = idS;
    data -> idTemp = idT;
    data -> avg = avgInput;
    data -> ts = tsInput;
    return (void*) data;
}

void * element_copy(void * element) {
    data_element_t* copy = malloc(sizeof (data_element_t));
    // char* new_name;
    // asprintf(&new_name,"%s",((my_element_t*)element)->ptrToData); 
    // assert(copy != NULL);
    copy->id = ((my_element_t*)element)->id;
   
    copy -> idSensor = ((data_element_t*)element)->idSensor;
    copy -> idTemp = ((data_element_t*)element)->idTemp;
    copy -> avg = ((data_element_t*)element)->avg;
    copy -> ts = ((data_element_t*)element)->ts;
    return (void *) copy;
}

void element_free(void** ptrElemenet) {/// you might wanna send the address of the pointer, so you can set it to zero
    //free((((my_element_t*) *ptrElemenet))->ptrToData);
    free(*ptrElemenet);
    *ptrElemenet = NULL;
}

int element_compare(void * x, void * y) {
    return ((((data_element_t*)x)->idSensor < ((data_element_t*)y)->idSensor) ? -1 : (((data_element_t*)x)->idSensor == ((data_element_t*)y)->idSensor) ? 0 : 1);
}

/* void test(){
        dplist_t* list; 
        void* element; 
        dplist_node_t* reference; 
        char storedChar;
        // Test get element at index  0 with multiple elements
        void* data1 = createNewData(2,'A'); 
        void* data2 = createNewData(4,'B'); 
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);
        reference = dpl_get_reference_at_index(list,0);// assume this method works 
        element = dpl_get_element_at_reference(list,reference);
        storedChar = *(((my_element_t*)element)->ptrToData);
        printf("\nThe char on element is expected to be A and it is -> %c\n\n",storedChar);
        //ck_assert_msg(storedChar == 'A', "Failure, expected 'A', but got %c",storedChar);
        reference = dpl_get_reference_at_index(list,1);
        element = dpl_get_element_at_reference(list,reference);
        storedChar = *(((my_element_t*)element)->ptrToData);
        printf("\nThe char on element is expected to be B and it is ->  %c\n\n",storedChar);        
        dpl_free(&list,true);	
} */

int main(void) {
    test(); 	
    return  0; 
}