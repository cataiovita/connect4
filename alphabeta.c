#include <stdio.h>
#include <stdlib.h>
#include "alphabeta.h"

// ~~~~~~~~~~~~~~~~~~~ DOWN COADA~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Functia alocare coada;
dequeue_t_ab *alloc_deq_ab(void)
{
	dequeue_t_ab *deq;

	deq = (dequeue_t_ab *)malloc(sizeof(dequeue_t_ab));
	deq->size = 0;
	deq->front = NULL;
	deq->back = NULL;

	return deq;
}

lcrs_node_ab *front_b(dequeue_t_ab *deq)
{
	return deq->front->data;
}

int push_b(dequeue_t_ab *deq, lcrs_node_ab *data)
{
	if (deq->size == 0)
	{
		push_f_list(&deq->back, data);
		deq->front = deq->back;
		deq->size++;
	}

	else
	{
		push_b_list(&deq->back, data);
		deq->size++;
	}

	return 0;
}

void push_f_list(list_node_ab **head, lcrs_node_ab *data)
{
	list_node_ab *new_node;

	new_node = (list_node_ab *)malloc(sizeof(list_node_ab));

	if (new_node == NULL)
		printf("Memoria nu a putut fi alocata.");

	else
	{
		new_node->data = data;
		new_node->next = *head;
		new_node->prev = NULL;

		if (*head)
		{
			(*head)->prev = new_node;
		}
	}

	*head = new_node;
}

void push_b_list(list_node_ab **head, lcrs_node_ab *data)
{
	list_node_ab *new_node;

	new_node = (list_node_ab *)malloc(sizeof(list_node_ab));

	if (new_node == NULL)
		printf("Memoria nu a putut fi alocata.");

	else
	{
		new_node->data = data;
		(*head)->next = new_node;
		new_node->prev = (*head);
		(*head) = new_node;
		(*head)->next = NULL;
	}
}

void pop_b(dequeue_t_ab *deq)
{
	list_node_ab *delNode;

	delNode = deq->front;
	deq->front = deq->front->next;
	deq->size--;
	free(delNode);
}

int char_to_int_ab(char *gridch)
{
	return *gridch - '0';
}

// Functie obtinere numar de noduri;
int get_number_ab(FILE *input, char *gridch)
{
	int numnodes = 0, temp = 0;

	while (*gridch >= '0' && *gridch <= '9')
	{
		numnodes = char_to_int_ab(gridch);
		numnodes += temp;
		temp = numnodes * 10;
		*gridch = fgetc(input);
	}

	return numnodes;
}

// ~~~~~~~~~~~~~~~~~~~~~ DOWN = STIVA ~~~~~~~~~~~~~~~~~~
// Alocare stiva;
stack_t *alloc_stack(void)
{
	stack_t *stk;

	stk = (stack_t *)malloc(sizeof(stack_t));
	stk->size = 0;
	stk->front = NULL;

	return stk;
}

// Top stiva;
lcrs_node_ab *stack_top(stack_t *stk)
{
	return stk->front->data;
}

// Push stiva implementata cu lista
int stack_push(stack_t *stk, lcrs_node_ab *data)
{
	if (stk->size == 0)
	{
		push_f_list(&stk->front, data);
		stk->size++;
	}

	else
	{
		push_b_list(&stk->front, data);
		stk->size++;
	}

	return 0;
}

lcrs_node_ab *stack_pop(stack_t *stk)
{
	lcrs_node_ab *child;
	list_node_ab *delNode;

	delNode = stk->front;
	child = stk->front->data;

	if (stk->front->prev)
		stk->front = stk->front->prev;

	stk->size--;
	free(delNode);

	return child;
}

// ~~~~~~~~~~~~~~~~~~~~~ DOWN = TREE ~~~~~~~~~~~~~~~~~~~~
// Initializare tree alpha beta;
lcrs_node_ab *init_ab_tree(void)
{
	lcrs_node_ab *root;

	root = (lcrs_node_ab *)calloc(1, sizeof(lcrs_node_ab));
	root->value = 0;
	root->sibling = NULL;
	root->child = NULL;

	return root;
}

lcrs_node_ab *create_child_ab(void)
{
	lcrs_node_ab *addChild;

	addChild = (lcrs_node_ab *)calloc(1, sizeof(lcrs_node_ab));
	addChild->sibling = NULL;
	addChild->child = NULL;

	return addChild;
}

// Generare frunza alpha beta tree
void generate_ab_leaf(lcrs_node_ab **root, dequeue_t_ab *deq, float *value)
{
	if ((*root) != NULL)
		(*root)->value = *value;
}

// Generare copil alpha-beta
void generate_ab_child(lcrs_node_ab **root, dequeue_t_ab *deq, float *number)
{
	lcrs_node_ab *bigChild, *addChild, *temp;
	int i;

	bigChild = create_child_ab();
	bigChild->value = 0;
	(*root)->child = bigChild;
	push_b(deq, bigChild);

	for (i = 0; i < *number - 1; i++)
	{
		temp = bigChild;

		addChild = create_child_ab();
		addChild->value = 0;

		while (temp->sibling)
			temp = temp->sibling;

		temp->sibling = addChild;
		push_b(deq, addChild);
	}
}

float max_ab_child(lcrs_node_ab **root, float *bvalue)
{
	lcrs_node_ab *temp;
	float max;

	temp = (*root)->child;
	max = temp->value;

	while (temp)
	{
		if (max < temp->value && temp->value != *bvalue)
			max = temp->value;

		temp = temp->sibling;
	}

	return max;
}

