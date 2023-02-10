#.DEFAULT_GOAL := valgrind

#.DEFAULT_GOAL := debug
FLAGS = -Wall -std=c11 -Werror -lm -lsqlite3 -pthread -I/usr/local/include -L/usr/local/lib -lcheck	
LIBS =  lib/tcpsock.c  -lmylist -L/lib/ 
SOURCE= sensor_db.c datamgr.c main.c connmgr.c sbuffer.c
DEFINES =  -DMAKE_NEW_TABLE=TRUE -DTIMEOUT=8 -DSET_MAX_TEMP=25 -DSET_MIN_TEMP=10 -DRUN_AVG_LENGTH=3
DEBUG_FLAGS =  -DDEBUG_BUFFER -DDEBUG_DTA_MGR -DDEBUG_CONN_MGR -DDEBUG_STORAGE_MGR 
PORT = 4000

test: 
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc $(SOURCE) $(LIBS) $(DEFINES) $(FLAGS)  -o  build/database 
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@./build/database $(PORT)
debug: 
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc $(SOURCE) $(LIBS) $(DEFINES) $(DEBUG_FLAGS)  -o build/database $(FLAGS) 
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@./build/database $(PORT)

valgrind: sensor_db.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc $(SOURCE) $(LIBS) $(DEFINES) -g -o  build/database $(FLAGS)
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@valgrind -s --leak-check=full ./build/database $(PORT) 
gdb: 
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc $(SOURCE) $(LIBS) $(DEFINES) -g -o build/database  $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@gdb -q -tui ./build/database $(PORT)

zip:
	@zip filesAndData/final.zip sensor_db.c datamgr.c main.c connmgr.c sbuffer.c lib/tcpsock.c lib/dplist.c sensor_db.h datamgr.h connmgr.h sbuffer.h lib/tcpsock.h lib/dplist.h logGenerator.h errmacros.h config.h makefile code_review_checklist.ods


####### CODE FOR SHARE LIBRARY ####

myshared: lib/dplist.c  #checks if *.c files are oder than myshared, if yes executes
	@gcc -fPIC -c lib/dplist.c -o mylist.o #makes object file for shared library.
	@gcc -shared mylist.o -o libmylist.so  #makes shared library
	@## gcc -o myshared  main.c -lmylist -L.
loaderCheck:
	@ldd myshared #checks for all the dependencies on the shared libraries.
	@#first this will cause an error. run time loader cannot find shared library
	@#solve by copy library to default lib folder.
copy: #we copy the library in the standar path.
	@sudo cp libmylist.so /usr/lib
nmCheck:
	nm myshared #This checks which symbols exist in the executable.
	#In this folder you will notice that all the implementations of list are U (undefined),
	#instead of T(Text), what means that they are not in the executable.
objdumpCheck: #this is another way to check if functions are implemented.
	objdump -t myshared
