#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gametree.h"

// Functia prin care citesc configuratia initiala din fisier
char **read_input(FILE *input_table, char **argv, char **move,
	int *n, int *m)
{
	int i = 0, j = 0;
	char gridch;
	char **table;
	// Deschid fisierul si aloc memoriei vectorul move
	// in care stochez mutarea initiala;
	input_table = fopen(argv[2], "r");
	*move = (char *)calloc(2, sizeof(char));
	// Citesc m, n si move next din fisier- prima linie;
	fscanf(input_table, "%d", n);
	gridch = fgetc(input_table);
	fscanf(input_table, "%d", m);
	gridch = fgetc(input_table);
	gridch = fgetc(input_table);
	(*move)[0] = gridch;
	gridch = fgetc(input_table);
	// Identific prima mutare necesara;
	if ((*move)[0] == 'R')
		(*move)[1] = 'B';

	else
		(*move)[1] = 'R';
	// Aloc memorie necesara tablei (matricei)
	table = (char **)malloc(*n * sizeof(char *));

	for (i = 0; i < *n; i++)
		table[i] = (char *)malloc(*m * sizeof(char));

	// Pornesc cu indexul linie invers, pentru a indexa
	// matricea corect, pt afisare; si parcurgere
	i = *n - 1;
	j = 0;

	// Parcurg fisierul si introduc elementele in matricea table;
	// Pornesc invers cu indexarea matricei;
	do {
		// Citesc char cu char din fisierul de intrare;
		gridch = fgetc(input_table);
		// Cand gasesc caracterul R, B sau -, adaug in matrice;
		if (gridch == 'R' || gridch == 'B' || gridch == '-')
		{
			table[i][j] = gridch;
			j++;
		}
		// Cand gasesc randul nou, decrementez indexul
		// liniei, iar cel al coloanei devine 0;
		else if (gridch == '\n')
		{
			i--;
			j = 0;
		}
	} while (gridch != EOF);

	fclose(input_table);

	return table;
}
// Initializez nodul radacina a carei matrice e cea din
// fisierul de intrare;
lcrs_node_t *init_tree(FILE *input_table, char **argv, char **move,
	int *n, int *m)
{
	lcrs_node_t *root = NULL;

	root = (lcrs_node_t *)malloc(sizeof(lcrs_node_t));
	root->table = read_input(input_table, argv, move, n, m);
	root->sibling = NULL;
	root->child = NULL;

	return root;
}

// Functie de inserare a unui caracter in matrice,
// necesara pentru punctul in care realizez variatiile
// mutarilor pe fiecare nivel;
void insert_char_matrix(char **matrix, char move, int *n,
	int *m, int *c)
{
	int i, j = 0;

	// Parcurg toata matricea si pe pozitia la care
	// gasesc -, (pozitie liber), inserez caracterul(R/B)
	for (j = *c; j < *m; j++)
	{
		for (i = 0; i < *n; i++)
		{
			if (i == 0)
			{
				if (matrix[i][j] == '-')
				{
					matrix[i][j] = move;
					*c = j;
					return;
				}
			}

			else if (matrix[i][j] == '-' && matrix[i - 1][j] != '-')
			{
				matrix[i][j] = move;
				return;
			}

			if (i == *n - 1)
				*c = *c + 1;
		}
	}
}

// Functie prin care numar numarul de mutari posibile
// de la fiecare nivel al jocului (mutari reprezentate
// de caracterul -)
int count_pos_moves(char **matrix, int *n, int *m)
{
	int i, j, count = 0;

	for (i = 0; i < *n; i++)
	{
		for (j = 0; j < *m; j++)
		{
			if (i == 0)
			{
				if (matrix[i][j] == '-')
					count++;
			}

			else if (i > 0)
			{
				if (matrix[i][j] == '-' &&
					matrix[i - 1][j] != '-')
					count++;
			}
		}
	}

	return count;
}

// Functie prin care aloc o tabla de joc/ matrice
// a unui nod
void alloc_one_table(char ***table, int *n, int *m)
{
	int i;

	*table = (char **)malloc(*n * sizeof(char *));

	for (i = 0; i < *n; i++)
		(*table)[i] = (char *)malloc((*m + 1) * sizeof(char));
}

// Functie prin care copiez configuratia tablei anterioare
// Pentru a copia fiecare configuratie in toti fratii
// nodului prim;
void copy_prev_table(char **prev, char **current, int *n, int *m)
{
	int i, j = 0;

	for (i = 0; i < *n; i++)
		for (j = 0; j < *m; j++)
			current[i][j] = prev[i][j];
}

// Functie de alocare a memoriei unui nod copil si copiere
// a configuratiei matricei parintelui;
lcrs_node_t *alloc_child(lcrs_node_t **root, int *n, int *m)
{
	lcrs_node_t *addChild;

	addChild = (lcrs_node_t *)malloc(sizeof(lcrs_node_t));

	addChild->sibling = NULL;
	addChild->child = NULL;

	alloc_one_table(&addChild->table, n, m);
	copy_prev_table((*root)->table, addChild->table, n, m);

	return addChild;
}

// Functie prin care verific daca jocul e remiza
// Mutarile nu mai sunt posibile;
int check_draw_game(char **matrix, int *n, int *m)
{
	int i, j;

	for (i = 0; i < *n; i++)
	{
		for (j = 0; j < *m; j++)
		{
			if (matrix[i][j] == '-')
				return -1;
		}
	}

	return 0;
}

