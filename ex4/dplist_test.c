/**
 * \author Patricio Adolfo Castillo Calderon
 */

#define _GNU_SOURCE  
// #include <stdbool.h>
#include "dplist.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct {
    int id;
    char* ptrToData;
} my_element_t;

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

void* createNewData(int idInput, char charInput){
    my_element_t* data = malloc(sizeof (my_element_t));
    data ->id = idInput;
    data ->ptrToData = malloc(sizeof (char));
    *(data ->ptrToData )= charInput;
    return (void*) data;
}

void * element_copy(void * element) {
    my_element_t* copy = malloc(sizeof (my_element_t));
    char* new_name;
    asprintf(&new_name,"%s",((my_element_t*)element)->ptrToData); 
    assert(copy != NULL);
    copy->id = ((my_element_t*)element)->id;
    copy->ptrToData = new_name;
    return (void *) copy;
}

void element_free(void** ptrElemenet) {/// you might wanna send the address of the pointer, so you can set it to zero
    free((((my_element_t*) *ptrElemenet))->ptrToData);
    free(*ptrElemenet);
    *ptrElemenet = NULL;
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
/*  START_TEST(test_ListFree)
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
    END_TEST */
 
/* START_TEST(test_removeNode)
    {
    	dplist_t* list;    	
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
        void* data1 = createNewData(0,'A'); 
        void* data2 = createNewData(1,'B'); 
        // void* data3 = createNewData(2,'C');
        // void* data4 = createNewData(3,'A'); 
        // void* data5 = createNewData(4,'B'); 
        // void* data6 = createNewData(5,'C');
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 0,false);
        dpl_insert_at_index(list, data2, 1,false);
        // dpl_insert_at_index(list, data3, 2,false);
        // dpl_insert_at_index(list, data4, 3,false);
        // dpl_insert_at_index(list, data5, 4,false);
        // dpl_insert_at_index(list, data6, 5,false);
        list= dpl_remove_at_index(list,0,true);
        list= dpl_remove_at_index(list,1,true);
        // list= dpl_remove_at_index(list,3,true);
        // list= dpl_remove_at_index(list,4,true);
        // list= dpl_remove_at_index(list,5,true);
        ck_assert_msg (dpl_size(list) == 0, "Failure:expected list to have size of 0, got a size of %d",dpl_size(list));
        //ck_assert_msg(dpl_get_element_at_index(result,1) == 'B', "Failure, expected %c, but got %c",dpl_get_element_at_index(list,1),dpl_get_element_at_index(result,0));
        dpl_free(&list,true);
    }
END_TEST   */
/* START_TEST(test_dpl_get_element_at_index)
    {
    	dplist_t* list; 
        void* element;   	
    	// Test list NULL
    	list = NULL;
        element = dpl_get_element_at_index(list,0);
        ck_assert_msg(element == NULL, "Failure: expected result to be NULL");
        //Test remove elemement from empty list 
        list = dpl_create(element_copy, element_free, element_compare);
        element = dpl_get_element_at_index(list,0);
        ck_assert_msg(element == NULL, "Failure: expected result to be NULL");
        dpl_free(&list,true);	

        // Test remove element at index  0 with multiple elements
        void* data1 = createNewData(2,'A'); // It may crash if I add multiple nodes with same data.
        void* data2 = createNewData(4,'B'); // It may crash if I add multiple nodes with same data.
        void* data3 = createNewData(5,'C'); // It may crash if I add multiple nodes with same data.
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);
        dpl_insert_at_index(list, data3, 3,false);
        element = dpl_get_element_at_index(list,1);
        char storedChar = *(((my_element_t*)element)->ptrToData);
        ck_assert_msg(storedChar == 'B', "Failure, expected 'B', but got %c",storedChar);
        dpl_free(&list,true);
        // Test remove element at index in the middle 
        // Test remove element at index in the end
        void* data4 = createNewData(2,'A'); // It may crash if I add multiple nodes with same data.
        void* data5 = createNewData(4,'B'); // It may crash if I add multiple nodes with same data.
        void* data6 = createNewData(5,'C'); // It may crash if I add multiple nodes with same data.
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data4, 1,false);
        dpl_insert_at_index(list, data5, 2,false);
        dpl_insert_at_index(list, data6, 3,false);
        element = dpl_get_element_at_index(list,10);
        char storedChar2 = *(((my_element_t*)element)->ptrToData);
        ck_assert_msg(storedChar2 == 'C', "Failure, expected 'B', but got %c",storedChar);
        dpl_free(&list,true);

    }
END_TEST  */
/*START_TEST(test_get_index_of_element)
    {
    	dplist_t* list; 
        int indexOut;
     
    	// // Test list NULL
    	// list = NULL;
        // indexOut = dpl_get_index_of_element(list,NULL);
        // ck_assert_msg(indexOut == -1, "Failure: expected result to be NULL");
        // // //Test remove elemement from empty list 
        // list = dpl_create(element_copy, element_free, element_compare);
        // indexOut = dpl_get_index_of_element(list,NULL);
        // ck_assert_msg(indexOut == -1, "Failure: expected result to be NULL");
        // dpl_free(&list,true);	

        // // // Test look for element with multiple elements
        // void* data1 = createNewData(2,'A');
        // void*data2 = createNewData(4,'B'); 
        // void*data3 = createNewData(5,'C'); 
        // list = dpl_create(element_copy, element_free, element_compare);
        // dpl_insert_at_index(list, data1, 1,false);
        // dpl_insert_at_index(list, data2, 2,false);
        // dpl_insert_at_index(list, data3, 3,false);
        // indexOut = dpl_get_index_of_element(list,data1);
        // ck_assert_msg(indexOut == 2, "Failure: expected result to be NULL");
        // dpl_free(&list,true);

        // // Test remove element at index in the end
        // void* data4 = createNewData(2,'A');
        // void* data5 = createNewData(4,'B'); 
        // void* data6 = createNewData(5,'C'); 
        // list = dpl_create(element_copy, element_free, element_compare);
        // dpl_insert_at_index(list, data4, 1,false);
        // dpl_insert_at_index(list, data5, 2,false);
        // dpl_insert_at_index(list, data6, 3,false);
        // indexOut = dpl_get_index_of_element(list,data6);
        // ck_assert_msg(indexOut == 2, "Failure: expected result to be 2, but got %d",indexOut);
        // dpl_free(&list,true);
        // // Test with multiple and NULL input element. 
        // void* data7 = createNewData(2,'A');
        // void* data8 = createNewData(4,'B'); 
        // void* data9 = createNewData(5,'C'); 
        // list = dpl_create(element_copy, element_free, element_compare);
        // dpl_insert_at_index(list, data7, 1,false);
        // dpl_insert_at_index(list, data8, 2,false);
        // dpl_insert_at_index(list, data9, 3,false);
        // indexOut = dpl_get_index_of_element(list,NULL);
        // ck_assert_msg(indexOut == -1, "Failure: expected result to be 2, but got %d",indexOut);
        // dpl_free(&list,true);

    }
END_TEST
 */
START_TEST(test_get_element_at_reference)
    {
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
        ck_assert_msg(storedChar == 'A', "Failure, expected 'A', but got %c",storedChar);
        reference = dpl_get_reference_at_index(list,1);
        element = dpl_get_element_at_reference(list,reference);
        storedChar = *(((my_element_t*)element)->ptrToData);
        ck_assert_msg(storedChar == 'B', "Failure, expected 'B', but got %c",storedChar);
        dpl_free(&list,true);	

	    // Test list NULL
        void* data4 = createNewData(2,'A'); // It may crash if I add multiple nodes with same data.
        void* data5 = createNewData(4,'B'); // It may crash if I add multiple nodes with same data.
        void* data6 = createNewData(5,'C'); // It may crash if I add multiple nodes with same data.
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data4, 1,false);
        dpl_insert_at_index(list, data5, 2,false);
        dpl_insert_at_index(list, data6, 3,false);
        reference = dpl_get_reference_at_index(list,0);
        element = dpl_get_element_at_reference(NULL,reference);
        ck_assert_msg(element == NULL, "Failure: expected result to be NULL");
        dpl_free(&list,true);	
        }
