#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
	[4 bytes magic: "RLE\x01"] [pairs: byte | count_byte ...]
	Si un byte aparece solo una vez se almacena igua( byte, 1)
	Soporte: Todos los formatos binarios (PPM, BMP, raw pixel data, text, etc.)
*/

/*
	Uso rápido:

		# Compilar
			gcc -o rle rle.c
		# Comprimir
			./rle compress imagen.ppm imagen.ppm.rle
		# Descomprimir
		./rle decompress imagen.ppm.rle imagen_restaurada.ppm
		
*/

#define MAGIC     "RLE\x01"
#define MAGIC_LEN 4
#define MAX_RUN   255

static FILE *open_binary(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);
	if (!f)
	{
		fprintf(stderr, "Error: cannot open '%s'\n", path);
		exit(1);
	}
	return f;
}

static void compress(FILE *in, FILE *out)
{
	if (fwrite(MAGIC, 1, MAGIC_LEN, out) != MAGIC_LEN)
	{
		fprintf(stderr, "Error: failed to write header\n");
		exit(1);
	}

	int cur = fgetc(in);
	if (cur == EOF)
		return;
	int  next;
	int  ctr = 1;

	while ((next = fgetc(in)) != EOF)
	{
		if (next == cur && ctr < MAX_RUN)
			ctr++;
		else
		{
			fputc(cur, out);
			fputc(ctr, out);
			cur = next;
			ctr = 1;
		}
	}
	fputc(cur, out);
	fputc(ctr, out);
}

static void decompress(FILE *in, FILE *out)
{
	char magic[MAGIC_LEN];
	if (fread(magic, 1, MAGIC_LEN, in) != MAGIC_LEN || memcmp(magic, MAGIC, MAGIC_LEN) != 0)
	{
		fprintf(stderr, "Error: not a valid RLE file (bad magic)\n");
		exit(1);
	}

	int c;
	while ((c = fgetc(in)) != EOF)
	{
		int ctr = fgetc(in);
		if (ctr == EOF)
		{
			fprintf(stderr, "Error: truncated RLE stream\n");
			exit(1);
		}
		
		for (int i = 0; i < ctr; i++)
		{
			if (fputc(c, out) == EOF)
			{
				fprintf(stderr, "Error: write failed\n");
				exit(1);
			}
		}
	}
}

static void usage(const char *prog)
{
	fprintf(stderr,
		"Usage: %s <compress|decompress> <input_file> <output_file>\n"
		"\n"
		"  compress    <input>  <output.rle>   Compress any binary file\n"
		"  decompress  <input.rle>  <output>   Restore original file\n"
		"\n"
		"Examples:\n"
		"  %s compress  image.ppm   image.ppm.rle\n"
		"  %s decompress image.ppm.rle image_out.ppm\n",
		prog, prog, prog);
	exit(1);
}

int	main(int ac, char **av)
{
	if (ac != 4)
		usage(av[0]);

	const char *mode      = av[1];
	const char *path_in   = av[2];
	const char *path_out  = av[3];

	FILE *in  = open_binary(path_in,  "rb");
	FILE *out = open_binary(path_out, "wb");

	if (!strcmp(mode, "compress"))
		compress(in, out);
	else if (!strcmp(mode, "decompress"))
		decompress(in, out);
	else
	{
		fprintf(stderr, "Error: unknown mode '%s'\n", mode);
		usage(av[0]);
	}

	fclose(in);
	fclose(out);
	fprintf(stderr, "Done.\n");
	return 0;
}
