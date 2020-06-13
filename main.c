#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gametree.h"
#include "minimax.h"
#include "alphabeta.h"
#include "paranoic.h"

int main(int argc, char **argv)
{
	FILE *input = NULL, *output = NULL;
	lcrs_node_t *root = NULL;
	lcrs_node_max *rootmax = NULL;
	lcrs_node_ab *root_ab = NULL;
	lcrs_node_par *rootpar = NULL;
	dequeue_t *deq = NULL;
	dequeue_t_ab *deq_ab = NULL;
	dequeue_t_par *deqpar = NULL;
	stack_t *stk = NULL;

	char *move = '\0';
	int n, m, players;

	if (strcmp(argv[1], "-c1") == 0)
	{
		root = init_tree(input, argv, &move, &n, &m);
		create_game_tree(&root, &move, 0, &n, &m);
		write_output(output, argv, root, &n, &m);
		diss_alloc_tree(root, &n);
		free(move);
	}

	if (strcmp(argv[1], "-c2") == 0)
	{
		deq = alloc_deq();
		rootmax = init_max_tree();
		read_input_max(input, rootmax, deq, argv);
		fill_upward(&rootmax, 0);
		write_output_max(output, argv, rootmax);
		dissalloc_max_tree(rootmax);
		free(deq);
	}

	if (strcmp(argv[1], "-c3") == 0)
	{
		deq_ab = alloc_deq_ab();
		root_ab = init_ab_tree();
		stk = alloc_stack();
		read_input_ab(input, root_ab, deq_ab, argv);
		get_pruned(&root_ab, stk);
		generate_ab(&root_ab, 0, 1.0/0.0);
		write_output_ab(output, argv, root_ab);
		dissalloc_ab_tree(root_ab);
		free(deq_ab);
		free(stk);
	}

	if (strcmp(argv[1], "-b") == 0)
	{
		deqpar = alloc_deq_par();
		rootpar = init_paran_tree();
		players = read_input_paranoic(input, rootpar, deqpar, argv);
		fill_paranoic(&rootpar, 0, players);
		write_output_par(output, argv, rootpar);
		dissalloc_paran_tree(rootpar);
		free(deqpar);
	}

	return 0;
}

