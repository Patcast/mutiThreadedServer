myshared: dplist.c main.c #checks if *.c files are oder than myshared, if yes executes
	@gcc -fPIC -c dplist.c -o mylist.o #makes object file for shared library.
	@gcc -shared -o libmylist.so mylist.o #makes shared library
	@gcc -o myshared  main.c -lmylist -L.
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
