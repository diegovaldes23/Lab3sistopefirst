all: lab3

funciones.o: funciones.c funciones.h
	g++ -c funciones.c

lab3: funciones.o lab3.c
	g++ -o lab3 funciones.o lab3.c
clean:
	rm -f *.o lab3
run_all:
	./lab3
	