#include <stdio.h>
#include <stdlib.h>
#include "minimax.h"

// ~~~~~~~~~~~~~~~~~~~ FUNCTII COADA~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Functia alocare coada (implementata cu liste)
dequeue_t *alloc_deq(void)
{
	dequeue_t *deq;

	deq = (dequeue_t *)malloc(sizeof(dequeue_t));
	deq->size = 0;
	deq->front = NULL;
	deq->back = NULL;

	return deq;
}

// Functie front coada;
lcrs_node_max *front(dequeue_t *deq)
{
	return deq->front->data;
}

// Functie push front pentru liste;
void push_front_list(list_node **head, lcrs_node_max *data)
{
	list_node *new_node;

	new_node = (list_node *)malloc(sizeof(list_node));

	if (new_node == NULL)
		printf("Memoria nu a putut fi alocata.");

	else
	{
		new_node->data = data;
		new_node->next = *head;
		*head = new_node;
	}
}

// Functie de push back liste;
void push_back_list(list_node **head, lcrs_node_max *data)
{
	list_node *new_node;

	new_node = (list_node *)malloc(sizeof(list_node));

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

// Functie push coada;
int push(dequeue_t *deq, lcrs_node_max *data)
{
	if (deq->size == 0)
	{
		push_front_list(&deq->back, data);
		deq->front = deq->back;
		deq->size++;
	}

	else
	{
		push_back_list(&deq->back, data);
		deq->size++;
	}

	return 0;
}

// Functie pop coada;
void pop(dequeue_t *deq)
{
	list_node *delNode;

	delNode = deq->front;
	deq->front = deq->front->next;
	deq->size--;
	free(delNode);
}

// Functie transformare char to int;
int char_to_int(char *gridch)
{
	return *gridch - '0';
}

// Functia prin care transform caracterele citite din fisier
// in int-uri
int get_number(FILE *input, char *gridch)
{
	int numnodes = 0, temp = 0;

	// Parcurg fiecare caracter pana gridch devine diferit
	// de o cifra si returnez numarul de care am nevoie;
	while (*gridch >= '0' && *gridch <= '9')
	{
		numnodes = char_to_int(gridch);
		numnodes += temp;
		temp = numnodes * 10;
		*gridch = fgetc(input);
	}

	return numnodes;
}

// ~~~~~~~~~~~~~~~~~~~~~ DOWN = TREE ~~~~~~~~~~~~~~~~~~~~

// Functie alocare si initializare tree-root;
lcrs_node_max *init_max_tree(void)
{
	lcrs_node_max *root;

	root = (lcrs_node_max *)malloc(sizeof(lcrs_node_max));
	root->value = 0;
	root->sibling = NULL;
	root->child = NULL;

	return root;
}

// Functie prin care creez copilul;
lcrs_node_max *create_child_max(void)
{
	lcrs_node_max *addChild;

	addChild = (lcrs_node_max *)malloc(sizeof(lcrs_node_max));
	addChild->sibling = NULL;
	addChild->child = NULL;

	return addChild;
}

// Functie prin care atribui nodului de tip frunza valoarea necesara;
void create_leaf_child(lcrs_node_max **root, dequeue_t *deq, int *value)
{
	if ((*root) != NULL)
		(*root)->value = *value;
}

// Creare copil al tree-ului minimax;
void create_twig_child(lcrs_node_max **root, dequeue_t *deq, int *number)
{
	lcrs_node_max *bigChild, *addChild, *temp;
	int i;

	// Creez primul copil din subtree;
	bigChild = create_child_max();
	bigChild->value = 0;
	(*root)->child = bigChild;
	// Il pushez pe coada;
	push(deq, bigChild);

	for (i = 0; i < *number - 1; i++)
	{
		temp = bigChild;
		// Pushez pe coada restul de copii(siblings ai primului);
		addChild = create_child_max();
		addChild->value = 0;

		while (temp->sibling)
			temp = temp->sibling;

		temp->sibling = addChild;
		// Ii pushez si pe acestia pe coada;
		push(deq, addChild);
	}
}

// Functie ce imi intoarce maximul dintre lista de noduri copil
int max_child(lcrs_node_max **root)
{
	lcrs_node_max *temp;
	int max;

	temp = (*root)->child;
	max = temp->value;

	// Parcurg lista de copii;
	while (temp)
	{
		// Daca max e mai mare decat o valoare copil
		// setez max val. respectiva;
		if (max < temp->value)
			max = temp->value;

		temp = temp->sibling;
	}

	return max;
}

// Similar o functie ce imi returneaza minimul valorilor
// din toti copiii parintelui;
int min_child(lcrs_node_max **root)
{
	lcrs_node_max *temp;
	int min;

	temp = (*root)->child;
	min = temp->value;

	while (temp)
	{
		if (min > temp->value)
			min = temp->value;

		temp = temp->sibling;
	}

	return min;
}
// Functia prin care compar si extrag cea mai mare, respectiv
// cea mai mica valoare a nodurilor de tip frunza, pentru
// a le propaga mai departe, dinspre ultimul nivel spre radacina;
void fill_upward(lcrs_node_max **root, int count)
{
	if (*root)
	{
		// Ma folosesc de count pentru a numara nivelul la care ma aflu
		fill_upward(&(*root)->child, count + 1);
		fill_upward(&(*root)->sibling, count);

		if ((*root)->child && count % 2 == 1)
			(*root)->value = min_child(root);

		else if ((*root)->child)
			(*root)->value = max_child(root);
	}
}

// Functia prin care afisez arborele, initial, il scriu recursiv in
// fisierul de iesire- output
void print_max_tree(FILE *output, lcrs_node_max *root, int count)
{
	int i;

	if (root)
	{
		for (i = 0; i < count; i++)
			fprintf(output, "%s", "\t");

		fprintf(output, "%d", root->value);
		fprintf(output, "%s", "\n");

		print_max_tree(output, root->child, count + 1);
		print_max_tree(output, root->sibling, count);
	}
}
// Functia prin care dezaloc arborele
void dissalloc_max_tree(lcrs_node_max *root)
{
	if (root)
	{
		dissalloc_max_tree(root->child);
		dissalloc_max_tree(root->sibling);
		free(root);
	}
}
// Functia de citire din fisierul de intrare;
void read_input_max(FILE *input, lcrs_node_max *root, dequeue_t *deq,
	char **argv)
{
	lcrs_node_max *temp = NULL;
	char gridch = '\0';
	int numnodes, sign = 0, depth = 0;

	// Deschid fisierul de intrare;
	input = fopen(argv[2], "r");
	// Initial citesc adancimea;
	fscanf(input, "%d", &depth);
	temp = root;
	// Citesc fiecarecaracter din fisier pana la EOF si cu pointerul
	// temporal temp, pointez de fiecare data la inceputul
	// cozii de la adresa caruia adaug de fiecare data copiii;
	while (gridch != EOF)
	{
		gridch = fgetc(input);

		// Daca gasesc semnul - inainte de vreun numar, flag-ul
		// sign devine 1;
		if (gridch == '-')
			sign = 1;

		// Functia get_number prin care returnez numarul pe care
		// il citesc din fisier, si de care am nevoie pt crearea
		// numarului de noduri din arbore
		numnodes = get_number(input, &gridch);

		// Daca flag-ul devine 1, fac numarul negativ;
		if (sign == 1)
			numnodes = (-1) * numnodes;

		if (numnodes)
			sign = 0;

		// Daca dau de caracterul ')' la final, creez numarul
		// de copii necesar si pushez pe coada adresa acestora
		// la fiecare creatie, dupa care dau pop la front-ul cozii
		if (gridch == ')')
		{
			create_twig_child(&temp, deq, &numnodes);
			temp = front(deq);
			pop(deq);
		}

		// Daca gasesc caracterul ']', atribui valoarea necesara
		// frunzei;
		if (gridch == ']')
		{
			create_leaf_child(&temp, deq, &numnodes);

			if (deq->size != 0)
			{
				// Temp pointeaza de fiecare data la inceputul
				// cozii si continui iteratiile;
				temp = front(deq);
				pop(deq);
			}
		}
	}

	fclose(input);
}

// Functie prin care scriul outputul in fisierul de iesire
void write_output_max(FILE *output, char **argv, lcrs_node_max *root)
{
	output = fopen(argv[3], "w");
	print_max_tree(output, root, 0);
	fclose(output);
}
