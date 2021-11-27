.DEFAULT_GOAL := valgrind
HEADERS=dplist.h
SOURCE=dplist.c
EXE=build/dplist

FLAGS = -std=c11 -Werror -lm $(shell pkg-config --cflags --libs check)

test: dplist.c dplist_test.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	mkdir -p build
	gcc dplist.c dplist_test.c -o build/dplist_test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	./build/dplist_test


valgrind: dplist.c dplist_test.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc dplist.c dplist_test.c -g -o build/dplist_test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@export CK_FORK=no ; valgrind -s ./build/dplist_test

fullValgrind: dplist.c dplist_test.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	@mkdir -p build
	@gcc -g dplist.c dplist_test.c -o build/dplist_test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	@export CK_FORK=no ; valgrind -s --leak-check=full --show-leak-kinds=all ./build/dplist_test

gdb: dplist.c dplist_test.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	mkdir -p build
	gcc dplist.c dplist_test.c -g -o build/dplist_test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	export CK_FORK=no ; gdb -q -tui ./build/dplist_test

ddd: dplist.c dplist_test.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	mkdir -p build
	gcc dplist.c dplist_test.c -g -o build/dplist_test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	export CK_FORK=no ; ddd ./build/dplist_test
zip: dplist.c
	zip lab4_ex4.zip dplist.c

.PHONY : clean check

check : $(SOURCE) $(HEADERS)
	@echo "Running cppcheck :"
	cppcheck --enable=all --suppress=missingIncludeSystem --error-exitcode=1 $(SOURCE) $(HEADERS)

clean:
	rm -f *.o $(EXE) $(EXE)_test *~

