# Makefile trab ICC

FLAGS=-Wall -Wextra -g -lm
Executavel=pi

all: $(Executavel)

$(Executavel): trab.o
	gcc -o $(Executavel) trab.o $(FLAGS)

trab.o: trab.c
	gcc -c trab.c $(FLAGS)

clean:
	rm -f *.o *.gch

purge:
	rm -f *.o *.gch $(Executavel)

