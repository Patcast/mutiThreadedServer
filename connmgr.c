#include "connmgr.h"




void* connmgr_listen(void * thread_param_input){

    dplist_t* list;
    pollfd_t* fds=NULL;
    int  server_fd, current_size,resultLock,result,timeout = (TIMEOUT * 1000);
    sensor_data_t data;
    char end_server = FALSE;
    socket_sensor_t * newSocket;
    
    thread_parameters_t* param = thread_param_input;


    list = dpl_create(element_copy,element_free,element_compare);

    // Start server socket & Set up the initial listening socket 
    // printf("Test server is started\n");
    newSocket = malloc(sizeof(socket_sensor_t));// make space on heap for new socket 
    if (tcp_passive_open(&(newSocket->tcpPointer), param->portNumber) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    addNewFd(&fds,list,newSocket);
    server_fd = newSocket->tcpPointer->sd; 
    SERVER_OPENS_CONNECTION(param->portNumber,(*(param->ptrToFilePtr)));
    //Loop waiting for incoming connects or for incoming data on any of the connected sockets.                          
    do
    {
        result = validateSockets(list,&fds);
        printf("...\n");
        result = poll(fds, dpl_size(list), timeout);
        if (result < 0) {
            perror("There was an error doing the polling");
            break;}
        if (result == 0){   
            printf("poll() timed out.End program.\n");
            end_server = TRUE; // The program will close. 
            break;}

        // One or more descriptors are readable.  Need to determine which ones they are.
        current_size = dpl_size(list);
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
                    // printf("Listening socket is readable\n");
                    newSocket = malloc(sizeof(socket_sensor_t));// make space on heap for new socket 
                    result = tcp_wait_for_connection(((socket_sensor_t*)dpl_get_element_at_index(list,INDEX_OF_SERVER))->tcpPointer, &(newSocket->tcpPointer));
                    // make a new connection
                    if (result != TCP_NO_ERROR){
                        perror("Adding socket failed");
                        end_server = TRUE; 
                        break;
                    } 
                    newSocket->id = 0;
                    time(&(newSocket->ts));
                    addNewFd(&fds,list,newSocket);       
            }

            else
            {
                    /* Receive all incoming data on this socket            */
                        do {
                                
                                newSocket = dpl_get_element_at_index(list,i); // add check to see if data is found 
                                // read sensor data
                                int bytes = sizeof(data.id);
                                result = tcp_receive(newSocket->tcpPointer, (void *) &data.id, &bytes);
                                bytes = sizeof(data.value);
                                result = tcp_receive(newSocket->tcpPointer, (void *) &data.value, &bytes);
                                bytes = sizeof(data.ts);
                                result = tcp_receive(newSocket->tcpPointer, (void *) &data.ts, &bytes);
                                if ((result == TCP_NO_ERROR) && bytes) {
                                    time(&(newSocket->ts));
                                    if(newSocket->id==0){
                                        newSocket->id = data.id;
                                        NEW_SENSOR((newSocket->id),(*(param->ptrToFilePtr)));
                                    }
                                    resultLock = pthread_mutex_lock( param->data_mutex ); /// critical secction  
                                    SYNCRONIZATION_ERROR(resultLock);
                                    sbuffer_insert(param->bufferHead,&data);
                                    #ifdef DEBUG_CONN_MGR  
                                            printf("B:\t%d\t%f\t%ld\n",data.id,data.value,data.ts);
                                    #endif
                                    resultLock = pthread_cond_broadcast(param->myConVar);        //// communicate that there is data available. 
                                    SYNCRONIZATION_ERROR(resultLock);
                                    resultLock = pthread_mutex_unlock(param->data_mutex );
                                    SYNCRONIZATION_ERROR(resultLock);
                                    break;    
                                }
                                else{
                                    SENSOR_DISCONECTED(newSocket->id,(*(param->ptrToFilePtr)));
                                    removeFd(&fds,list,i);
                                    if (result != TCP_CONNECTION_CLOSED)perror("\nError when reading from sensor.\n");
                                    break;
                                }
                            } while (TRUE);
            }
        }/// end of big loop of pollable descriptors 
    } while (end_server == FALSE); // End of serving running.
    connmgr_free(list,&fds); 
    pthread_exit(NULL);

}


void connmgr_free(dplist_t* mylist, pollfd_t** ptrToFds){
    dpl_free(&mylist,true);
    free(*ptrToFds);
    printf("\nThe server waitng time has expired.\nThe server has shut down sucessfully.\n");
}

int addNewFd(pollfd_t** ptrToFds,dplist_t * list,socket_sensor_t* socket){
    int size = dpl_size(list);
    dpl_insert_at_index(list,socket,size,false);

    if(size <1) *ptrToFds = calloc(dpl_size(list),sizeof(pollfd_t));
    else        *ptrToFds = reallocarray(*ptrToFds,sizeof(pollfd_t),dpl_size(list));
    
    if (*ptrToFds == NULL ) {
        printf("Memory not allocated.\n");
        return 1;
    }
    //else    printf("New Fd registered.\n");

    pollfd_t* fds = *ptrToFds;
    fds[size].fd=socket->tcpPointer->sd;
    fds[size].events = POLLIN;

    return 0;
}

int removeFd(pollfd_t** ptrToFds,dplist_t * list, int index){

    dpl_remove_at_index(list, index,true);
    free(*ptrToFds);
    *ptrToFds = calloc(dpl_size(list),sizeof(pollfd_t));
     if (*ptrToFds == NULL ) {
        printf("Memory not allocated.\n");
        return 1;
    }
    pollfd_t* fds = *ptrToFds;
    for (int i = 0; i<dpl_size(list);i++){
        fds[i].fd= ((socket_sensor_t*)dpl_get_element_at_index(list,i))->tcpPointer->sd;
        fds[i].events = POLLIN;
    }
    //printf("size of list %d\n",dpl_size(list));
    return 0;
} 


void * element_copy(void * element) {
    return NULL;
}

void element_free(void** ptrElemenet){
    socket_sensor_t* sensor = (socket_sensor_t*)*ptrElemenet;
    tcpsock_t* element = sensor->tcpPointer;
    if (tcp_close(&element) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    free(sensor);
    *ptrElemenet = NULL;
}

int element_compare(void * x_data, void * y_data) {
    tcpsock_t* x =(tcpsock_t*) (((socket_sensor_t*)x_data)->tcpPointer);
    tcpsock_t* y =(tcpsock_t*) (((socket_sensor_t*)y_data)->tcpPointer);
    return ((((tcpsock_t*)x)->sd < ((tcpsock_t*)y)->sd) ? -1 : (((tcpsock_t*)x)->sd == ((tcpsock_t*)y)->sd) ? 0 : 1);
}

int validateSockets(dplist_t* list,pollfd_t** ptrToFds){
     time_t  t_now;
     double r1;
     socket_sensor_t* socket;

    time(&t_now);

    for (int i = 1; i<dpl_size(list);i++){
        socket = ((socket_sensor_t*) dpl_get_element_at_index(list,i));
        r1= difftime(t_now,socket->ts);
        if (r1>TIMEOUT){
            printf("\nConnection with fd %d has expired.\nNo data sent over the past %f seconds.\n",socket->tcpPointer->sd,r1);    
            removeFd(ptrToFds,list,i);
        }
    }
    return 0;   
}




