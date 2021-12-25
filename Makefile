# Create your own targets that compile the application


# the files for ex2 will be ziped and are then ready to
# be submitted to labtools.groept.be
FLAGS = -std=c11 -DPORT=5000 -DTIMEOUT=20 -Werror -lm $(shell pkg-config --cflags --libs check)

test: connectionManager.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc connectionManager.c lib/tcpsock.c -lmylist -L/lib/ -o build/test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	./build/test
debug: connectionManager.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc connectionManager.c lib/tcpsock.c -lmylist -L/lib/ -g -o build/test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@gdb -q -tui ./build/test	


zip:
	zip lab7_ex2.zip connmgr.c connmgr.h config.h lib/dplist.c lib/dplist.h lib/tcpsock.c lib/tcpsock.h
