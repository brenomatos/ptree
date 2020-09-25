all:
	gcc -Wall -c barreira.c -lpthread
	gcc -Wall -c ptree.c -lpthread
	gcc -Wall main.c ptree.o barreira.o -lpthread -o tree
clean:
	rm tree barreira.o ptree.o
run:
	./tree