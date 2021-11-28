/**
 * \author Patricio Adolfo Castillo Calderon
 */

#include "config.h"  //tools to handle tada from sensors
#include "datamgr.h"  
#include "dplist.h"
#include <stdlib.h>
#include <stdio.h>
void* element_copy(void * element);
void element_free(void** element);
/** Uses the property id to compare to my_element_t object.
 * \param x a pointer to the list
 * \param y a pointer to the list
 * \return An integer, if x(id) SMALLER than y(id) returns -1, 0 if equal and 1 if x(id) is BIGGER than y(id).
 */
int element_compare(void * x, void * y);
void* createNewData(std_int_t idS,std_int_t idT, std_int_t avgInput, std_time_t tsInput);

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
    copy -> idSensor = ((data_element_t*)element)->idSensor;
    copy -> idTemp = ((data_element_t*)element)->idTemp;
    copy -> avg = ((data_element_t*)element)->avg;
    copy -> ts = ((data_element_t*)element)->ts;
    return (void *) copy;
}

void element_free(void** ptrElemenet) {
    free(*ptrElemenet);
    *ptrElemenet = NULL;
}

int element_compare(void * x, void * y) {
    return ((((data_element_t*)x)->idSensor < ((data_element_t*)y)->idSensor) ? -1 : (((data_element_t*)x)->idSensor == ((data_element_t*)y)->idSensor) ? 0 : 1);
}

void test(){
        dplist_t* list; 
        void* data1 = createNewData(0,0,10,1638097803); 
        void* data2 = createNewData(1,1,12,1638097803);  
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);      
        dpl_free(&list,true);	
} 

int main(void) {
    test(); 	
    return  0; 
}


