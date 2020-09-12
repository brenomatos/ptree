all:
	gcc -Wall -c barreira.c -lpthread
	gcc -Wall -c ptree.c
	gcc -Wall main.c ptree.o barreira.o -o tree
clean:
	rm tree barreira.o ptree.o
run:
	./tree