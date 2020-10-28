#ifndef COLORS_H
#define COLORS_H

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#define MAX_COLORS 64
#define FULL 9223372036854775807
typedef uint64_t colors_t;

/* set all the bits at 1, set all the possible colors for a given size */
colors_t colors_full (const size_t size);

/* returns the empty colors_t 0. */
colors_t colors_empty (void);

/* shift 1 by the color_id and returns the newly created color */
colors_t colors_set (const size_t color_id);

/* add a new color to the existing colors_t */
colors_t colors_add(const colors_t colors, const size_t color_id);

/* discard a color from the existing colors_t */
colors_t colors_discard(const colors_t colors, const size_t color_id);

/* returns true if the color index is set to '1', false otherwise. */
bool colors_is_in(const colors_t colors, const size_t color_id);

/* Bitwise operation to negate the colors_t and returns it. */
colors_t colors_negate(const colors_t colors);

/* computes the intersection between 2 colors_t and returns it. */
colors_t colors_and(const colors_t colors1, const colors_t colors2);

/* returns the union of two colors_t */
colors_t colors_or(const colors_t colors1, const colors_t colors2);

/* computes the xor of two colors_t */
colors_t colors_xor(const colors_t colors1, const colors_t colors2);

/* returns the bitwise subtract of two colors_t : colors1/colors2 */
colors_t colors_subtract(const colors_t colors1, const colors_t colors2);

/* returns true if two colors are equal, false otherwise. */
bool colors_is_equal(const colors_t colors1, const colors_t colors2);

/* returns true if colors1 is include in color2, false otherwise. */
bool colors_is_subset(const colors_t colors1, const colors_t colors2);

/* checks if there is only one color in the colors */
bool colors_is_singleton(const colors_t colors);

/* returns the number of colors in a colors_t set. */
size_t colors_count(const colors_t colors);

/* returns the rightmost color of a colors_t set */
colors_t colors_rightmost(const colors_t colors);

/* returns the leftmost color of a colors_t set  */
colors_t colors_leftmost(const colors_t colors);

/* generates a random colors_t set */
colors_t colors_random(const colors_t colors);

/* check if the subgrid has no same singletons, each color, and is not empty. */
bool subgrid_consistency(colors_t subgrid[], const size_t size);

bool subgrid_heuristics(colors_t* subgrid[], const size_t size);

#endif /* colors_h */