END_TEST 
/* START_TEST(test_get_first_reference)
    {
    	dplist_t* list; 
        dplist_node_t* reference; 
        my_element_t* element;

    	// Test list NULL
    	list = NULL;
        reference = dpl_get_first_reference(list);
        ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        //Test remove elemement from empty list 
        list = dpl_create(element_copy, element_free, element_compare);
         reference = dpl_get_first_reference(list);
        ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        dpl_free(&list,true);	
        // Test get first wit multiple elements 
        void* data1 = createNewData(2,'A'); 
        void* data2 = createNewData(4,'B'); 
        void* data3 = createNewData(5,'C'); 
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);
        dpl_insert_at_index(list, data3, 3,false);
        reference = dpl_get_first_reference(list);
        element = dpl_get_element_at_reference(list,reference);
        char storedChar = *(((my_element_t*)element)->ptrToData);
        ck_assert_msg(storedChar == 'A', "Failure, expected 'B', but got %c",storedChar);
        dpl_free(&list,true);	
    }
END_TEST */
/* START_TEST(test_get_index_of_reference)
    {
    	dplist_t* list; 
        dplist_node_t* reference; 
        my_element_t* element;
        int index;
    	// // Test list NULL
    	// list = NULL;
        // reference = dpl_get_index_of_reference(list,NULL);
        // ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        // // //Test remove elemement from empty list 
        // // list = dpl_create(element_copy, element_free, element_compare);
        // // reference = dpl_get_last_reference(list);
        // // ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        // // dpl_free(&list,true);	
        // Test get first wit multiple elements 

        void* data1 = createNewData(2,'A'); 
        void* data2 = createNewData(4,'B'); 
        void* data3 = createNewData(5,'C'); 
        void* data4 = createNewData(2,'A'); 
        void* data5 = createNewData(4,'B'); 
        void* data6 = createNewData(5,'C'); 
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);
        dpl_insert_at_index(list, data3, 3,false);
        dpl_insert_at_index(list, data4, 4,false);
        dpl_insert_at_index(list, data5, 5,false);
        dpl_insert_at_index(list, data6, 6,false);
        reference = dpl_get_first_reference(list);
        index = dpl_get_index_of_reference(list,NULL);
        ck_assert_msg(index == -1, "Failure, expected -1, but got %d",index);
        dpl_free(&list,true);	
    }
END_TEST */
/* START_TEST(test_get_next_reference)
    {
    	dplist_t* list; 
        dplist_node_t* reference; 
        my_element_t* element;
        int index;

        dplist_t* listTwo; 
        dplist_node_t* referenceTwo; 
        my_element_t* elementTwo;
        int indexTwo;
    	// // Test list NULL
    	// list = NULL;
        // reference = dpl_get_index_of_reference(list,NULL);
        // ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        // // //Test remove elemement from empty list 
        // // list = dpl_create(element_copy, element_free, element_compare);
        // // reference = dpl_get_last_reference(list);
        // // ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        // // dpl_free(&list,true);	
        // Test get first wit multiple elements 

        void* data1 = createNewData(2,'A'); 
        void* data2 = createNewData(4,'B'); 
        void* data3 = createNewData(5,'C'); 
        void* data4 = createNewData(2,'A'); 
        void* data5 = createNewData(4,'B'); 
        void* data6 = createNewData(5,'C'); 
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);
        dpl_insert_at_index(list, data3, 3,false);
        dpl_insert_at_index(list, data4, 4,false);
        dpl_insert_at_index(list, data5, 5,false);
        dpl_insert_at_index(list, data6, 6,false);

        void* data11 = createNewData(2,'A'); 
        void* data12 = createNewData(4,'B'); 
        void* data13 = createNewData(5,'C'); 
        listTwo = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(listTwo, data11, 1,false);
        dpl_insert_at_index(listTwo, data12, 2,false);
        dpl_insert_at_index(listTwo, data13, 3,false);

        reference = dpl_get_last_reference(list);
        referenceTwo = dpl_get_next_reference(listTwo,reference);
        ck_assert_msg(referenceTwo == NULL, "Failure: expected result to be NULL");
        //element = dpl_get_element_at_reference(list,referenceTwo);
        //char storedChar = *(((my_element_t*)element)->ptrToData);
        //ck_assert_msg(storedChar == -1, "Failure, expected -1, but got %d",storedChar);
        dpl_free(&list,true);	
        dpl_free(&listTwo,true);	

    }
END_TEST */
/* START_TEST(test_get_last_reference)
    {
    	dplist_t* list; 
        dplist_node_t* reference; 
        my_element_t* element;

    	// Test list NULL
    	list = NULL;
        reference = dpl_get_last_reference(list);
        ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        //Test remove elemement from empty list 
        list = dpl_create(element_copy, element_free, element_compare);
        reference = dpl_get_last_reference(list);
        ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        dpl_free(&list,true);	
        // Test get first wit multiple elements 
        void* data1 = createNewData(2,'A'); 
        void* data2 = createNewData(4,'B'); 
        void* data3 = createNewData(5,'C'); 
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);
        dpl_insert_at_index(list, data3, 3,false);
        reference = dpl_get_last_reference(list);
        element = dpl_get_element_at_reference(list,reference);
        char storedChar = *(((my_element_t*)element)->ptrToData);
        ck_assert_msg(storedChar == 'C', "Failure, expected 'C', but got %c",storedChar);
        dpl_free(&list,true);	
    }
END_TEST */
/* START_TEST(test_get_previous_reference)
    {
    	dplist_t* list; 
        dplist_node_t* reference; 
        my_element_t* element;
        int index;

        dplist_t* listTwo; 
        dplist_node_t* referenceTwo; 
        my_element_t* elementTwo;
        int indexTwo;
    	// // Test list NULL
    	// list = NULL;
        // reference = dpl_get_index_of_reference(list,NULL);
        // ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        // // //Test remove elemement from empty list 
        // // list = dpl_create(element_copy, element_free, element_compare);
        // // reference = dpl_get_last_reference(list);
        // // ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        // // dpl_free(&list,true);	
        // Test get first wit multiple elements 

        void* data1 = createNewData(2,'A'); 
        void* data2 = createNewData(4,'B'); 
        void* data3 = createNewData(5,'C'); 
        void* data4 = createNewData(2,'A'); 
        void* data5 = createNewData(4,'B'); 
        void* data6 = createNewData(5,'C'); 
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);
        dpl_insert_at_index(list, data3, 3,false);
        dpl_insert_at_index(list, data4, 4,false);
        dpl_insert_at_index(list, data5, 5,false);
        dpl_insert_at_index(list, data6, 6,false);

        void* data11 = createNewData(2,'A'); 
        void* data12 = createNewData(4,'B'); 
        void* data13 = createNewData(5,'C'); 
        listTwo = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(listTwo, data11, 1,false);
        dpl_insert_at_index(listTwo, data12, 2,false);
        dpl_insert_at_index(listTwo, data13, 3,false);

        reference = dpl_get_last_reference(list);
        referenceTwo = dpl_get_previous_reference(list,reference);
        //ck_assert_msg(referenceTwo == NULL, "Failure: expected result to be NULL");
        element = dpl_get_element_at_reference(list,referenceTwo);
        char storedChar = *(((my_element_t*)element)->ptrToData);
        ck_assert_msg(storedChar == 'B', "Failure, expected 'B', but got %c",storedChar);
        dpl_free(&list,true);	
        dpl_free(&listTwo,true);	

    }
END_TEST */
/* START_TEST(test_get_reference_of_element)
    {
    	dplist_t* list; 
        dplist_node_t* reference; 
        my_element_t* element;
        int index;

        dplist_t* listTwo; 
        dplist_node_t* referenceTwo; 
        my_element_t* elementTwo;
        int indexTwo;

    	
        void* data1 = createNewData(2,'A'); 
        void* data2 = createNewData(4,'B'); 
        void* data3 = createNewData(5,'C'); 
        void* data4 = createNewData(2,'A'); 
        void* data5 = createNewData(4,'B'); 
        void* data6 = createNewData(5,'C'); 
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);
        dpl_insert_at_index(list, data3, 3,false);
        dpl_insert_at_index(list, data4, 4,false);
        dpl_insert_at_index(list, data5, 5,false);
        dpl_insert_at_index(list, data6, 6,false);

        void* data11 = createNewData(22,'A'); 
        void* data12 = createNewData(42,'B'); 
        void* data13 = createNewData(52,'C'); 
        listTwo = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(listTwo, data11, 0,false);
        dpl_insert_at_index(listTwo, data12, 1,false);
        dpl_insert_at_index(listTwo, data13, 2,false);


        reference = dpl_get_reference_of_element(list,data1);
        //referenceTwo = dpl_get_previous_reference(list,reference);
        //ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
         element = dpl_get_element_at_reference(list,reference);
        char storedChar = *(((my_element_t*)element)->ptrToData);
        ck_assert_msg(storedChar == 'A', "Failure, expected 'A', but got %c",storedChar);
        dpl_free(&list,true);	
        dpl_free(&listTwo,true);	

    }
END_TEST */
/* START_TEST(test_remove_at_reference)
    {
    	dplist_t* list; 
        dplist_node_t* reference; 
        my_element_t* element;
        int index;

        dplist_t* listTwo; 
        dplist_node_t* referenceTwo; 
        my_element_t* elementTwo;
        int indexTwo;

    	
        void* data1 = createNewData(2,'A'); 
        void* data2 = createNewData(4,'B'); 
        void* data3 = createNewData(5,'C'); 
        void* data4 = createNewData(2,'A'); 
        void* data5 = createNewData(4,'B'); 
        void* data6 = createNewData(5,'C'); 
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);
        dpl_insert_at_index(list, data3, 3,false);
        dpl_insert_at_index(list, data4, 4,false);
        dpl_insert_at_index(list, data5, 5,false);
        dpl_insert_at_index(list, data6, 6,false);

        void* data11 = createNewData(22,'A'); 
        void* data12 = createNewData(42,'B'); 
        void* data13 = createNewData(52,'C'); 
        listTwo = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(listTwo, data11, 0,false);
        dpl_insert_at_index(listTwo, data12, 1,false);
        dpl_insert_at_index(listTwo, data13, 2,false);

        
        reference = dpl_get_reference_of_element(listTwo,data11);

        printf("\n\n\n");
        printf("The size of the list is %d",dpl_size(listTwo));
        printf("\n\n\n");
        listTwo = dpl_remove_at_reference(listTwo,reference,true);
        printf("\n\n\n");
        printf("The size of the list is %d",dpl_size(listTwo));
        printf("\n\n\n");
        //referenceTwo = dpl_get_previous_reference(list,reference);
        //ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        // element = dpl_get_element_at_index(list,0);
        // char storedChar = *(((my_element_t*)element)->ptrToData);
        // ck_assert_msg(storedChar == 'B', "Failure, expected 'B', but got %c",storedChar);
        dpl_free(&list,true);	
        dpl_free(&listTwo,true);	

    }
END_TEST  */
/* START_TEST(test_remove_element)
    {
    	dplist_t* list; 
        dplist_node_t* reference; 
        my_element_t* element;
        int index;

        dplist_t* listTwo; 
        dplist_node_t* referenceTwo; 
        my_element_t* elementTwo;
        int indexTwo;

    	
        void* data1 = createNewData(2,'A'); 
        void* data2 = createNewData(4,'B'); 
        void* data3 = createNewData(5,'C'); 
        void* data4 = createNewData(2,'A'); 
        void* data5 = createNewData(4,'B'); 
        void* data6 = createNewData(5,'C'); 
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, data1, 1,false);
        dpl_insert_at_index(list, data2, 2,false);
        dpl_insert_at_index(list, data3, 3,false);
        dpl_insert_at_index(list, data4, 4,false);
        dpl_insert_at_index(list, data5, 5,false);
        dpl_insert_at_index(list, data6, 6,false);

        void* data11 = createNewData(22,'A'); 
        void* data12 = createNewData(42,'B'); 
        void* data13 = createNewData(52,'C'); 
        listTwo = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(listTwo, data11, 0,false);
        dpl_insert_at_index(listTwo, data12, 1,false);
        dpl_insert_at_index(listTwo, data13, 2,false);

        
        //reference = dpl_get_reference_of_element(listTwo,data11);

        printf("\n\n\n");
        printf("The size of the list is %d",dpl_size(listTwo));
        printf("\n\n\n");
        listTwo = dpl_remove_element(listTwo,data11,true);
        printf("\n\n\n");
        printf("The size of the list is %d",dpl_size(listTwo));
        printf("\n\n\n");
        //referenceTwo = dpl_get_previous_reference(list,reference);
        //ck_assert_msg(reference == NULL, "Failure: expected result to be NULL");
        // element = dpl_get_element_at_index(list,0);
        // char storedChar = *(((my_element_t*)element)->ptrToData);
        // ck_assert_msg(storedChar == 'B', "Failure, expected 'B', but got %c",storedChar);
        dpl_free(&list,true);	
        dpl_free(&listTwo,true);	

    }
END_TEST */
int main(void) {
    Suite *s1 = suite_create("LIST_EX3");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_checked_fixture(tc1_1, setup, teardown);
    //Tests here...
    //tcase_add_test(tc1_1, test_get_previous_reference);
    tcase_add_test(tc1_1, test_get_element_at_reference);
    //
    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return nf == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
