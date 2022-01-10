#ifndef __errmacros_h__
#define __errmacros_h__

#include <errno.h>


#define ERR_HANDLER(condition, err_code)                         \
    do {                                                                \
            if ((condition)) DEBUG_PRINTF(#condition " failed\n");      \
            assert(!(condition));                                       \
        } while(0)

#define SYSCALL_ERROR(err) 									\
		do {												\
			if ( (err) == EOF )								\
			{												\
				perror("Error executing syscall");			\
				exit( EXIT_FAILURE );						\
			}												\
		} while(0)
#define MEMORY_ERROR(ptr) 									\
		do {												\
			if ( (ptr) == NULL )							\
			{												\
 				fprintf(stderr,"\nIn %s - function %s at line %d: There was a syncronization error", __FILE__, __func__, __LINE__);\
				fflush(stderr); \
				pthread_exit(NULL);						\
			}												\
		} while(0)
		
#define CHECK_MKFIFO(err) 									\
		do {												\
			if ( (err) == EOF )								\
			{												\
				if ( errno != EEXIST )						\
				{											\
					perror("Error executing mkfifo");		\
					exit( EXIT_FAILURE );					\
				}											\
			}												\
		} while(0)
		
#define FILE_OPEN_ERROR(fp) 								\
		do {												\
			if ( (fp) == NULL )								\
			{												\
				perror("File open failed");					\
				exit( EXIT_FAILURE );						\
			}												\
		} while(0)

#define FILE_CLOSE_ERROR(err) 								\
		do {												\
			if ( (err) == EOF )								\
			{												\
				perror("File close failed");				\
				exit( EXIT_FAILURE );						\
			}												\
		} while(0)

#define ASPRINTF_ERROR(err) 								\
		do {												\
			if ( (err) == EOF )								\
			{												\
				perror("asprintf failed");					\
				pthread_exit(NULL);						\
			}												\
		} while(0)

#define FFLUSH_ERROR(err) 								\
		do {												\
			if ( (err) == EOF )								\
			{												\
				perror("fflush failed");					\
				pthread_exit(NULL);						\
			}												\
		} while(0)
#define THREAD_ERROR(err) 								\
		do {												\
			if ( (err) != 0 )								\
			{												\
 				fprintf(stderr,"\nIn %s - function %s at line %d: There was a thread error.\n", __FILE__, __func__, __LINE__);\
				fflush(stderr); \
				exit( EXIT_FAILURE );						\
			}												\
		} while(0)
#define SYNCRONIZATION_ERROR(err,...) 								\
		do {												\
			if ( (err) != 0 )								\
			{												\
 				fprintf(stderr,"\nIn %s - function %s at line %d: There was a syncronization error.\n\n", __FILE__, __func__, __LINE__);\
				fflush(stderr); \
				pthread_exit(NULL);						\
			}												\
		} while(0)
#define EMPTY_LIST() 								\
		do {												\
 				fprintf(stderr,"\nIn %s - function %s at line %d:Cannot retive data. Because, the list is empty.\n\n", __FILE__, __func__, __LINE__);\
				fflush(stderr); 			\
				pthread_exit(NULL);\
		} while(0)

#define LIST_IS_NULL() 								\
		do {												\
				fprintf(stderr,"\nIn %s - function %s at line %d: Error from attempting to retrive data from NULL list.\n\n", __FILE__, __func__, __LINE__);\
				fflush(stderr); 			\
				pthread_exit(NULL);						\
		} while(0)
#define POLL_ERROR(result) 								\
		do {												\
			if ( result <=0)								\
			{												\
				if(result == 0)							\
				{									\
					printf("\nThe server waitng time has expired.\nThe server has shut down sucessfully.\n");\
					end_server = TRUE;  \
				}				\
				else{			\
					printf("There was an error during the polling");\
					pthread_exit(NULL);						\
				}								\
				break;		\
			}							\
		} while(0)


#endif
