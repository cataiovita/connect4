#ifndef MY_FUNC3_H
#define MY_FUNC3_H
#include <stdio.h>

// Structura de nod a arborelui
typedef struct lcrs_node_par {
	char *value;
	struct lcrs_node_par *sibling;
	struct lcrs_node_par *child;
} lcrs_node_par;

// Structura lista;
typedef struct list_node_par {
	struct lcrs_node_par *data;
	struct list_node_par *next;
} list_node_par;

// Structura coada;
typedef struct dequeue_t_par {
	int size;
	struct list_node_par *front;
	struct list_node_par *back;
} dequeue_t_par;

dequeue_t_par *alloc_deq_par(void);
lcrs_node_par *front_par(dequeue_t_par *deq);
void push_front_p_list(list_node_par **head, lcrs_node_par *data);
void push_back_p_list(list_node_par **head, lcrs_node_par *data);
int push_par(dequeue_t_par *deq, lcrs_node_par *data);
void pop_par(dequeue_t_par *deq);
void push_front_p_list(list_node_par **head, lcrs_node_par *data);
void push_back_p_list(list_node_par **head, lcrs_node_par *data);
int char_to_int_p(char *gridch);
int get_number_p(FILE *input, char *gridch);
int first_num(char *number);
lcrs_node_par *init_paran_tree(void);
lcrs_node_par *create_child_par(void);
void create_leaf_paran(lcrs_node_par **root, dequeue_t_par *deq,
	char *value, int *plys);
void create_twig_paran(lcrs_node_par **root, dequeue_t_par *deq,
	int *number);
char *par_max_child(lcrs_node_par **root);
char *par_min_child(lcrs_node_par **root);
void fill_paranoic(lcrs_node_par **root, int count, int players);
void print_paran_tree(FILE *output, lcrs_node_par *root, int count);
void dissalloc_paran_tree(lcrs_node_par *root);
int read_input_paranoic(FILE *input, lcrs_node_par *root,
	dequeue_t_par *deq, char **argv);
void write_output_par(FILE *output, char **argv, lcrs_node_par *root);

#endif
