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
				exit( EXIT_FAILURE );						\
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
				exit( EXIT_FAILURE );						\
			}												\
		} while(0)

#define FFLUSH_ERROR(err) 								\
		do {												\
			if ( (err) == EOF )								\
			{												\
				perror("fflush failed");					\
				exit( EXIT_FAILURE );						\
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
				exit( EXIT_FAILURE );						\
			}												\
		} while(0)

#endif
