#ifndef GRID_H
#define GRID_H

#define MAX_GRID_SIZE 64
#define EMPTY_CELL '_'

#include "colors.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static const char color_table[] =
"123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "@" "abcdefghijklmnopqrstuvwxyz" "&*";

/* Sudoku grid (froward declaration to hide the implementation) */
typedef struct _grid_t grid_t;

/* check if a given char exist in the grid*/
bool grid_check_char(const grid_t *grid, const char c);

/* memory allocation for a grid */
grid_t* grid_alloc(size_t size);

/* free the memory for a given grid */
void grid_free(grid_t *grid);

/* return true if the size is a valid size for the grid, false otherwise */
bool grid_check_size(const size_t size);

void grid_print(grid_t* grid, FILE* fd);
/* copy the given grid in a new memory area and returns it, NULL otherwise. */
grid_t* grid_copy(const grid_t* grid);

/* get the content of a given cell as a string and returns it. */
char* grid_get_cell(const grid_t* grid, const size_t row, const size_t column);

/* return the size of the grid */
size_t grid_get_size(const grid_t* grid);

/* set a grid cell to a specific value */
void grid_set_cell(grid_t* grid, const size_t size, const size_t column,
	const char color);

/* check if the grid has only singletons and is not empty */
bool grid_is_solved(grid_t* grid);

/* check if the grid is */
bool grid_is_consistent(grid_t* grid);

size_t grid_heuristics(grid_t* grid);

#endif /* GRID_H */
