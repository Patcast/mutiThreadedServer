#.DEFAULT_GOAL := valgrind


FLAGS = -lsqlite3 -std=c99 -Werror  -lm $(shell pkg-config --cflags --libs check)
CODE = source/sensor_db.c main.c connmgr.c lib/tcpsock.c lib/sbuffer.c -lmylist -L/lib/ 
DEFINES =  -DMAKE_NEW_TABLE=TRUE



test: 
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc $(CODE) $(DEFINES) -o  build/database -Wall -std=c11 -Werror -lm -lsqlite3 -pthread -I/usr/local/include -L/usr/local/lib -lcheck	
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@./build/database

valgrind: sensor_db.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc $(CODE) -g -o  build/database -Wall -std=c11 -Werror -lm -lsqlite3 -pthread -I/usr/local/include -L/usr/local/lib -lcheck	
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@export CK_FORK=no ;  valgrind -s ./build/database
fullValgrind: sensor_db.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc $(CODE) -g -o  build/database -Wall -std=c11 -Werror -lm -lsqlite3 -pthread -I/usr/local/include -L/usr/local/lib -lcheck	
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@export CK_FORK=no ; valgrind -s --leak-check=full ./build/database
gdb: sensor_db.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	mkdir -p build
	gcc $(CODE)-g -o build/database $(FLAGS)	
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	export CK_FORK=no ; gdb -q -tui ./build/database
final: sensor_db.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc $(CODE) -o  build/database -Wall -std=c11 -Werror -lm -lsqlite3 -pthread -I/usr/local/include -L/usr/local/lib -lcheck	
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@./build/database

zip:
	@zip filesAndData/lab6_ex2.zip sensor_db.c config.h
