#define _GNU_SOURCE        
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <poll.h>
#include <sys/types.h>
#include "errmacros.h"
#include "config.h"
#include "lib/tcpsock.h"
#include "lib/dplist.h"

dplist_t* list;

void* element_copy(void * element);
void element_free(void** element);
int element_compare(void * x, void * y);

void* makeFakeTcpSocket(int sdParam){
    tcpsock_t* socket;
    socket = malloc(sizeof(tcpsock_t));
    socket->sd = sdParam;
    return  (void *)socket;
}

void* findElemendWithFd(int fd);

int main (void){
    tcpsock_t* socket;

    printf("hello World");
   
    socket= makeFakeTcpSocket(1);
    dpl_insert_at_index(list,socket,100,false);
    socket= makeFakeTcpSocket(2);
    dpl_insert_at_index(list,socket,100,false);
    socket= makeFakeTcpSocket(3);
    dpl_insert_at_index(list,socket,100,false);
     socket= makeFakeTcpSocket(4);
    dpl_insert_at_index(list,socket,100,false);
    socket = findElemendWithFd(3);
    printf("I found an element %d",socket->sd);
    dpl_free(&list,true);
    return 0;
}

void* findElemendWithFd(int fd){
     tcpsock_t* socket = NULL;
     tcpsock_t* test;

    for(int i = 0; i< dpl_size(list); i++){
        test=(tcpsock_t*) dpl_get_element_at_index(list,i);
        if(test->sd == fd){
           socket=  test;
           break;
        } 
    }
    return socket;
}


void * element_copy(void * element) {
    return NULL;
}

void element_free(void** ptrElemenet) {
    tcpsock_t* element = (tcpsock_t*)*ptrElemenet;
    free(*ptrElemenet);
    *ptrElemenet = NULL;
}

int element_compare(void * x, void * y) {
    return ((((tcpsock_t*)x)->sd < ((tcpsock_t*)y)->sd) ? -1 : (((tcpsock_t*)x)->sd == ((tcpsock_t*)y)->sd) ? 0 : 1);
}