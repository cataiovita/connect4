#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paranoic.h"

// Functia alocare coada;
dequeue_t_par *alloc_deq_par(void)
{
	dequeue_t_par *deq;

	deq = (dequeue_t_par *)malloc(sizeof(dequeue_t_par));
	deq->size = 0;
	deq->front = NULL;
	deq->back = NULL;

	return deq;
}

// Functie front coada;
lcrs_node_par *front_par(dequeue_t_par *deq)
{
	return deq->front->data;
}

void push_front_p_list(list_node_par **head, lcrs_node_par *data)
{
	list_node_par *new_node;

	new_node = (list_node_par *)malloc(sizeof(list_node_par));

	if (new_node == NULL)
		printf("Memoria nu a putut fi alocata.");

	else
	{
		new_node->data = data;
		new_node->next = *head;
		*head = new_node;
	}
}

void push_back_p_list(list_node_par **head, lcrs_node_par *data)
{
	list_node_par *new_node;

	new_node = (list_node_par *)malloc(sizeof(list_node_par));

	if (new_node == NULL)
		printf("Memoria nu a putut fi alocata.");

	else
	{
		new_node->data = data;
		(*head)->next = new_node;
		(*head) = new_node;
		(*head)->next = NULL;
	}
}

int push_par(dequeue_t_par *deq, lcrs_node_par *data)
{
	if (deq->size == 0)
	{
		push_front_p_list(&deq->back, data);
		deq->front = deq->back;
		deq->size++;
	}

	else
	{
		push_back_p_list(&deq->back, data);
		deq->size++;
	}

	return 0;
}

void pop_par(dequeue_t_par *deq)
{
	list_node_par *delNode;

	delNode = deq->front;
	deq->front = deq->front->next;
	deq->size--;
	free(delNode);
}

int char_to_int_p(char *gridch)
{
	return *gridch - '0';
}

int get_number_p(FILE *input, char *gridch)
{
	int numnodes = 0, temp = 0;

	while (*gridch >= '0' && *gridch <= '9')
	{
		numnodes = char_to_int_p(gridch);
		numnodes += temp;
		temp = numnodes * 10;
		*gridch = fgetc(input);
	}

	return numnodes;
}

// Functie prin care transform string in fiecare
// prim numar al fiecarui nod, pentru a le compara ulterior;
int first_num(char *number)
{
	char *save;
	int first = 0, temp = 0, sign = 0, i = 0;

	save = strdup(number + 1);

	if (strcmp(save, "null"))
	{
		strtok(save, ",");

		if (save[0] == '-')
		{
			sign = 1;
			i++;
		}

		while (save[i] >= '0' && save[i] <= '9')
		{
			first = char_to_int_p(&save[i]);
			first += temp;
			temp = first * 10;
			i++;
		}
	}

	if (sign == 1)
		first = (-1) * first;

	free(save);

	return first;
}

// ~~~~~~~~~~~~~~~~~~~~~ DOWN = TREE ~~~~~~~~~~~~~~~~~~~~
// Functie de initializare radacina;
lcrs_node_par *init_paran_tree(void)
{
	lcrs_node_par *root;

	root = (lcrs_node_par *)malloc(sizeof(lcrs_node_par));
	root->value = "null";
	root->sibling = NULL;
	root->child = NULL;

	return root;
}

// Functie creare nod copil ar arborelui;
lcrs_node_par *create_child_par(void)
{
	lcrs_node_par *addChild;

	addChild = (lcrs_node_par *)malloc(sizeof(lcrs_node_par));
	addChild->sibling = NULL;
	addChild->child = NULL;

	return addChild;
}
// Functie prin care adaug stringul in nodul final, cand
// gasesc '['
void create_leaf_paran(lcrs_node_par **root, dequeue_t_par *deq,
	char *value, int *plys)
{
	int i;

	if ((*root) != NULL)
	{
		(*root)->value = (char *)calloc(10 * *plys * 2 + 1,
			sizeof(char));
		(*root)->value[0] = '[';

		for (i = 1; i < strlen(value) + 1; i++)
			(*root)->value[i] = value[i - 1];
	}
}

// Functie prin care creez nodurile copil;
void create_twig_paran(lcrs_node_par **root, dequeue_t_par *deq,
	int *number)
{
	lcrs_node_par *bigChild, *addChild, *temp;
	int i;

	// Creez copilul initial si il imping pe coada;
	bigChild = create_child_par();
	bigChild->value = "null";
	(*root)->child = bigChild;
	push_par(deq, bigChild);

	// Parcurg iterativ numarul de noduri necesar
	for (i = 0; i < *number - 1; i++)
	{
		temp = bigChild;

		// Aloc memorie si string-ul null fiecarui nod
		// initial
		addChild = create_child_par();
		addChild->value = "null";

		// Creez legaturile dintre frati;
		while (temp->sibling)
			temp = temp->sibling;

		temp->sibling = addChild;
		push_par(deq, addChild);
	}
}

