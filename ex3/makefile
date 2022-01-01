.DEFAULT_GOAL := valgrind
HEADERS=dplist.h
SOURCE=dplist.c
EXE=build/dplist

FLAGS = -std=c11 -Werror -DSET_MAX_TEMP=25 -DSET_MIN_TEMP=10 -lm $(shell pkg-config --cflags --libs check)

test: datamgr.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc datamgr.c main.c -lmylist -L/lib/ -o build/manager_test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	./build/manager_test
debug: datamgr.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc -DDEBUG datamgr.c main.c -lmylist -L/lib/ -o  build/manager_test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	./build/manager_test

valgrind: datamgr.c main.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc datamgr.c main.c -lmylist -L/lib/ -g -o build/manager_test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@export CK_FORK=no ; valgrind -s ./build/manager_test
teacher:
	@gcc datamgr.c  main.c  -o build/datamgr_test -Wall -std=c11 -Werror -lm -DSET_MIN_TEMP=14.01 -DSET_MAX_TEMP=27.96 -lmylist -L/lib/  -pthread -I/usr/local/include -L/usr/local/lib -lcheck
	@./build/datamgr_test
manager:  datamgr.c main.c #checks if *.c files are oder than myshared, if yes executes
	@gcc -o manager datamgr.c main.c -lmylist -L/lib/ -DSET_MAX_TEMP=20 -DSET_MIN_TEMP=5
zip:
	zip lab5_ex3.zip datamgr.c datamgr.h config.h lib/dplist.c lib/dplist.h
