#ifndef MY_FUNC1_H
#define MY_FUNC1_H
#include <stdio.h>

// Structura unui nod
typedef struct lcrs_node {
	char **table;
	struct lcrs_node *sibling;
	struct lcrs_node *child;
} lcrs_node_t;

char **read_input(FILE *input_table, char **argv, char **move,
	int *n, int *m);
lcrs_node_t *init_tree(FILE *input_table, char **argv,
	char **move, int *n, int *m);
void print_table(FILE *output, lcrs_node_t *root, int count,
	int *n, int *m);
void insert_char_matrix(char **matrix, char move, int *n,
	int *m, int *c);
int count_pos_moves(char **matrix, int *n, int *m);
void alloc_one_table(char ***table, int *n, int *m);
void copy_prev_table(char **prev, char **current, int *n, int *m);
lcrs_node_t *alloc_child(lcrs_node_t **root, int *n, int *m);
int check_draw_game(char **matrix, int *n, int *m);
int check_win_game(char **mat, int *n, int *m, char move, int c_move);
void create_child(lcrs_node_t **root, char **move, int c_move,
	int *n, int *m);
void create_game_tree(lcrs_node_t **root, char **move, int c_move,
	int *n, int *m);
void print_game_tree(FILE *output, lcrs_node_t *root, int count,
	int *n, int *m);
void write_output(FILE *output, char **argv, lcrs_node_t *root,
	int *n, int *m);
void diss_alloc_child(lcrs_node_t *root, int *n);
void diss_alloc_tree(lcrs_node_t *root, int *n);

#endif
