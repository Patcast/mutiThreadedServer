/**
 * \author Patricio Adolfo Castillo Calderon
 */

#include "dplist.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>


//********** 

element_t* createPtrToData(element_t input);
void deleteData (element_t* ptr);

//**********

void setup(void) {
    // Implement pre-test setup
}

void teardown(void) {
    // Implement post-test teardown
}
START_TEST(test_createHeader)
    {
    	dplist_t* list;
   	list = dpl_create(&createPtrToData,&deleteData);

    	//ck_assert_msg(result== 'A', "Failure: expected list to have value %c, got %c",*valueA,result);
    	dpl_free(&list);
    }
END_TEST

/*
START_TEST(test_ListInsertAtIndexListEmpty)
{
    dplist_t* list;
    dplist_t* result;
    // Test inserting at index -1
    list = dpl_create();
    result = dpl_insert_at_index(list, 'A', -1);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",dpl_size(result));
    dpl_free(&list);
    result = NULL;
    //Test inserting at index 0
    list = dpl_create();
    result = dpl_insert_at_index(list, 'A', 0);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",dpl_size(result));
    dpl_free(&list);
    result = NULL;

    // Test inserting at index 99
    list = dpl_create();
    result = dpl_insert_at_index(list, 'A', 99);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",dpl_size(result));
    dpl_free(&list);
    result = NULL;
}
END_TEST

*/

int main(void) {
    Suite *s1 = suite_create("LIST_EX1");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;
    suite_add_tcase(s1, tc1_1);
    /// tests here 
 	  
	tcase_add_checked_fixture(tc1_1, setup, teardown);
	tcase_add_test(tc1_1, test_createHeader);
   
   //// 
    srunner_run_all(sr, CK_VERBOSE);

    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return nf == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

element_t* createPtrToData(element_t input){
	element_t* newCharPtr;
	newCharPtr = malloc(sizeof(element_t));
	*newCharPtr = input;
	return newCharPtr;
}

void deleteData (element_t* ptr){
	free(ptr);
}
