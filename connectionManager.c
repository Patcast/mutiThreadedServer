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

#define MAX	80

#define MAX_CONN 30

#define TRUE             1
#define FALSE            0

#define PORT            5000           
// #ifndef PORT    
// #error PORT not set
// #endif

//    struct pollfd {
//                int   fd;         /* file descriptor */ if negative, result is ignored 
//                short events;     /* requested events */
//                short revents;    /* returned events */
//            };
int getSocket(tcpsock_t* sockets,tcpsock_t** clientSocket, int fd);

int main(void) {
    int nConnections = 3;
    //int sockets_fds [nConnections][2];
    struct pollfd fds[MAX_CONN];
    char recv_buf[MAX];
    int i,bytes, alive,server_fd,client_fd, current_size,result,timeout,rc, nfds=1;
    tcpsock_t* sockets[MAX_CONN];
    tcpsock_t * currentClient;
    sensor_data_t data;

    char end_server,compress_array,close_conn;


    // Start server socket 
    printf("Test server is started\n");
    if (tcp_passive_open(&(sockets[0]), PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);

    /*************************************************************/
    /* Initialize the pollfd structure                           */
    /*************************************************************/
    memset(fds, 0 , sizeof(fds));
    /*************************************************************/
    /* Set up the initial listening socket                        */
    /*************************************************************/
    tcp_get_sd((sockets[0]),&server_fd); 
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    /*************************************************************/
    /* Initialize the timeout to 3 minutes. If no                */
    /* activity after 3 minutes this program will end.           */
    /* timeout value is based on milliseconds.                   */
    /*************************************************************/
    timeout = (3 * 60 * 1000);

    /*************************************************************/
    /* Loop waiting for incoming connects or for incoming data   */
    /* on any of the connected sockets.                          */
    /*************************************************************/
    do
    {
        printf("Waiting on poll()...\n");
        rc = poll(fds, nfds, timeout);

        /***********************************************************/
        /* Check to see if the poll call failed.                   */
        /***********************************************************/
        if (rc < 0)
        {
        perror("  poll() failed");
        break;
        }

        /***********************************************************/
        /* Check to see if the 3 minute time out expired.          */
        /***********************************************************/
        if (rc == 0)
        {
        printf("  poll() timed out.  End program.\n");
        break;
        }

        /***********************************************************/
        /* One or more descriptors are readable.  Need to          */
        /* determine which ones they are.                          */
        /***********************************************************/
        current_size = nfds;
        for (i = 0; i < current_size; i++)
        {
            if(fds[i].revents == 0)
            continue;

            /*********************************************************/
            /* If revents is not POLLIN, it's an unexpected result,  */
            /* log and end the server.                               */
            /*********************************************************/
            if(fds[i].revents != POLLIN)
            {
                printf("  Error! revents = %d\n", fds[i].revents);
                end_server = TRUE;
                break;

            }
            tcp_get_sd((sockets[0]),&server_fd); 
            if (fds[i].fd == server_fd)
            {
                /*******************************************************/
                /* Listening descriptor is readable.                   */
                /*******************************************************/
                printf("  Listening socket is readable\n");

                /*******************************************************/
                /* Accept all incoming connections that are            */
                /* queued up on the listening socket before we         */
                /* loop back and call poll again.                      */
                /*******************************************************/
                do
                {
                    // make a new connection
                    if (tcp_wait_for_connection((sockets[0]), &(sockets[nfds])) != TCP_NO_ERROR){
                        //exit(EXIT_FAILURE); instead close it better
                        perror("Adding socket failed");
                        end_server = TRUE; 
                    } 

                    tcp_get_sd(sockets[nfds],&client_fd);
                    fds[nfds].fd = client_fd;
                    fds[nfds].events = POLLIN;
                    printf("Incoming client connection\n");
                    nfds++;
                }while ( client_fd != -1);
            }
            else
            {
                    printf("  Descriptor %d is readable\n", fds[i].fd);
                    close_conn = FALSE;
                    /*******************************************************/
                    /* Receive all incoming data on this socket            */
                    /* before we loop back and call poll again.            */
                    /*******************************************************/

                    do
                    {   

                        if(getSocket(sockets,&currentClient,fds[i].fd)!=0){
                            printf("the socket was not found");
                            break;
                        }
                        // read sensor ID
                        bytes = sizeof(data.id);
                        result = tcp_receive(currentClient, (void *) &data.id, &bytes);
                        // read temperature
                        bytes = sizeof(data.value);
                        result = tcp_receive(currentClient, (void *) &data.value, &bytes);
                        // read timestamp
                        bytes = sizeof(data.ts);
                        result = tcp_receive(currentClient, (void *) &data.ts, &bytes);

                        if ((result == TCP_NO_ERROR) && bytes) {
                            printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value,
                            (long int) data.ts);
                            break;// only breaks if there is a problem.
                        }
                        else{
                            if (result == TCP_CONNECTION_CLOSED) printf("Peer has closed connection\n");
                            else perror("  recv() failed");
                            close_conn = TRUE;
                            break;

                        }
                        //may need to set close_conn = TRUE;
                    } while (TRUE); 

                    if (close_conn)
                    {
                        /// Do an array of clients and a method to find a client base on there fd
                        tcp_close(&currentClient);    
                        //close(fds[i].fd); this is done by previous method. 
                        fds[i].fd = -1;
                        compress_array = TRUE;
                    }
            }  
        }/// end of big for loop of pollable descriptors 
    } while (end_server == FALSE); /* End of serving running.    */
    
    for (i = 0; i < nfds; i++)
    {
        if(fds[i].fd >= 0){
            getSocket(sockets,currentClient,fds[i].fd);
            tcp_close(&currentClient); 
        }

        
    }

    return 0;
}

int getSocket(tcpsock_t* sockets,tcpsock_t** clientSocket, int fd){



}

