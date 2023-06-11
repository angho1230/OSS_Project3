all: main.c draw.c draw.h reversi.c reversi.h net.c net.h
	gcc -o reversi main.c draw.c reversi.c net.c -lncurses

clean:
	rm -rf reversi

debug: main.c draw.c draw.h reversi.c reversi.h net.c net.h
	gcc -g -o reversi main.c draw.c reversi.c net.c -lncurses