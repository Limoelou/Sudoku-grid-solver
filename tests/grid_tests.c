#define _DEFAULT_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <colors.h>
#include <grid.h>

/* gcc -I ../include -c grid_tests.c */
/* gcc -o grid_tests grid_tests.o grid.o colors.o */

void
EXPECT (bool test, char *fmt, ...)
{
  fprintf (stdout, "Checking '");

  va_list vargs;
  va_start(vargs, fmt);
  vprintf(fmt, vargs);
  va_end(vargs);

  if (test)
    fprintf (stdout, "': (passed)\n");
  else
    fprintf (stdout, "': (failed!)\n");
}

void
ASSERT (bool test, char *fmt, ...)
{
  fprintf (stdout, "Checking '");

  va_list vargs;
  va_start(vargs, fmt);
  vprintf(fmt, vargs);
  va_end(vargs);

  if (test)
    fprintf (stdout, "': (passed)\n");
  else
    {
      fprintf (stdout, "': (critical fail!) aborting...\n");
      exit (EXIT_FAILURE);
    }
}

void
grid_tests (size_t size)
{
  fprintf (stdout,
	   " Testing grids of size %zu\n"
	   "==========================\n", size);

  /* Checking grid_check_size() */
  EXPECT ((grid_check_size (size)), "grid_check_size (%zu) == true", size);

  /* Allocation of the grid (grid_alloc()) */
  grid_t *grid = grid_alloc (size);
  EXPECT ((grid), "grid_alloc(size) != NULL");

  /* Checking grid_get_size() */
  EXPECT ((grid_get_size (grid) == size), "grid_get_size(grid) == %zu", size);

  /* Checking grid_check_char() */
  for (size_t i = 0; i < size; ++i)
    EXPECT ((grid_check_char (grid, color_table[i])),
	    "grid_check_char('%c') == true", color_table[i]);

  if (size < MAX_COLORS)
    EXPECT ((!grid_check_char (grid, color_table[size])),
	    "grid_check_char('%c') == false", color_table[size]);
  EXPECT ((!grid_check_char (grid, '+')), "grid_check_char('+') == false");
  EXPECT ((!grid_check_char (grid, '0')), "grid_check_char('0') == false");

  /* Checking grid_set_cell() with random initialization of the grid */
  for (size_t i = 0; i < grid_get_size (grid); ++i)
    for (size_t j = 0; j < grid_get_size (grid); ++j)
      grid_set_cell (grid, i, j, color_table[random () % size]);

  /* Checking grid_print() and grid_get_cell() */
  grid_print (grid, stdout);
  EXPECT ((true), "grid_print(grid)");

  /* Checking grid_copy() */
  grid_t *grid2 = grid_copy (grid);
  EXPECT ((true), "grid_copy(grid)");

  bool is_equal = true;
  for (size_t i = 0; i < grid_get_size (grid); ++i)
    for (size_t j = 0; j < grid_get_size (grid); ++j)
      {
	char *str1 = grid_get_cell (grid, i, j),
	     *str2 = grid_get_cell (grid2, i, j);

	if (strcmp (str1, str2))
	  is_equal = false;
	free(str1);
	free(str2);
      }
  EXPECT ((is_equal), "grid == grid_copy(grid)");

  EXPECT ((grid_get_cell (grid, size + 1, 0) == NULL),
	  "grid_get_cell (grid, %zu, 0) == NULL", size + 1);
  EXPECT ((grid_get_cell (grid, 0, size + 1) == NULL),
	  "grid_get_cell (grid, 0, %zu) == NULL", size + 1);
  EXPECT ((grid_get_cell (grid, size, size + 1) == NULL),
	  "grid_get_cell (grid, %zu, %zu) == NULL", size, size + 1);
  EXPECT ((grid_get_cell (grid, size + 1, size) == NULL),
	  "grid_get_cell (grid, %zu, %zu) == NULL", size + 1, size);
  EXPECT ((grid_get_cell (grid, size + 1, size + 1) == NULL),
	  "grid_get_cell (grid, %zu, %zu) == NULL", size + 1, size + 1);


  /* Checking side-effects on an attempt to set a cell out of bounds */
  grid_set_cell (grid, size + 2, size / 2, '1');
  is_equal = true;
  for (size_t i = 0; i < grid_get_size (grid); ++i)
    for (size_t j = 0; j < grid_get_size (grid); ++j)
      {
	char *str1 = grid_get_cell (grid, i, j),
	     *str2 = grid_get_cell (grid2, i, j);

	if (strcmp (str1, str2))
	  is_equal = false;
	free(str1);
	free(str2);
      }
  EXPECT ((is_equal),
	  "no side effect on grid_set_cell(grid, size + 2, size / 2, '1')");

  /* Checking grid_free() */
  grid_free (grid);
  grid_free (grid2);
  EXPECT ((true), "grid_free(grid)");

  fputs ("\n", stdout);
}

int
main (void)
{
  /* Initializing PRNG */
  srandom (time (NULL) - getpid ());

  /* Testing NULLs */
  fputs ("Testing NULL grids\n"
	 "==================\n", stdout);

  /* Checking grid_alloc() */
  EXPECT ((grid_alloc (0) == NULL), "grid_alloc(0) == NULL");
  EXPECT ((grid_alloc (17) == NULL), "grid_alloc(17) == NULL");
  EXPECT ((grid_alloc (65) == NULL), "grid_alloc(65) == NULL");

  /* Checking grid_get_size() */
  EXPECT ((grid_get_size (NULL) == 0), "grid_get_size(NULL) == 0");

  /* Checking grid_free() */
  grid_free (NULL);
  EXPECT ((true), "grid_free(NULL)");

  /* Checking grid_print() */
  grid_print (NULL, stdout);
  EXPECT ((true), "grid_print(NULL, stdout)");

  /* Checking grid_copy() */
  EXPECT ((!grid_copy (NULL)), "grid_copy(NULL) == NULL");

  /* Checking grid_get_cell() */
  EXPECT ((grid_get_cell (NULL, 1, 1) == NULL),
	  "grid_get_cell(NULL, 1, 1) == NULL");

  /* Checking grid_set_cell() */
  grid_set_cell (NULL, 1, 1, '1');
  EXPECT ((true), "grid_set_cell(NULL, 1, 1, '1')");

  fputs ("\n", stdout);

  /* Positive tests on valid grid sizes */
  grid_tests (1);
  grid_tests (4);
  grid_tests (9);
  grid_tests (16);
  grid_tests (25);
  grid_tests (36);
  grid_tests (49);
  grid_tests (64);

  return EXIT_SUCCESS;
}
