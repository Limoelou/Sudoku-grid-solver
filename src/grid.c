#include "grid.h"
#include "colors.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <err.h>
#include <math.h>
#include <string.h>

/* Internal structure (hiden from outside) to represent a sudoku grid */
struct _grid_t {
  size_t size;
  colors_t **cells;
};

bool grid_check_char(const grid_t *grid, const char c) {
  size_t size = grid->size;
  switch (size) {
  case 1:
    return (c == '_' || c == '1');
  case 4:
    return (c == '_' || (c >= '1' && c <= '4'));
  case 9:
    return (c == '_' || (c >= '1' && c <= '9'));
  case 16:
    return (c == '_' || (c >= '1' && c <= '9') || (c >= 'A' && c <= 'G'));
  case 25:
    return (c == '_' || (c >= '1' && c <= '9') || (c >= 'A' && c <= 'P'));
  case 36:
    return (c == '_' || (c >= '1' && c <= '9') || (c >= 'A' && c <= 'Z') ||
            c == '@');
  case 49:
    return (c == '_' || (c >= '1' && c <= '9') || (c >= 'A' && c <= 'Z') ||
            c == '@' || (c >= 'a' && c <= 'm'));
  case 64:
    return (c == '_' || (c >= '1' && c <= '9') || (c >= 'A' && c <= 'Z') ||
            c == '@' || (c >= 'a' && c <= 'z') || c == '&' || c == '*');
  default:
    errx(EXIT_FAILURE, "wrong size for the grid !");
  }
}

grid_t *grid_alloc(size_t size) {
  if (!grid_check_size(size)) {
    return NULL;
  }

  colors_t **cells = calloc(size, sizeof(colors_t *));
  if (cells == NULL) {
    errx(EXIT_FAILURE, "can't allocate memory for the grid !");
  }

  for (size_t i = 0; i < size; i++) {
    cells[i] = calloc(size, sizeof(colors_t));
    if (cells[i] == NULL) {
      errx(EXIT_FAILURE, "can't allocate cells[i] in grid_alloc.");
    }
  }

  grid_t *grid = malloc(sizeof(grid_t));
  grid->size = size;
  grid->cells = cells;
  grid_t *grid_ptr = grid;
  return grid_ptr;
}

void grid_free(grid_t *grid) {
  if (grid == NULL) {
    return;
  }

  size_t size = grid->size;
  colors_t **cells = grid->cells;
  for (size_t i = 0; i < size; i++) {
    free(cells[i]);
  }
  free(cells);
  free(grid);
}

void grid_print(grid_t *grid, FILE *fd) {
  if (grid == NULL) {
    return;
  }

  size_t size = grid->size;
  char *output_string = NULL;
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      output_string = grid_get_cell(grid, i, j);
      fputs(output_string, fd);
      free(output_string);
      fputc(' ', fd);
    }
    fputc('\n', fd);
  }
}

bool grid_check_size(const size_t size) {
  bool valid_size = false;
  switch (size) {
  case 1:
  case 4:
  case 9:
  case 16:
  case 25:
  case 36:
  case 49:
  case 64:
    valid_size = true;
    break;
  }
  return valid_size;
}

grid_t *grid_copy(const grid_t *grid) {
  if (grid == NULL) {
    return NULL;
  }

  size_t size = grid->size;
  grid_t *new_grid = grid_alloc(size);
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      new_grid->cells[i][j] = grid->cells[i][j];
    }
  }
  return new_grid;
}

char *grid_get_cell(const grid_t *grid, const size_t row, const size_t column) {
  if (grid == NULL) {
    return NULL;
  }

  size_t size = grid->size;
  if (row > size || column > size) {
    return NULL;
  }

  int string_index = 0;
  colors_t color_cell = grid->cells[row][column];
  char *color_string = malloc(colors_count(color_cell) + 1 * sizeof(char));
  for (size_t color_id = 0; color_id < MAX_COLORS; color_id++) {
    if (colors_is_in(color_cell, color_id)) {
      color_string[string_index] = color_table[color_id];
      string_index++;
    }
  }
  color_string[string_index] = '\0';
  return color_string;
}

size_t grid_get_size(const grid_t *grid) {
  if (grid == NULL) {
    return 0;
  }
  return grid->size;
}

