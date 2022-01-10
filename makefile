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
	@zip filesAndData/final.zip sensor_db.c datamgr.c main.c connmgr.c sbuffer.c lib/tcpsock.c lib/dplist.c sensor_db.h datamgr.h connmgr.h sbuffer.h lib/tcpsock.h lib/dplist.h logGenerator.h errmacros.h config.h makefile
