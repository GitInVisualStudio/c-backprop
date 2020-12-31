compile:
	gcc -Wall -std=c99 -o main main.c vector.c network.c -lm
run: compile
	./main
valgrind: compile
	valgrind -s --leak-check=full ./main