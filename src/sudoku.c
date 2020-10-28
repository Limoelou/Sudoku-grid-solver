#include "sudoku.h"
#include "colors.h"
#include "grid.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <err.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_GRID_SIZE 64

static bool verbose = false;

/* check if the given path is a regular file */
static int is_file_or_directory(const char *path) {
  struct stat path_to_file;
  stat(path, &path_to_file);
  return S_ISREG(path_to_file.st_mode);
}

/* display the version of the software when calling the 'version' option */
static void display_version() {
  printf("sudoku %d.%d.%d\nSolve/generate sudoku grids"
         "(possible sizes: 1, 4, 9, 16, 25, 36, 49, 64)\n",
         VERSION, SUBVERSION, REVISION);
}

/* display the help panel when calling the 'help' option */
static void display_help() {
  printf(
      "Usage: sudoku [-a|-o FILE |-v|-V|-h] FILE ...\n"
      "\t sudoku -g[SIZE] [-u|-o FILE|-v|-V|-h]\n"
      "Solve or generate Sudoku grids of various sizes (1,4,9,16,25,36,49,64)"
      "\n"
      "-g[N], --generarte[=N]\t generate a grid of size NxN (default:9)\n"
      "-a, --all\t\tsearch for all possible solutions\n"
      "-u,--unique\t\t generate a grid with unique solution\n"
      "-o FILE, --output FILE\t write result to FILE\n"
      "-v, --verbose\t\t verbose output\n"
      "-V, --version\t\t display version and exit\n"
      "-h, --help\t\t display this help\n");
}

static grid_t *file_parser(char *file) {
  /*true if we saw a EOL while parsing */
  bool EOL = false;
  char first_row[MAX_GRID_SIZE];
  struct _grid_t *grid;

  FILE *fd = fopen(file, "r");
  if (fd == NULL) {
    err(EXIT_FAILURE, "null pointer for given file.");
  }

  /* the row number of the grid */
  size_t row = 0;
  /* number of columns */
  size_t row_size = 0;
  int c;
  char tmp;

  while ((c = (char)fgetc(fd)) != EOF) {
    switch (c) {
    case ' ':
    case '\t':
      break;
    case '#':
      while ((c = fgetc(fd)) != '\n' && ((int)c != EOF)) {
        continue;
      }

    case '\n':
      if (row_size == 0) {
        break;
      }

      if (row == 0) {
        grid = grid_alloc(row_size);
        if (grid == NULL) {
          errx(EXIT_FAILURE, "error trying to allocate the grid.");
        }
      } else {
        if (row_size != grid_get_size(grid)) {
          errx(EXIT_FAILURE, "the line %lu is malformed!", row + 1);
        }
      }

      if (row + 1 > grid_get_size(grid)) {
        errx(EXIT_FAILURE, "row %lu exceeds the grid size", row + 1);
      }

      for (size_t column = 0; column < row_size; column++) {
        tmp = first_row[column];
        if (grid_check_char(grid, tmp)) {
          grid_set_cell(grid, row, column, first_row[column]);
        } else {
          errx(EXIT_FAILURE,
               "wrong character %c at"
               " line %lu",
               c, row + 1);
        }
      }

      if (row > grid_get_size(grid)) {
        errx(EXIT_FAILURE, "the row number exceeds the grid size");
      }

      EOL = true;
      row++;
      row_size = 0;
      break;

    default:
      first_row[row_size] = c;
      row_size++;
      EOL = false;
      break;
    }
  }
  if (!EOL) {
    if (row == 0) {
      grid = grid_alloc(row_size);
      if (grid == NULL) {
        errx(EXIT_FAILURE, "error trying to allocate the grid.");
      }
    }

    if (row_size != grid_get_size(grid)) {
      errx(EXIT_FAILURE, "the line %lu is malformed!", row);
    }

    for (size_t i = 0; i < grid_get_size(grid); i++) {
      tmp = first_row[i];
      if (grid_check_char(grid, tmp)) {
        grid_set_cell(grid, row, i, first_row[i]);
      } else {
        errx(EXIT_FAILURE,
             "wrong character %c at"
             "line %lu",
             c, row + 1);
      }
    }
    row++;
  }

  if (row != grid_get_size(grid)) {
    errx(EXIT_FAILURE, "Wrong number of rows");
  }

  if (row > MAX_GRID_SIZE) {
    errx(EXIT_FAILURE, "the last row exceeds the max grid size allowed.");
  }

  fclose(fd);
  return grid;
}

