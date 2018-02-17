all:
	gcc -Wall -g -o main main.c `pkg-config --cflags --libs gtk+-3.0` -w 
	./main

clean:
	rm -f main.o
	rm -f main