#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct info {
	int map_width;
	int map_height;
	char background;
	char is_fill;
	float ltX;
	float ltY;
	float radius;
	char draw_char;
	char *result;
} g_info;

g_info g_i;

int ft_strlen(char *str) {
	int i = 0;
	while (*(str + i))
		i++;
	return i;
}

int check_location(int x, int y)
{
	if (sqrtf(powf(g_i.ltX - x, 2) + powf(g_i.ltY - y, 2) ) > g_i.radius )
		return 0; // outside
	if (sqrtf(powf(g_i.ltX - x, 2) + powf(g_i.ltY - y, 2) ) > g_i.radius - 1)
		return 1; // border
	return 2; // inside
}


void execute()
{
	for (int i = 0; i < g_i.map_height * g_i.map_width; ++i)
	{
		// x = i % map_width , y = i / map_width
		int check = check_location(i % g_i.map_width,  i / g_i.map_width);
		if (check == 1 || (g_i.is_fill == 'C' && check == 2))
			*(g_i.result + i) = g_i.draw_char;
	}
}

int error_op()
{
	char *error_op = "Error: Operation file corrupted\n";
	write(1, error_op, ft_strlen(error_op));
	return 1;
}


int main(int ac, char **av) {
	FILE *f;
	char *error_arg = "Error: argument\n";
	char *error_op = "Error: Operation file corrupted\n";

	if (ac != 2) {
		write(1, error_arg, ft_strlen(error_arg));
		return (1);
	}
	f = fopen(*(av + 1), "r");
	if (!f
		|| fscanf(f, "%d %d %c\n", &g_i.map_width, &g_i.map_height, &g_i.background) != 3
		|| fscanf(f, "%c %f %f %f %c\n", &g_i.is_fill, &g_i.ltX, &g_i.ltY, &g_i.radius, &g_i.draw_char) != 5
			) {
		write(1, error_op, ft_strlen(error_op));
		return (1);
	}

	// init background
	g_i.result = (char *) malloc(g_i.map_height * g_i.map_width * sizeof(char));
	for (int i = 0; i < g_i.map_height * g_i.map_width; ++i)
		*(g_i.result + i) = g_i.background;

	execute();

	while (fscanf(f, "%c %f %f %f %c\n", &g_i.is_fill, &g_i.ltX, &g_i.ltY, &g_i.radius, &g_i.draw_char) == 5)
		execute();
	fclose(f);
	for (int i = 0; i < g_i.map_height * g_i.map_width; ++i)
	{
		if (i > 0 && i % g_i.map_width == 0)
			write(1, "\n", 1);
		write(1, (g_i.result + i), 1);
	}
	free(g_i.result);
	return 0;
}