void grid_set_cell(grid_t *grid, const size_t row, const size_t column,
                   const char color) {
  if (grid == NULL || row > grid->size - 1 || column > grid->size - 1) {
    return;
  }

  size_t size = grid->size;
  if (!grid_check_char(grid, color)) {
    return;
  }

  if (color == EMPTY_CELL) {
    grid->cells[row][column] = colors_full(size);
    return;
  }

  char *index = strchr(color_table, color);
  size_t color_id = (index - color_table) / sizeof(char);
  grid->cells[row][column] = colors_set(color_id);
}

bool grid_is_solved(grid_t *grid) {
  if (grid == NULL) {
    return false;
  }

  size_t size = grid->size;
  for (size_t row = 0; row < size; row++) {
    for (size_t column = 0; column < size; column++) {
      colors_t grid_char = grid->cells[row][column];
      if (!colors_is_singleton(grid_char) || grid_char == '_') {
        return false;
      }
    }
  }
  return true;
}

bool grid_is_consistent(grid_t *grid) {
  size_t size = grid->size;
  colors_t subgrid[size];
  size_t cell_id = 0;
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      subgrid[cell_id] = grid->cells[i][j];
      cell_id++;
    }

    if (!subgrid_consistency(subgrid, size)) {
      return false;
    }
    cell_id = 0;
  }
  for (size_t j = 0; j < size; j++) {
    for (size_t i = 0; i < size; i++) {
      subgrid[cell_id] = grid->cells[i][j];
      cell_id++;
    }

    if (!subgrid_consistency(subgrid, size)) {
      return false;
    }
    cell_id = 0;
  }

  double block_size = sqrt((double)grid->size);
  for (size_t i = 0; i < grid->size; i += block_size) {
    for (size_t j = 0; j < grid->size; j += block_size) {
      for (size_t k = i; k < i + block_size; k++) {
        for (size_t l = j; l < j + block_size; l++) {
          subgrid[cell_id] = grid->cells[k][l];
          cell_id++;
        }
      }

      if (!subgrid_consistency(subgrid, size)) {
        return false;
      }
      cell_id = 0;
    }
  }

  // BLOCK PART
  /*size_t block_size = (size_t) sqrt(grid->size); // make a func ?
  for(size_t block_id = 0; block_id < grid->size; block_id++)
  {
      size_t row_offset = (block_id / block_size) * block_size;
      size_t column_offset = (block_id % block_size ) * block_size;
      for(size_t i = 0; i < block_size; i ++)
      {
          for(size_t j = 0; j < block_size; j ++)
          {
              //printf("\t(%ld,%ld) = '%s', (%ld)", i, j,
  grid_get_cell(grid,k,l), colors_count(grid->cells[k][l])); subgrid[cell_id++]
  = grid->cells[j + row_offset][j + column_offset];
          }
          cell_id = 0;
          if(!subgrid_consistency(subgrid,size))
          {
              printf("block %ld inconsistent",block_id);
              return false;
          }
      }
  }*/
  return true;
}

size_t grid_heuristics(grid_t *grid) {
  size_t size = grid->size;
  colors_t *subgrid[size];
  size_t cell_id = 0;
  bool has_changed = true;
  while (has_changed) {
    has_changed = false;
    for (size_t i = 0; i < size; i++) {
      for (size_t j = 0; j < size; j++) {
        subgrid[cell_id] = &(grid->cells[i][j]);
        cell_id++;
      }
      has_changed |= subgrid_heuristics(subgrid, size);
      cell_id = 0;
    }

    for (size_t j = 0; j < size; j++) {
      for (size_t i = 0; i < size; i++) {
        subgrid[cell_id] = &(grid->cells[i][j]);
        cell_id++;
      }
      has_changed |= subgrid_heuristics(subgrid, size);
      cell_id = 0;
    }

    double block_size = sqrt((double)grid->size);
    for (size_t i = 0; i < grid->size; i += block_size) {
      for (size_t j = 0; j < grid->size; j += block_size) {
        for (size_t k = i; k < i + block_size; k++) {
          for (size_t l = j; l < j + block_size; l++) {
            subgrid[cell_id] = &(grid->cells[k][l]);
            cell_id++;
          }
        }
        has_changed |= subgrid_heuristics(subgrid, size);
        cell_id = 0;
      }
    }
  }
  if (grid_is_consistent(grid)) {
    if (grid_is_solved(grid)) {
      return 0;
    }
    return 1;
  }
  return 2;
}
