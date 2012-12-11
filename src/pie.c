/*

Copyright 2012 Carl D Hamann

This file is part of pie.

pie is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

pie is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with pie.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 4096
#define DEPTH 256
#define OVERHEAD 6

typedef struct polynomial
{
	size_t offset;
	size_t length;
	size_t iterations;
} polynomial_s;

DECLARE_BINARYHEAP(polynomial_s)
DEFINE_BINARYHEAP(polynomial_s)

u_int8_t log8(size_t x);
int itersort(polynomial_s_p a, polynomial_s_p b);
size_t encode(const u_int8_t *data, size_t dlength, u_int8_t *output, size_t olength, const u_int8_t *sentinel, size_t slength, size_t depth);

static u_int8_t table[DEPTH][WIDTH];

int main(void)
{
	int i;
	int j;
	int k;
	int l;
	size_t r;
	struct polynomial p;

	r = fread(&table[0][0], sizeof(table[0][0]), WIDTH, stdin);

	k = 0;

	for(i = 1; i < DEPTH; i++)
	{
		for(j = 0; j + i < r; j++)
		{
			table[i][j] = table[i - 1][j + 1] - table[i - 1][j];

			if(table[i][j] == 0)
			{
				if(p.iterations == 0)
				{
					p.offset = j;
				}

				p.iterations++;
			}
			else if(p.iterations > OVERHEAD && table[i - 1][p.offset] != 0)
			{
				p.length = i;
				index[k++] = p;
				p.iterations = 0;
			}
			else
			{
				p.iterations = 0;
			}
		}

		if(p.iterations > OVERHEAD && table[i - 1][p.offset] != 0)
		{
			p.length = i;
			index[k++] = p;
		}

		p.iterations = 0;
	}

	l = 0;

	for(i = 0; i < k; i++)
	{
		for(j = 0; j < index[i].depth; j++)
		{
			deltas[l++] = table[j][index[i].position];
		}
	}

	fwrite(&k, sizeof(k), 1, stdout);
	fwrite(index, sizeof(*index), k, stdout);
	fwrite(deltas, sizeof(*deltas), l, stdout);
	fclose(stdout);
	fclose(stdin);

	exit(EXIT_SUCCESS);
}

int itersort(polynomial_s_p a, polynomial_s_p b)
{
	if(a->iterations < b->iterations)
	{
		return 1;
	}
	else if(a->iterations > b->iterations)
	{
		return -1;
	}
	else if(a->depth < b->depth)
	{
		return -1;
	}
	else if(a->depth > b->depth)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

u_int8_t log8(size_t x)
{
	u_int8_t i;
	for(i = 0; x != 0; i++)
	{
		x >>= 8;
	}

	return i;
}

size_t encode(const u_int8_t *data, size_t dlength, u_int8_t *output, size_t olength, const u_int8_t *sentinel, size_t slength, size_t depth)
{
	u_int8_t **table;
	u_int8_t *tb;

	polynomial_s **index;
	polynomial_s *ix;

	polynomial_s p;

	struct polynomial_s_binaryheap heap;

	size_t i;
	size_t j;
	size_t k;

	index = calloc(dlength, sizeof(*index));

	if(index == NULL)
	{
		return 0;
	}

	ix = calloc(dlength, sizeof(*ix));

	if(ix == NULL)
	{
		free(index);
		return 0;
	}
	
	table = calloc(depth, sizeof(*table));

	if(table == NULL)
	{
		free(index);
		free(ix);
		return 0;
	}

	tb = calloc(dlength * depth, sizeof(*tb));

	if(tb == NULL)
	{
		free(index);
		free(ix);
		free(table);
		return 0;
	}

	for(i = 0; i < depth; i++)
	{
		table[i] = tb + i * dlength;
	}

	memcpy(&table[0][0], data, dlength);

	heap = polynomial_s_binaryheap(index, dlength, 0, NULL, itersort);

	for(i = 0; i < dlength && i < depth; i++)
	{
		k = i;
		for(j = 0; j < i; j++)
		{
			table[k][j] = table[k - 1][j + 1] - table[k - 1][j];
			if(table[k][j] == 0)
			{
			}

			k--;
		}
	}

	for(i = depth; i < dlength; i++)
	{
		k = depth - 1;
		for(j = i - depth; j < i; j++)
		{
			table[k][j] = table[k - 1][j + 1] - table[k - 1][j];
			k--;
		}
	}

	free(index);
	free(ix);
	free(table);
	free(tb);
}