int main(int argc, char *argv[]) {
  bool all = false;
  bool gen_bool = false;
  bool unique = false;
  bool version = false;
  bool help = false;
  bool solved = true;
  int optc;
  FILE *output_fd = stdout;
  /* options descriptor */
  const struct option long_opts[] = {{"generate", optional_argument, NULL, 'g'},
                                     {"all", no_argument, NULL, 'a'},
                                     {"unique", no_argument, NULL, 'u'},
                                     {"output", required_argument, NULL, 'o'},
                                     {"verbose", no_argument, NULL, 'v'},
                                     {"version", no_argument, NULL, 'V'},
                                     {"help", no_argument, NULL, 'h'},
                                     {NULL, 0, NULL, 0}};
  while ((optc = getopt_long(argc, argv, "g::auo:vVh", long_opts, NULL)) !=
         -1) {
    switch (optc) {
    case 'g': /* generate */
      if (optarg) {
        /* the value of the argument given to the 'g' option */
        int optnb = atoi(optarg);
        switch (optnb) {
        case 1:
        case 4:
        case 9:
        case 16:
        case 25:
        case 36:
        case 49:
        case 64:
          break;
        default:
          errx(EXIT_FAILURE, "%d isn't a valid size !", optnb);
        }
      }
      gen_bool = true;
      break;
    case 'a': /* all */
      all = true;
      break;
    case 'u': /* unique */
      unique = true;
      break;
    case 'o': /* output */
      if (optarg == NULL) {
        errx(EXIT_FAILURE, "no output file given.");
      } else {
        char *outfile = optarg;
        FILE *fd = fopen(outfile, "w");

        if (fd == NULL) {
          errx(EXIT_FAILURE, "the file doesn't exist.");
        }
        fclose(fd);
      }
      break;

    case 'v': /* verbose */
      verbose = true;
      break;

    case 'V': /* version */
      display_version();
      version = true;
      return EXIT_SUCCESS;

    case 'h': /* help */
      display_help();
      help = true;
      return EXIT_SUCCESS;

    default:
      errx(EXIT_FAILURE, "invalid option '%s'!", argv[optind - 1]);
    }
  }
  /* check errors in generator mode */
  if (gen_bool) {
    if (all) {
      warnx("'all' conflicts with the generator mode, disabling it !");
      all = false;
    }

    if (optind != argc) {
      errx(EXIT_FAILURE, "error: no arguments in generator mode");
    }
  }
  /* check errors in solver mode */
  else {
    if (unique) {
      warnx("'unique' conflicts with the solver mode, disabling it !");
    }

    if (optind == argc && help == false && version == false) {
      errx(EXIT_FAILURE, "error: no input grid given !");
    }
    /* directory and file related checkings */
    int optindex = optind;
    char *arg_path;
    while (optindex < argc) {
      arg_path = argv[optindex];
      /* check if the path is a valid regular file */
      if (is_file_or_directory(arg_path)) {
        /* check if the file is readable */
        if (access(arg_path, R_OK) == -1) {
          errx(EXIT_FAILURE, "%s has no read permission.", arg_path);
        }
        optindex++;
      } else {
        errx(EXIT_FAILURE, "%s is not a file.", arg_path);
      }

      struct _grid_t *output_grid;
      output_grid = file_parser(arg_path);
      if (grid_heuristics(output_grid) == 2) {
        warnx("grid is inconstent !\n");
        grid_print(output_grid, output_fd);
        grid_free(output_grid);
        solved = false;
      } else if (!grid_heuristics(output_grid) == 1) {
        printf("grid is consistent and solved !\n");
        grid_print(output_grid, output_fd);
        grid_free(output_grid);
      } else {
        warnx("grid is consistent but not solved !\n");
        grid_print(output_grid, output_fd);
        grid_free(output_grid);
      }
    }
    if (solved == false) {
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
}
