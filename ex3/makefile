#.DEFAULT_GOAL := valgrind


FLAGS = -lsqlite3 -std=c99 -Werror  -lm $(shell pkg-config --cflags --libs check)


# database:  sensor_db.c main.c
# 	@gcc -o database sensor_db.c main.c 
# 	@./database
# the files for ex2 will be ziped and are then ready to
# be submitted to labtools.groept.be
test: sensor_db.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc sensor_db.c main.c -o  build/database -Wall -std=c11 -Werror -lm -lsqlite3 -pthread -I/usr/local/include -L/usr/local/lib -lcheck	
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@./build/database

valgrind: sensor_db.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc sensor_db.c main.c -g -o  build/database -Wall -std=c11 -Werror -lm -lsqlite3 -pthread -I/usr/local/include -L/usr/local/lib -lcheck	
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@export CK_FORK=no ;  valgrind -s ./build/database
fullValgrind: sensor_db.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc sensor_db.c main.c -g -o  build/database -Wall -std=c11 -Werror -lm -lsqlite3 -pthread -I/usr/local/include -L/usr/local/lib -lcheck	
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@export CK_FORK=no ; valgrind -s --leak-check=full ./build/database
gdb: sensor_db.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	mkdir -p build
	gcc sensor_db.c main.c -g -o build/database $(FLAGS)	
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	export CK_FORK=no ; gdb -q -tui ./build/database
final: sensor_db.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc sensor_db.c main.c -o  build/database -Wall -std=c11 -Werror -lm -lsqlite3 -pthread -I/usr/local/include -L/usr/local/lib -lcheck	
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@./build/database

zip:
	@zip lab6_ex2.zip sensor_db.c config.h