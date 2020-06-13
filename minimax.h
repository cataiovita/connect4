#ifndef MY_FUNC2_H
#define MY_FUNC2_H
#include <stdio.h>

// Structura de nod al arborelui max;
typedef struct lcrs_node_max {
	int value;
	struct lcrs_node_max *sibling;
	struct lcrs_node_max *child;
} lcrs_node_max;

// Structura de lista de care am nev. pt coada;
typedef struct list_node {
	struct lcrs_node_max *data;
	struct list_node *next;
} list_node;

// Structura de coada;
typedef struct dequeue_t {
	int size;
	struct list_node *front;
	struct list_node *back;
} dequeue_t;

dequeue_t *alloc_deq(void);
lcrs_node_max *front(dequeue_t *deq);
void push_front_list(list_node **head, lcrs_node_max *data);
void push_back_list(list_node **head, lcrs_node_max *data);
int push(dequeue_t *deq, lcrs_node_max *data);
void pop(dequeue_t *deq);
void push_front_list(list_node **head, lcrs_node_max *data);
void push_back_list(list_node **head, lcrs_node_max *data);
int char_to_int(char *gridch);
int check_leaf(char *check_node);
int get_number(FILE *input, char *gridch);
lcrs_node_max *init_max_tree(void);
lcrs_node_max *create_child_max(void);
void create_leaf_child(lcrs_node_max **root, dequeue_t *deq, int *value);
void create_twig_child(lcrs_node_max **root, dequeue_t *deq, int *number);
int max_child(lcrs_node_max **root);
int min_child(lcrs_node_max **root);
void fill_upward(lcrs_node_max **root, int count);
void print_max_tree(FILE *output, lcrs_node_max *root, int count);
void dissalloc_max_tree(lcrs_node_max *root);
void read_input_max(FILE *input, lcrs_node_max *root, dequeue_t *deq,
	char **argv);
void write_output_max(FILE *output, char **argv, lcrs_node_max *root);

#endif
