
CC = gcc
CFLAGS = -O3 -Wall
# For debugging, replace "-O3" with "-g -DDEBUG"

all: demolino
debug: all
	./demolino < test.demo
	./demolino < test.demo > runtime/demo.c
	gcc -o runtime/demo runtime/demort.c
	./runtime/demo
clean:
	rm -f *.o
	rm -f *.yy.c
	rm -f *.tab.*
	rm -f demolino
	rm -f runtime/demo
	rm -f runtime/demo.c
rebuild: clean all

demolino: demo.tab.o lex.yy.o demolino.o link.o debug.o compiler.o object.o
	gcc -o $@ $(CFLAGS) $^ -ll
lex.yy.c: demo.l
	flex $^
demo.tab.c: demo.y
	bison -d $^