// Functie prin care verific daca configuratia actuala este
// o stare de win a playerului, verific pe diagonale si
// orizontala, verticala;
int check_win_game(char **mat, int *n, int *m, char move, int c_move)
{
	int i, j;

	for (i = 0; i < *n; i++)
	{
		for (j = 0; j < *m - 3; j++)
		{
			if (mat[i][j] == move && mat[i][j + 1] == move &&
				mat[i][j + 2] == move &&
				mat[i][j + 3] == move)
				return 0;
		}
	}

	for (i = 0; i < *n - 3; i++)
	{
		for (j = 0; j < *m; j++)
		{
			if (mat[i][j] == move && mat[i + 1][j] == move &&
				mat[i + 2][j] == move &&
				mat[i + 3][j] == move)
				return 0;
		}
	}

	for (i = 3; i < *n; i++)
	{
		for (j = 0; j < *m - 3; j++)
		{
			if (mat[i][j] == move && mat[i - 1][j + 1] == move &&
				mat[i - 2][j + 2] == move &&
				mat[i - 3][j + 3] == move)
				return 0;
		}
	}

	for (i = 3; i < *n; i++)
	{
		for (j = 3; j < *m; j++)
		{
			if (mat[i][j] == move && mat[i - 1][j - 1] == move &&
				mat[i - 2][j - 2] == move &&
				mat[i - 3][j - 3] == move)
				return 0;
		}
	}

	return -1;
}

// Functie prin care creez copiii fiecaru nod parinte;
void create_child(lcrs_node_t **root, char **move, int c_move,
	int *n, int *m)
{
	int count, i, c = 0;
	lcrs_node_t *rotsChild, *addChild, *temp;

	// Initial, verific cate pozitii posibile sunt
	// la nodul parinte;
	count = count_pos_moves((*root)->table, n, m);
	// Aloc nodul copil initial;
	rotsChild = alloc_child(root, n, m);
	// In functie de mutarea curenta, inserez caracterul
	// corespunzator
	if (c_move % 2 == 0)
		insert_char_matrix(rotsChild->table, (*move)[0], n, m, &c);

	else
		insert_char_matrix(rotsChild->table, (*move)[1], n, m, &c);
	// Pornesc crearea fratilor copilului initial;
	(*root)->child = rotsChild;
	// Parcurg numarul de copii necesari;
	for (i = 0; i < count - 1; i++)
	{
		// Aloc memorie copilului
		temp = rotsChild;
		addChild = alloc_child(root, n, m);
		c++;
		// Continui sa inserez in pozitia actuala caract.
		if (c_move % 2 == 0)
			insert_char_matrix(addChild->table,
				(*move)[0], n, m, &c);

		else
			insert_char_matrix(addChild->table,
				(*move)[1], n, m, &c);

		// Creez legaturile necesare dintre frait;
		while (temp->sibling)
			temp = temp->sibling;

		temp->sibling = addChild;
	}
}
// Functie prin care creez arborele de joc, recursiv;
void create_game_tree(lcrs_node_t **root, char **move, int c_move,
	int *n, int *m)
{
	if (*root)
	{
		// Verific daca nu e stare finala (win/draw);
		if (check_draw_game((*root)->table, n, m) != 0 &&
			check_win_game((*root)->table, n, m,
				(*move)[0], c_move) &&
			check_win_game((*root)->table, n, m,
				(*move)[1], c_move))
			create_child(root, move, c_move, n, m);

		// Creez recursiv copilul, apoi fratii lui;
		create_game_tree(&(*root)->child, move, c_move + 1, n, m);
		create_game_tree(&(*root)->sibling, move, c_move, n, m);
	}
}

// Functie de afisare a matricei fiecarui nod;
void print_table(FILE *output, lcrs_node_t *root, int count,
	int *n, int *m)
{
	int i, j, k;

	for (i = *n - 1; i >= 0; i--)
	{
		for (j = 0; j < count; j++)
			fprintf(output, "%s", "\t");

		for (k = 0; k < *m; k++)
		{
			if (k == *m - 1)
				fprintf(output, "%c", root->table[i][k]);

			else
				fprintf(output, "%c ", root->table[i][k]);
		}

		fprintf(output, "%s", "\n");
	}
}

// Functie prin care parcurg tot arborele si apelez functia
// anterioara de printare a fiecarei matrici din interior;
void print_game_tree(FILE *output, lcrs_node_t *root, int count,
		int *n, int *m)
{
	if (root)
	{
		print_table(output, root, count, n, m);
		fprintf(output, "%s", "\n");
		print_game_tree(output, root->child, count + 1, n, m);
		print_game_tree(output, root->sibling, count, n, m);
	}
}

// Functie de scriere a output.
void write_output(FILE *output, char **argv, lcrs_node_t *root,
	int *n, int *m)
{
	output = fopen(argv[3], "w");
	print_game_tree(output, root, 0, n, m);
	fclose(output);
}

// Dezaloc memoria fiecarui copil;
void diss_alloc_child(lcrs_node_t *root, int *n)
{
	int i;

	for (i = 0; i < *n; i++)
		free(root->table[i]);

	free(root->table);
	free(root);
}

// Dezaloc memoria arborelui;
void diss_alloc_tree(lcrs_node_t *root, int *n)
{
	if (root)
	{
		diss_alloc_tree(root->child, n);
		diss_alloc_tree(root->sibling, n);
		diss_alloc_child(root, n);
	}
}
