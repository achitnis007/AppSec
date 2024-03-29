default: prog

get-deps:
	# Assuming Debian or Ubuntu here
	sudo apt-get install -y build-essential check

dictionary.o: dictionary.c
	gcc -g -O0 -Wall -c dictionary.c dictionary.h

spell.o: spell.c
	gcc -g -O0 -Wall -c spell.c

test.o: test_main.c
	gcc -g -O0 -Wall -c test_main.c

main.o: main.c
	gcc -g -O0 -Wall -c main.c

test: dictionary.o spell.o test_main.o
	gcc -g -O0 -Wall -o test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit
	./test_main

prog: dictionary.o spell.o main.o
	gcc -g -O0 -Wall -o spell_check dictionary.o spell.o main.o

clean:
	rm dictionary.o spell.o main.o test_main.o

cleanall:clean
	rm spell_check
	
