/**
 * \author Patricio Adolfo Castillo Calderon
 */

#define _GNU_SOURCE  
#include <stdbool.h>
#include "dplist.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct {
    int id;
    char* name;
} my_element_t;

void* element_copy(void * element);
void element_free(void* element);
/** Uses the property id to compare to my_element_t object.
 * \param x a pointer to the list
 * \param y a pointer to the list
 * \return An integer, if x(id) SMALLER than y(id) returns -1, 0 if equal and 1 if x(id) is BIGGER than y(id).
 */
int element_compare(void * x, void * y);
void* createNewData(int id, char name);

//********************************************* Methods 

void* createNewData(int idInput, char charInput){
    my_element_t* data = malloc(sizeof (my_element_t));
    data ->id = idInput;
    data ->name = malloc(sizeof (char));
    *(data ->name )= charInput;
    return (void*) data;
}

void * element_copy(void * element) {
    my_element_t* copy = malloc(sizeof (my_element_t));
    char* new_name;
    asprintf(&new_name,"%s",((my_element_t*)element)->name); 
    assert(copy != NULL);
    copy->id = ((my_element_t*)element)->id;
    copy->name = new_name;
    return (void *) copy;
}

void element_free(void* element) {
    free((((my_element_t*)element))->name);
    free(element);
    element = NULL;
}

int element_compare(void * x, void * y) {
    return ((((my_element_t*)x)->id < ((my_element_t*)y)->id) ? -1 : (((my_element_t*)x)->id == ((my_element_t*)y)->id) ? 0 : 1);
}

void setup(void) {
    // Implement pre-test setup
}

void teardown(void) {
    // Implement post-test teardown
}
/* 
START_TEST(test_ListFree)
    {
        // Test free NULL, don't use callback
        dplist_t* list;
        list = NULL;
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free NULL, use callback
        list = NULL;
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, don't use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL"); 
        // TODO : Test free with one element, also test if inserted elements are set to NULL
        list = dpl_create(element_copy, element_free, element_compare);
        //void* element = createNewElement(2,'A');
        list = dpl_insert_at_index(list,NULL,0,false);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");
        // TODO : Test free with multiple element, also test if inserted elements are set to NULL
        list = dpl_create(element_copy, element_free, element_compare);
        void* data = createNewData(2,'A'); // It may crash if I add multiple nodes with same data.
        list = dpl_insert_at_index(list,NULL,0,true);
        list = dpl_insert_at_index(list,data,0,false);
        // TODO : Invalid read of size 1(error from valgrind)
        list = dpl_insert_at_index(list,data,0,true);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

    }
    END_TEST
 */
START_TEST(test_removeNode)
    {
    	dplist_t* list;
    	dplist_t* result;
    	
    	// // Test list NULL
    	// list = NULL;
        // result = dpl_remove_at_index(list,0,false);
        // ck_assert_msg(result == NULL, "Failure: expected result to be NULL");
        // // Test remove elemement from empty list 
        // list = dpl_create(element_copy, element_free, element_compare);
        // result= dpl_remove_at_index(list,0,false);
        // ck_assert_msg(result == list, "Failure: expected both pointer to point at the header");
        // dpl_free(&list,true);	
        
        //   // Test remove element at index  0 with multiple elements
        // list = dpl_create(element_copy, element_free, element_compare);
        // dpl_insert_at_index(list, 'A', 1,false);
        // dpl_insert_at_index(list, 'B', 2,false);
        // dpl_insert_at_index(list, 'C', 3,false);
        // result= dpl_remove_at_index(list,10,false);
        // ck_assert_msg(dpl_get_element_at_index(result,0) == 'B', "Failure, expected %c, but got %c",dpl_get_element_at_index(list,1),dpl_get_element_at_index(result,0));
        // dpl_free(&list,true);
        // Test remove element at index in the middle 
        // Test remove element at index in the end
         void* data1 = createNewData(2,'A'); // It may crash if I add multiple nodes with same data.
        void* data2 = createNewData(4,'A'); // It may crash if I add multiple nodes with same data.
        void* data3 = createNewData(5,'A'); // It may crash if I add multiple nodes with same data.
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);
        dpl_insert_at_index(list, data3, 3,false);
        result= dpl_remove_at_index(list,10,false);
        ck_assert_msg (dpl_size(list) == 2, "Failure:expected list to have size of 3, got a size of %d",dpl_size(list));
        //ck_assert_msg(dpl_get_element_at_index(result,1) == 'B', "Failure, expected %c, but got %c",dpl_get_element_at_index(list,1),dpl_get_element_at_index(result,0));
        dpl_free(&list,true);
	
    }
END_TEST

int main(void) {
    Suite *s1 = suite_create("LIST_EX3");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_checked_fixture(tc1_1, setup, teardown);
    // tcase_add_test(tc1_1, test_ListFree);
    tcase_add_test(tc1_1, test_removeNode);

    // Add other tests here...

    srunner_run_all(sr, CK_VERBOSE);

    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
