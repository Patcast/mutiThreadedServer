ex1Debug:
	@mkdir -p build
	@gcc -Wall -Werror -g -o ./build/ex1 ./ex1/*.c
	@gdb ./build/ex1 -q -tui

ex1Run:
	@mkdir -p build
	@gcc -Wall -Werror -o ./build/ex1 ./ex1/*.c
	@./build/ex1
ex1NoWarn:
	@mkdir -p build
	@gcc -g -o ./build/ex1 ./ex1/*.c
	@gdb ./build/ex1 -q -tui


zip: ex3/main.c
	cd ex3 && zip ../lab3_ex3.zip main.c
