#simple example of a Makefile


#variables for the compilation rules
CC = gcc #means that i want to compile with gcc instead of c
CFLAGS = -wall -g -std=c99 #declarating all the flags that will be used
#LDFLAGS links all the pieces together
# when compiling a program to connect a library  you can use -lm for example to compile the math library



simpleTest: simpleTest.c stringTools.c #requirements
	$(CC) $^ -o $@ $(LDFLAGS)
		#gcc simpleTest.c stringTools.c -o simpleTest #rule

#%.o: %.c
		#$(CC) $< -o $@ $(CFLAGS)

clean: #rule clean
	rm -rf *.o simpleTest #eliminate the files i dont want


.PHONY: clean #indicates the other rules that are not meant to create a files
