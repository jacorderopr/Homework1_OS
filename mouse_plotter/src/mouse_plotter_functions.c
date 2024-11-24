#include "mouse_plotter.h"

int ReadMouseData(const char *file_path, int **numbers, int *num_integers)
{
  FILE *fptr;
  long file_size;

  fptr = fopen(file_path, "rb");
  if (fptr == NULL)
  {
    perror("Error opening file");
    return 1;
  }

  fseek(fptr, 0, SEEK_END);
  file_size = ftell(fptr);
  rewind(fptr);

  *num_integers = file_size / sizeof(int);

  if (*num_integers % 2 != 0)
  {
    fprintf(stderr, "Error: File contains an odd number of integers.\n");
    fclose(fptr);
    return 1;
  }

  *numbers = (int *)malloc(file_size);
  if (*numbers == NULL)
  {
    perror("Memory allocation failed");
    fclose(fptr);
    return 1;
  }

  int items_read = fread(*numbers, sizeof(int), *num_integers, fptr);
  if (items_read != *num_integers)
  {
    fprintf(stderr, "Error reading data from file.\n");
    free(*numbers);
    fclose(fptr);
    return 1;
  }

  fclose(fptr);

  return 0;
}

void PlotMouseData(int *numbers, int num_integers)
{
  int x, y;
  int max_row, max_col;

  initscr();
  noecho();
  curs_set(FALSE);

  getmaxyx(stdscr, max_row, max_col);

  float scale_x = (float)max_col / 100.0;
  float scale_y = (float)max_row / 25.0;

  for (int i = 0; i < num_integers; i += 2)
  {
    x = numbers[i];
    y = numbers[i + 1];

    int term_x = (int)(x * scale_x);
    int term_y = (int)(y * scale_y);

    if (term_x < 0)
      term_x = 0;
    if (term_x >= max_col)
      term_x = max_col - 1;
    if (term_y < 0)
      term_y = 0;
    if (term_y >= max_row)
      term_y = max_row - 1;

    mvprintw(term_y, term_x, "*");

    refresh();

    usleep(10000); // 10 milliseconds

    if (i % 20 == 0)
    {
      clear();
    }
  }

  clear();
  refresh();

  endwin();
}
