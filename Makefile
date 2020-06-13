build:
	gcc -Wall -g main.c gametree.c gametree.h minimax.c minimax.h alphabeta.c alphabeta.h paranoic.c paranoic.h -o minimax
clean:
	rm minimax