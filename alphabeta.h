#ifndef MY_FUNC4_H
#define MY_FUNC4_H
#include <stdio.h>

typedef struct lcrs_node_ab {
	float value;
	float alpha;
	float beta;
	struct lcrs_node_ab *sibling;
	struct lcrs_node_ab *child;
} lcrs_node_ab;

typedef struct list_node_ab {
	struct lcrs_node_ab *data;
	struct list_node_ab *next;
	struct list_node_ab *prev;
} list_node_ab;

typedef struct dequeue_t_ab {
	int size;
	struct list_node_ab *front;
	struct list_node_ab *back;
} dequeue_t_ab;

typedef struct stack_t {
	int size;
	struct list_node_ab *front;
} stack_t;

dequeue_t_ab *alloc_deq_ab(void);
lcrs_node_ab *front_b(dequeue_t_ab *deq);
void push_f_list(list_node_ab **head, lcrs_node_ab *data);
void push_b_list(list_node_ab **head, lcrs_node_ab *data);
int push_b(dequeue_t_ab *deq, lcrs_node_ab *data);
void pop_b(dequeue_t_ab *deq);
void push_f_list(list_node_ab **head, lcrs_node_ab *data);
void push_b_list(list_node_ab **head, lcrs_node_ab *data);
stack_t *alloc_stack(void);
int char_to_int_ab(char *gridch);
int get_number_ab(FILE *input, char *gridch);
lcrs_node_ab *stack_top(stack_t *stk);
int stack_push(stack_t *stk, lcrs_node_ab *data);
lcrs_node_ab *stack_pop(stack_t *stk);
lcrs_node_ab *init_ab_tree(void);
lcrs_node_ab *create_child_ab(void);
void generate_ab_leaf(lcrs_node_ab **root, dequeue_t_ab *deq, float *value);
void generate_ab_child(lcrs_node_ab **root, dequeue_t_ab *deq, float *number);
float max_ab_child(lcrs_node_ab **root, float *bvalue);
float min_ab_child(lcrs_node_ab **root, float *bvalue);
void generate_ab(lcrs_node_ab **root, int count, float bvalue);
void print_ab_tree(FILE *output, lcrs_node_ab *root, int count);
void dissalloc_ab_tree(lcrs_node_ab *root);
void read_input_ab(FILE *input, lcrs_node_ab *root, dequeue_t_ab *deq,
	char **argv);
float max_value(float *a, float *b);
float min_value(float *a, float *b);
float alpha_max(float *a, float *b, float *c);
float beta_min(float *a, float *b, float *c);
void prune_it(lcrs_node_ab *root, float value);
void get_pruned(lcrs_node_ab **root, stack_t *stk);
void write_output_ab(FILE *output, char **argv, lcrs_node_ab *root);

#endif
