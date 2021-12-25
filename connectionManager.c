#define _GNU_SOURCE        
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <poll.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include "errmacros.h"
#include "config.h"
#include "lib/tcpsock.h"
#include "lib/dplist.h"

#define MAX	80

#define MAX_CONN 30
#define INDEX_OF_SERVER 0

#define TRUE             1
#define FALSE            0

       
#ifndef PORT    
#error PORT not set
#endif

#ifndef TIMEOUT    
#error TIMEOUT not set
#endif

dplist_t* list;

void* element_copy(void * element);
void element_free(void** element);
int element_compare(void * x, void * y);

typedef struct pollfd pollfd_t;

int main(void) {
    struct pollfd fds[MAX_CONN];
    int  server_fd,client_fd, current_size,result,timeout, nfds=1;
    tcpsock_t * currentSocket;
    sensor_data_t data;
    char end_server = FALSE,compress_array,close_conn;

    list = dpl_create(element_copy,element_free,element_compare);

    // Start server socket 
    printf("Test server is started\n");
    if (tcp_passive_open(&(currentSocket), PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    dpl_insert_at_index(list,currentSocket,INDEX_OF_SERVER,false);

  
    //Initialize the pollfd structure          
    memset(fds, 0 , sizeof(fds));
    timeout = (TIMEOUT * 1000);

    //Set up the initial listening socket                        
    server_fd = currentSocket->sd;
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
  
    //Loop waiting for incoming connects or for incoming data on any of the connected sockets.                          
    do
    {

        printf("Waiting on poll()...\n");
        result = poll(fds, nfds, timeout);
        /* Check to see if the poll call failed.                   */
        if (result < 0)
        {
            printf("There was an error doing the polling\n\n");
            perror("poll() failed");
            break;
        }
        /* Check to see if the 3 minute time out expired.          */
        if (result == 0)
        {
            printf("  poll() timed out.End program.\n");
            end_server = TRUE; // The program will close. 
            break;
        }

        /* One or more descriptors are readable.  Need to          */
        /* determine which ones they are.                          */
        current_size = nfds; // nfds changes through out the loop, so it is important to store the initial value seperately. 
        for (int i = 0; i < current_size; i++)
        {
            // check if there is a revent and no errors. 
            if(fds[i].revents == 0) continue; // There are no revents for this fd. 

            if(fds[i].revents != POLLIN)
            {
                printf("Error! The revents is un expected. fd = %d\n", fds[i].revents);
                end_server = TRUE;
                break;

            }

            
            if (fds[i].fd == server_fd) // server_fd never changes 
            {               
                    printf("Listening socket is readable\n");
                    /// waits for new connection and saves it in currentSocket
                    result = tcp_wait_for_connection((tcpsock_t* )dpl_get_element_at_index(list,INDEX_OF_SERVER), &currentSocket);
                    // make a new connection
                    if (result != TCP_NO_ERROR){
                        perror("Adding socket failed");
                        end_server = TRUE; 
                        break;
                    } 
                    dpl_insert_at_index(list,currentSocket,nfds,false);
                    fds[nfds].fd = currentSocket->sd;
                    fds[nfds].events = POLLIN;
                    printf("Incoming client connection\n");
                    nfds++;
            }

            else
            {
                    printf("  Descriptor %d is readable\n", fds[i].fd);
                    close_conn = FALSE;
                    /* Receive all incoming data on this socket            */
                    /* before we loop back and call poll again.            */ 
                        do {
                                
                                currentSocket = dpl_get_element_at_index(list,i); // add check to see if data is found 
                                // read sensor ID
                                int bytes = sizeof(data.id);
                                result = tcp_receive(currentSocket, (void *) &data.id, &bytes);
                                // read temperature
                                bytes = sizeof(data.value);
                                result = tcp_receive(currentSocket, (void *) &data.value, &bytes);
                                // read timestamp
                                bytes = sizeof(data.ts);
                                result = tcp_receive(currentSocket, (void *) &data.ts, &bytes);
                                if ((result == TCP_NO_ERROR) && bytes) {
                                    printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value,(long int) data.ts);
                                    
                                    break;    
                                }
                                else if (result == TCP_CONNECTION_CLOSED){
                                    printf("\nPeer has closed connection\n");
                                    close_conn = TRUE;
                                    break;
                                } 
                                else {
                                    printf("\nError when reading from sensor.\n");
                                    perror("recv() failed");
                                    close_conn = TRUE;
                                    break;
                                }
                            } while (TRUE);

                    if (close_conn)
                    {
                        dpl_remove_element(list,currentSocket,true);// frees node and socket)
                        fds[i].fd = -1;
                        compress_array = TRUE;
                    }
            }
        }/// end of big loop of pollable descriptors 

        if (compress_array){
            compress_array = FALSE;
            for (int i = 0; i < nfds; i++)
            {
                if (fds[i].fd == -1)
                {
                    for(int j = i; j < nfds-1; j++)
                    {
                        fds[j].fd = fds[j+1].fd;
                    }
                    i--;
                    nfds--;
                }
            }
        }
    } while (end_server == FALSE); // End of serving running.
    printf("\nThe server waitng time has expired\n");
    dpl_free(&list,true);
    
    return 0;
}

void * element_copy(void * element) {
    return NULL;
}

void element_free(void** ptrElemenet) {
    tcpsock_t* element = (tcpsock_t*)*ptrElemenet;
    if (tcp_close(&element) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    *ptrElemenet = NULL;
}

int element_compare(void * x, void * y) {
    return ((((tcpsock_t*)x)->sd < ((tcpsock_t*)y)->sd) ? -1 : (((tcpsock_t*)x)->sd == ((tcpsock_t*)y)->sd) ? 0 : 1);
}
