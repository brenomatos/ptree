all:
	gcc -Wall -c barreira.c
	gcc -Wall ptree.c barreira.o -o tree
clean:
	rm tree barreira.o
run:
	./tree