// Functie prin care compar prima valoare (int)
// a fiecarui nod copil, pentru a propaga valorile in sus;
char *par_max_child(lcrs_node_par **root)
{
	lcrs_node_par *temp;
	int max;
	char *good;

	temp = (*root)->child;
	max = first_num(temp->value);
	good = temp->value;

	while (temp)
	{
		if (max < first_num(temp->value))
		{
			max = first_num(temp->value);
			good = temp->value;
		}

		temp = temp->sibling;
	}

	return good;
}

// Similar, functie de determinare a valorii minime;
char *par_min_child(lcrs_node_par **root)
{
	lcrs_node_par *temp;
	int min;
	char *good;

	temp = (*root)->child;
	min = first_num(temp->value);
	good = temp->value;

	while (temp)
	{
		// Transform fiecare string din nod
		// in numar;
		if (min > first_num(temp->value))
		{
			min = first_num(temp->value);
			good = temp->value;
		}

		temp = temp->sibling;
	}

	return good;
}

// Functie prin care parcurg tot arborele, de la cel
// mai inferior nivel, pana la radacina pt a-l completa
// dupa tehnica paranoic;
void fill_paranoic(lcrs_node_par **root, int count, int players)
{
	if (*root)
	{
		fill_paranoic(&(*root)->child, count + 1, players);
		fill_paranoic(&(*root)->sibling, count, players);

		if (count % players == 0 && (*root)->child)
			(*root)->value = par_max_child(root);

		else if ((*root)->child)
			(*root)->value = par_min_child(root);
	}
}

// Afisez copacul creat;
void print_paran_tree(FILE *output, lcrs_node_par *root, int count)
{
	int i;

	if (root)
	{
		for (i = 0; i < count; i++)
			fprintf(output, "%s", "\t");

		fprintf(output, "%s", root->value);
		fprintf(output, "%s", "\n");

		print_paran_tree(output, root->child, count + 1);
		print_paran_tree(output, root->sibling, count);
	}
}

// Dezaloc memoria
void dissalloc_paran_tree(lcrs_node_par *root)
{
	if (root)
	{
		dissalloc_paran_tree(root->child);
		dissalloc_paran_tree(root->sibling);

		if (root->child == NULL)
			free(root->value);

		free(root);
	}
}

// Functie prin care citesc inputul;
int read_input_paranoic(FILE *input, lcrs_node_par *root,
	dequeue_t_par *deq, char **argv)
{
	lcrs_node_par *temp = NULL;
	char gridch = '\0';
	int numnodes, depth = 0, plys = 0;
	char *buffer = '\0';

	buffer = (char *)calloc(10 * plys * 2 + 1, sizeof(char));
	// Citesc adancimea si numarul de jucatori;
	input = fopen(argv[2], "r");
	fscanf(input, "%d", &depth);
	fscanf(input, "%d", &plys);
	temp = root;

	// Citesc char cu char pana la EOF
	while (gridch != EOF)
	{
		// Iau numarul de noduri necesare destinate creatiei
		gridch = fgetc(input);
		numnodes = get_number_p(input, &gridch);
		// Creez copiii in functie de numarul citit;
		if (gridch == ')')
		{
			create_twig_paran(&temp, deq, &numnodes);
			temp = front_par(deq);
			pop_par(deq);
		}
		// Creez frunzele in functie de stringul citit;
		if (gridch == '[')
		{
			// Stochez initial intr-un buffer pe care il
			// dau ca parametrul functiei de completare
			buffer = (char *)realloc(buffer, 10 * (plys * 2 + 1)
				* sizeof(char));
			fscanf(input, "%s", buffer);
			create_leaf_paran(&temp, deq, buffer, &plys);

			// Scot la fiecare pas capul cozii;
			if (deq->size != 0)
			{
				temp = front_par(deq);
				pop_par(deq);
			}
		}
	}

	free(buffer);
	fclose(input);

	return plys;
}

// Functie prin care scriu outputul;
void write_output_par(FILE *output, char **argv, lcrs_node_par *root)
{
	output = fopen(argv[3], "w");
	print_paran_tree(output, root, 0);
	fclose(output);
}
