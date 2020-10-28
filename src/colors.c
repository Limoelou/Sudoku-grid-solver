#include "colors.h"
#include "grid.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <inttypes.h>

colors_t colors_full(const size_t size) {
  if (size <= 63) {
    return (((colors_t)1 << size) - 1);
  }
  return (((colors_t)1 << MAX_COLORS) - 1);
}

colors_t colors_empty(void) { return (colors_t)0; }

colors_t colors_set(const size_t color_id) {
  if (color_id < MAX_COLORS) {
    return ((colors_t)1 << color_id);
  }
  return colors_empty();
}

colors_t colors_add(const colors_t colors, const size_t color_id) {
  if (color_id < MAX_COLORS) {
    return (colors_t)(colors | ((colors_t)1 << (color_id)));
  }
  return colors;
}

colors_t colors_discard(const colors_t colors, const size_t color_id) {
  if (color_id < MAX_COLORS) {
    return (colors ^ ((colors_t)1 << (color_id))) & colors;
  }
  return colors;
}

bool colors_is_in(const colors_t colors, const size_t color_id) {
  if (color_id < MAX_COLORS) {
    return (colors & ((colors_t)1 << color_id)) != 0;
  }
  return false;
}

colors_t colors_negate(const colors_t colors) { return ~colors; }

colors_t colors_and(const colors_t colors1, const colors_t colors2) {
  return (colors1 & colors2);
}

colors_t colors_or(const colors_t colors1, const colors_t colors2) {
  return (colors1 | colors2);
}

colors_t colors_xor(const colors_t colors1, const colors_t colors2) {
  return (colors1 ^ colors2);
}

colors_t colors_subtract(const colors_t colors1, const colors_t colors2) {
  return colors1 ^ (colors1 & colors2);
}

bool colors_is_equal(const colors_t colors1, const colors_t colors2) {
  return (colors1 ^ colors2) == 0;
}

bool colors_is_subset(const colors_t colors1, const colors_t colors2) {
  if (colors1 == 0 || colors2 == 0) {
    return false;
  }
  return (colors1 & colors2) == colors1;
}

bool colors_is_singleton(const colors_t colors) {
  if (colors == 0) {
    return false;
  }

  return (colors & (colors - 1)) == 0;
}

size_t colors_count(const colors_t colors) {
  uint64_t y = colors;
  y -= ((y >> 1) & 0x5555555555555555ull);
  y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
  return ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
}

colors_t colors_rightmost(const colors_t colors) {
  return (colors & (-colors));
}

colors_t colors_leftmost(const colors_t colors) {
  if (colors == 0) {
    return colors_empty();
  }
  int pos = 0;
  while ((colors >> pos) > 0 && pos < MAX_COLORS) {
    pos++;
  }
  return colors_set(pos - 1);
}

colors_t colors_random(const colors_t colors) {
  if (colors == 0) {
    return colors_empty();
  } else {
    while (true) {
      size_t r = rand() % 64;
      if (colors_is_in(colors, r)) {
        return (colors_set(r));
      }
    }
  }
}

bool subgrid_consistency(colors_t subgrid[], const size_t size) {
  colors_t colors_seen = colors_full(size);
  if (subgrid == NULL) {
    return false;
  }
  // bool consistent = false;
  for (size_t i = 0; i < size; i++) {
    colors_t sub_cell = subgrid[i];
    if (colors_count(subgrid[i] == 0)) {
      return false;
    }
    if (colors_is_singleton(sub_cell)) {
      for (size_t j = i + 1; j < size; j++) {
        colors_t singleton_cell = subgrid[j];
        if (colors_is_singleton(singleton_cell)) {
          if (colors_is_equal(sub_cell, singleton_cell)) {
            return false;
          }
        }
      }
    }
    colors_seen = colors_subtract(colors_seen, sub_cell);
  }
  if (!colors_is_equal(colors_seen, colors_empty())) {
    return false;
  }
  return true;
}

static bool cross_hatching(colors_t *subgrid[], size_t size) {
  bool cross_hatch = false;
  for (size_t i = 0; i < size; i++) {
    if (colors_is_singleton(*(subgrid[i]))) {
      for (size_t j = 0; j < size; j++) {
        if (i != j) {
          if (*(subgrid[j]) == colors_subtract(*(subgrid[j]), *(subgrid[i]))) {

          } else {
            *(subgrid[j]) = colors_subtract(*(subgrid[j]), *(subgrid[i]));
            cross_hatch = true;
          }
        }
      }
    }
  }
  return cross_hatch;
}

static bool lone_number(colors_t *subgrid[], size_t size) {
  /*bool lone_numb = false;
  for (size_t i = 0; i < size; i++) {
    colors_t subset = colors_full(size);
    subset = colors_subtract(subset, *(subgrid[i]));
    subset = colors_negate(subset);
    for (size_t j = 0; j < size; j++) {
      if (j != i) {
        subset = colors_subtract(subset, *(subgrid[j]));
      }
    }
    if (colors_count(subset) > 0) {
      *(subgrid[i]) = colors_set(colors_leftmost(subset));
      lone_numb = true;
    }
  }
  return lone_numb;
}*/
  bool lone_numb = false;
  for (size_t i = 0; i < size; i++) {
    if (!colors_is_singleton(*(subgrid[i]))) {
      for (size_t j = 0; j < size; j++) {
        if (colors_is_in(*(subgrid[i]), j)) {
          for (size_t k = 0; k < size; k++) {
            if (colors_is_in(*(subgrid[k]), j) && k != i) {
              return false;
            }
          }
          *(subgrid[i]) = colors_set(j);
          lone_numb = true;
        }
      }
    }
  }
  return lone_numb;
}

/*static bool naked_subset(colors_t* subgrid[], size_t size)
{
    bool naked_sub = false;
    for(size_t i = 0; i < size; i++)
    {
        if(!colors_is_singleton(*(subgrid[i])))
        {
            for(size_t j = 0; j < size; j++)
            {
                if(colors_is_subset(*(subgrid[i]), *(subgrid[j])))
                {
                    *(subgrid[j]) = colors_subtract(*(subgrid[j]),
*(subgrid[i])); naked_sub = true;
                }
            }
        }
    }
    return naked_sub;
}*/

static bool naked_subset(colors_t *subgrid[], size_t size) {
  bool naked_sub = false;
  for (size_t i = 0; i < size; i++) {
    if (!colors_is_singleton(*(subgrid[i]))) {
      for (size_t j = 0; j < size; j++) {
        if (j != i && colors_is_equal(*(subgrid[i]), *(subgrid[j]))) {
          for (size_t k = 0; k < size; k++) {
            if (k != j && k != i) {
              if (*subgrid[k] == colors_discard(*subgrid[k], *(subgrid[i]))) {

              } else {
                *(subgrid[k]) = colors_discard(*(subgrid[k]), *(subgrid[i]));
                naked_sub = true;
              }
            }
          }
        }
      }
    }
  }
  return naked_sub;
}

bool subgrid_heuristics(colors_t *subgrid[], size_t size) {
  bool res = false;
  res |= cross_hatching(subgrid, size);
  res |= lone_number(subgrid, size);
  // res |= naked_subset(subgrid, size);
  return res;
}
