compile:
	gcc -Wall -std=c99 -o main main.c vector.c network.c
run: compile
	./main
valgrind: compile
	valgrind -s ./main