float min_ab_child(lcrs_node_ab **root, float *bvalue)
{
	lcrs_node_ab *temp;
	float min;

	temp = (*root)->child;
	min = temp->value;

	while (temp)
	{
		if (min > temp->value && temp->value != *bvalue)
			min = temp->value;

		temp = temp->sibling;
	}

	return min;
}

// Generarea arborelui alpha beta
void generate_ab(lcrs_node_ab **root, int count, float bvalue)
{
	if (*root)
	{
		generate_ab(&(*root)->child, count + 1, bvalue);
		generate_ab(&(*root)->sibling, count, bvalue);

		if ((*root)->child && count % 2 == 1 &&
			(*root)->child->value != bvalue)
			(*root)->value = min_ab_child(root, &bvalue);

		else if ((*root)->child && count % 2 == 0 &&
			(*root)->child->value != bvalue)
			(*root)->value = max_ab_child(root, &bvalue);
	}
}

void print_ab_tree(FILE *output, lcrs_node_ab *root, int count)
{
	int i;

	if (root)
	{
		if (root->value != 1.0/0.0)
		{
			for (i = 0; i < count; i++)
				fprintf(output, "%s", "\t");

			fprintf(output, "%g", root->value);
			fprintf(output, "%s", "\n");
		}

		print_ab_tree(output, root->child, count + 1);
		print_ab_tree(output, root->sibling, count);
	}
}

void dissalloc_ab_tree(lcrs_node_ab *root)
{
	if (root)
	{
		dissalloc_ab_tree(root->child);
		dissalloc_ab_tree(root->sibling);
		free(root);
	}
}

// Functie citire fisier
void read_input_ab(FILE *input, lcrs_node_ab *root, dequeue_t_ab *deq,
	char **argv)
{
	lcrs_node_ab *temp = NULL;
	char gridch = '\0';
	float numnodes;
	int sign = 0, depth = 0;

	input = fopen(argv[2], "r");
	fscanf(input, "%d", &depth);
	temp = root;

	while (gridch != EOF)
	{
		gridch = fgetc(input);

		if (gridch == '-')
			sign = 1;

		numnodes = get_number_ab(input, &gridch);

		if (sign == 1)
			numnodes = (-1) * numnodes;

		if (numnodes)
			sign = 0;

		if (gridch == ')')
		{
			generate_ab_child(&temp, deq, &numnodes);
			temp = front_b(deq);
			pop_b(deq);
		}

		if (gridch == ']')
		{
			generate_ab_leaf(&temp, deq, &numnodes);

			if (deq->size != 0)
			{
				temp = front_b(deq);
				pop_b(deq);
			}
		}
	}

	fclose(input);
}

float max_value(float *a, float *b)
{
	if (*a > *b)
		return *a;

	else
		return *b;
}

float min_value(float *a, float *b)
{
	if (*a > *b)
		return *b;

	else
		return *a;
}

float alpha_max(float *a, float *b, float *c)
{
	if (*a > *b && *a > *c)
		return *a;

	else if (*b > *a && *b > *c)
		return *b;

	else
		return *c;
}

float beta_min(float *a, float *b, float *c)
{
	if (*a < *b && *a < *c)
		return *a;

	else if (*b < *a && *b < *c)
		return *b;

	else
		return *c;
}

void prune_it(lcrs_node_ab *root, float value)
{
	if (root)
	{
		prune_it(root->child, value);
		prune_it(root->sibling, value);
		root->value = value;
	}
}

// Functie de prunning alpha-beta
void get_pruned(lcrs_node_ab **root, stack_t *stk)
{
	lcrs_node_ab *temp, *parent;
	int count = -1;
	// Initializare alpha-beta [-inf, inf];
	float alpha = -1.0/0.0;
	float beta = 1.0/0.0;

	temp = *root;

	while (temp || stk->size)
	{
		// Pushez toti copiii pe stiva;
		while (temp && temp->value != 1.0/0.0)
		{
			stack_push(stk, temp);
			temp->alpha = alpha;
			temp->beta = beta;
			temp = temp->child;
			count++;
		}

		// Mut pointerul temporal la inceputul cozii;
		temp = stk->front->data;

		stack_pop(stk);
		count -= 1;
		// Pastrez pointer la parinte pt actualizare
		// valori alpha-beta;
		if (stk->size)
			parent = stk->front->data;

		if (temp && temp->value != 1.0/0.0)
		{
			if (temp->child == NULL)
			{
				// Setez valori alpha-beta parinte, daca
				// pointerul temp e frunza;
				if (count % 2 == 0)
					parent->alpha = max_value(&parent->alpha,
						&temp->value);

				else if (count % 2 == 1)
					parent->beta = min_value(&parent->beta,
						&temp->value);
			}

			else if (temp->child)
			{
				// Setez valori alpha-beta parinte, daca
				// pointerul temp nu mai are copii;
				if (count % 2 == 0)
					parent->alpha = alpha_max(&parent->alpha,
						&temp->alpha,
						&temp->beta);

				else if (count % 2 == 1)
					parent->beta = beta_min(&parent->beta,
						&temp->alpha,
						&temp->beta);
			}
		}

		if (parent)
		{
			// La sfarsitul iteratiei, transmit valorile
			// alpha si beta in adancime urmatorilor copii;
			alpha = parent->alpha;
			beta = parent->beta;
		}

		// Daca alpha >= beta, prune it;
		if (alpha >= beta && temp && temp->sibling)
			prune_it(temp->sibling, 1.0 / 0.0);
		// Ma mut cu temp la urmatorul copil;
		temp = temp->sibling;
	}
}

void write_output_ab(FILE *output, char **argv, lcrs_node_ab *root)
{
	output = fopen(argv[3], "w");
	print_ab_tree(output, root, 0);
	fclose(output);
}
