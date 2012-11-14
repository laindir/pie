#include <stdio.h>
#include <stdlib.h>

#define WIDTH 4096
#define DEPTH 256
#define OVERHEAD 1
#define IDXSIZE WIDTH * DEPTH

struct polynomial
{
	unsigned short position;
	unsigned short depth;
	unsigned short iterations;
	unsigned short padding;
};

static unsigned char table[DEPTH][WIDTH];
static struct polynomial index[IDXSIZE];
static unsigned char deltas[WIDTH];

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
					p.position = j;
				}

				p.iterations++;
			}
			else if(p.iterations > OVERHEAD && table[i - 1][p.position] != 0)
			{
				p.depth = i;
				index[k++] = p;
				p.iterations = 0;
			}
			else
			{
				p.iterations = 0;
			}
		}

		if(p.iterations > OVERHEAD && table[i - 1][p.position] != 0)
		{
			p.depth = i;
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
