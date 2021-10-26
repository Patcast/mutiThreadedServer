/**
 * \author Patricio Adolfo Castillo Calderon
 */

#include "dplist.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>

void setup(void) {
    // Implement pre-test setup
}

void teardown(void) {
    // Implement post-test teardown
}
START_TEST(test_ListFree)
    {
        // Test free NULL
        dplist_t* list = NULL;
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list
        list = dpl_create();
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");
        // Test free with one element
        list = dpl_create();
        dpl_insert_at_index(list, 'A', 0);
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");
        
        //: Test free with multiple element
	list = dpl_create();
        dpl_insert_at_index(list, 'A', 0);
        dpl_insert_at_index(list, 'A', 1);
        dpl_insert_at_index(list, 'A', 1);
        dpl_insert_at_index(list, 'A', 1);
        dpl_free(&list);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");
    }
END_TEST

START_TEST(test_ListInsertAtIndexListNULL)
    {
        dplist_t* result;
        // Test inserting at index -1
        result = dpl_insert_at_index(NULL, 'A', -1);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        // TODO : Test inserting at index 0
        result = dpl_insert_at_index(NULL, 'A', 0);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        // TODO : Test inserting at index 99
	result = dpl_insert_at_index(NULL, 'A', 99);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
    }
END_TEST

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

START_TEST(test_updateSizeOfHeader)
    {
    	dplist_t* list;
        // Test size with 0 elements 
	list = dpl_create();
        ck_assert_msg (dpl_size(list) == 0, "Failure:expected list to have size of 0, got a size of %d",dpl_size(list));
        dpl_free(&list);
        // Test size with 3 elements
        list = dpl_create();
        dpl_insert_at_index(list, 'A', 0);
        dpl_insert_at_index(list, 'A', 0);
        dpl_insert_at_index(list, 'A', 0);
        ck_assert_msg (dpl_size(list) == 3, "Failure:expected list to have size of 3, got a size of %d",dpl_size(list));
        dpl_free(&list);
    }
END_TEST

START_TEST(test_removeNode)
    {
    	dplist_t* list;
    	dplist_t* result;
    	
    	// Test list NULL
    	list = NULL;
        result = dpl_remove_at_index(list,0);
        ck_assert_msg(result == NULL, "Failure: expected result to be NULL");	
    	// Test remove elemement from empty list 
    	list = dpl_create();
        result= dpl_remove_at_index(list,0);
        ck_assert_msg(result == list, "Failure: expected both pointer to point at the header");
        free(list);
        // Test remove element at index  0
        // Test remove element at index in the middle 
        // Test remove element at index in the end
	
    }
END_TEST

int main(void) {
    Suite *s1 = suite_create("LIST_EX1");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;
    suite_add_tcase(s1, tc1_1);
    
    //tcase_add_checked_fixture(tc1_1, setup, teardown);
    //tcase_add_test(tc1_1, test_ListFree);
    //tcase_add_test(tc1_1, test_ListInsertAtIndexListNULL);
    //tcase_add_test(tc1_1, test_ListInsertAtIndexListEmpty);
    //tcase_add_test(tc1_1, test_updateSizeOfHeader);
     tcase_add_test(tc1_1, test_removeNode);
   
    
    srunner_run_all(sr, CK_VERBOSE);

    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
