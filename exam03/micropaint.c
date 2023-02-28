#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct info {
	int map_width;
	int map_height;
	char background;
	char is_fill;
	float ltX;
	float ltY;
	float width;
	float height;
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
	if (x < g_i.ltX || x > g_i.ltX + g_i.width || y < g_i.ltY || y > g_i.ltY + g_i.height)
		return 0; // outside
	if (x - g_i.ltX < 1.0 || g_i.ltX + g_i.width - x  < 1.0 || y - g_i.ltY < 1.0|| g_i.ltY + g_i.height - y < 1.0)
		return 1; // border
	return 2; // inside
}

void execute()
{
	for (int i = 0; i < g_i.map_height * g_i.map_width; ++i)
	{
		// x = i % map_width , y = i / map_width
		int check = check_location(i % g_i.map_width,  i / g_i.map_width);
		if (check == 1 || (g_i.is_fill == 'R' && check == 2))
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
	int scan_ret;

	if (ac != 2) {
		write(1, error_arg, ft_strlen(error_arg));
		return (1);
	}
	f = fopen(*(av + 1), "r");
	if (!f)
		return (error_op());
	scan_ret = fscanf(f, "%d %d %c\n", &g_i.map_width, &g_i.map_height, &g_i.background);
	if (scan_ret != 3)
		return (error_op());
	scan_ret = fscanf(f, "%c %f %f %f %f %c\n", &g_i.is_fill, &g_i.ltX, &g_i.ltY, &g_i.width, &g_i.height, &g_i.draw_char);
	if (scan_ret != 6)
		return (error_op());

	// init background
	g_i.result = (char *) malloc(g_i.map_height * g_i.map_width * sizeof(char));
	for (int i = 0; i < g_i.map_height * g_i.map_width; ++i)
		*(g_i.result + i) = g_i.background;

	execute();

	scan_ret = fscanf(f, "%c %f %f %f %f %c\n", &g_i.is_fill, &g_i.ltX, &g_i.ltY, &g_i.width, &g_i.height, &g_i.draw_char);
	while (scan_ret == 6)
	{
		execute();
		scan_ret = fscanf(f, "%c %f %f %f %f %c\n", &g_i.is_fill, &g_i.ltX, &g_i.ltY, &g_i.width, &g_i.height, &g_i.draw_char);
	}
	if (scan_ret != -1)
		return error_op();
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

