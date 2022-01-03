#.DEFAULT_GOAL := valgrind

.DEFAULT_GOAL := debug
FLAGS = -Wall -std=c11 -Werror -lm -lsqlite3 -pthread -I/usr/local/include -L/usr/local/lib -lcheck	
LIBS =  lib/tcpsock.c  -lmylist -L/lib/ 
SOURCE= sensor_db.c datamgr.c main.c connmgr.c sbuffer.c
DEFINES =  -DMAKE_NEW_TABLE=TRUE -DTIMEOUT=8 -DSET_MAX_TEMP=25 -DSET_MIN_TEMP=10 -DRUN_AVG_LENGTH=3
DEBUG_FLAGS = -DDEBUG_DTA_MGR -DDEBUG_CONN_MGR -DDEBUG_STORAGE_MGR #-DDEBUG_BUFFER
PORT = 4000

test: 
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc $(SOURCE) $(LIBS) $(DEFINES) -o  build/database $(FLAGS) 
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
	@valgrind -s ./build/database $(PORT)
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
	@zip filesAndData/lab6_ex2.zip sensor_db.c config.h
