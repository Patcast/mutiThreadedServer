# Create your own targets that compile the application


# the files for ex2 will be ziped and are then ready to
# be submitted to labtools.groept.be
FLAGS = -pthread -std=c11  -DTIMEOUT=10 -Werror -lm $(shell pkg-config --cflags --libs check)

test: main.c 
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc  main.c  -lmylist -L/lib/ -o build/test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@./build/test
superTest: connmgr.c main.c 
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc main.c connmgr.c -o conn_mgr_main -Wall -std=c11 -Werror -lm -L. -Wl,-rpath=. -ltcpsock -ldplist -DTIMEOUT=5
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	./build/test
debug: connectionManager.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc connmgr.c main.c  lib/tcpsock.c -lmylist -L/lib/ -g -o build/test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@gdb -q -tui ./build/test
valgrind: connectionManager.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc connmgr.c main.c lib/tcpsock.c -lmylist -L/lib/ -g -o build/test $(FLAGS)
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@export CK_FORK=no ;  valgrind -s ./build/test
zip:
	zip lab7_ex2.zip connmgr.c  errmacros.h connmgr.h config.h lib/dplist.c lib/dplist.h lib/tcpsock.c lib/tcpsock.h
