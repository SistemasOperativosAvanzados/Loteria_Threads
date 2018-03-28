all:
	gcc -Wall -g -o main main.c finalLottery.c `pkg-config --cflags --libs gtk+-3.0` -w -lm
	./main

clean:
	rm -f main.o
	rm -f main