#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdarg.h>
#include <string.h>

#include <colors.h>

/* gcc -I ../include -c colors_tests.c */
/* gcc -o colors_tests colors_tests.o colors.o */

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

int
main (void)
{
  /* Testing colors_full */
  /***********************/
  fputs ("colors_full\n"
	 "===========\n", stdout);

  EXPECT ((colors_full (0) == 0), "colors_full(0) == []");
  EXPECT ((colors_full (1) == 1), "colors_full(1) == [0]");
  EXPECT ((colors_full (4) == 15), "colors_full(4) == [0,1,2,3]");
  EXPECT ((colors_full (9) == 511), "colors_full(9) == [0,1,2, ... ,8]]");
  EXPECT ((colors_full (16) == 65535), "colors_full(16)) == [0,1, ... ,15]");
  EXPECT ((colors_full (25) == 33554431), "colors_full(25) == [0,1, ... ,24]");
  EXPECT ((colors_full (36) == 68719476735), "colors_full(36) == [0,1, ... ,35]");
  EXPECT ((colors_full (49) == 562949953421311),
	  "colors_full(49) == [0,1, ... ,48]");
  EXPECT ((colors_full (64) == 18446744073709551615ULL),
	  "colors_full(64) == [0,1, ... ,63]");

  /* Has not been specified ! */
  EXPECT ((colors_full (17) == 131071), "colors_full(17) == [0,1, ... ,16]");
  EXPECT ((colors_full (73) == 18446744073709551615ULL),
	  "colors_full(73) == [0,1, ... ,63]");

  fputs ("\n", stdout);


  /* Testing colors_empty */
  /************************/
  fputs ("colors_empty\n"
	 "============\n", stdout);

  EXPECT ((colors_empty () == 0), "colors_empty() == []");

  fputs ("\n", stdout);


  /* Testing colors_set */
  /**********************/
  fputs ("colors_set\n"
	 "==========\n", stdout);

  EXPECT ((colors_set (0) == 1), "colors_set(0) == [0]");
  EXPECT ((colors_set (1) == 2), "colors_set(1) == [1]");
  EXPECT ((colors_set (17) == 131072), "colors_set(17) == [17]");
  EXPECT ((colors_set (42) == 4398046511104ULL), "colors_set(42) == [42]");
  EXPECT ((colors_set (63) == 9223372036854775808ULL), "colors_set(63) == [63]");

  /* Has not been specified */
  EXPECT ((colors_set (79) == 0), "colors_set (79) == []");

  fputs ("\n", stdout);


  /* Testing colors_add */
  /**********************/
  fputs ("colors_add\n"
	 "==========\n", stdout);

  EXPECT ((colors_add (colors_set (27), 4) == 134217744),
	  "colors_add(colors_set(27), 4) == [4,27]");
  EXPECT ((colors_add (colors_set (41), 15) == 2199023288320ULL),
	  "colors_add(colors_set(41), 15) == [15,41]");
  EXPECT ((colors_add (colors_add (colors_set (4), 27), 41) == 2199157473296ULL),
	  "colors_add(colors_add(colors_set(4), 27), 41) == [4,27,41]");
  EXPECT ((colors_add (colors_add (colors_set (4), 0), 41) == 2199023255569ULL),
	  "colors_add(colors_add(colors_set(4), 0), 41) == [0,4,41]");
  EXPECT ((colors_add (colors_add (colors_set (4), 63), 41) == 9223374235878031376ULL),
	  "colors_add(colors_add(colors_set(4), 63), 41) == [4,41,63]");
  EXPECT ((colors_add (colors_add (colors_set (4), 27), 27) == 134217744),
	  "colors_add(colors_add(colors_set(4), 27), 27) == [4,27]");

  /* Has not been specified ! */
  EXPECT ((colors_add (colors_add (colors_set (4), 27), 79) == 134217744),
	  "colors_add(colors_add(colors_set(4), 27), 79) == [4,27]");
  EXPECT ((colors_add (colors_add (colors_set (4), 47), 79) == 140737488355344ULL),
	  "colors_add(colors_add(colors_set(4), 47), 79) == [4,47]");

  fputs ("\n", stdout);


  /* Testing colors_discard */
  /**************************/
  fputs ("colors_discard\n"
	 "==============\n", stdout);

  /* p0 = [1,3,7,42] */
  colors_t p0 = colors_add (colors_add (colors_add (colors_set (1), 3), 7), 42);
  /* p1 = [1,3,42] */
  colors_t p1 = colors_add (colors_add (colors_set (1), 3), 42);
  /* p2 = [3,7,42] */
  colors_t p2 = colors_add (colors_add (colors_set (3), 7), 42);
  /* p3 = [1,7,42] */
  colors_t p3 = colors_add (colors_add (colors_set (1), 7), 42);
  /* p4 = [1,3,7] */
  colors_t p4 = colors_add (colors_add (colors_set (1), 3), 7);

  EXPECT ((colors_discard (p0, 7) == p1),
	  "colors_discard ([1,3,7,42], 7) == [1,3,42]");
  EXPECT ((colors_discard (p0, 1) == p2),
	  "colors_discard ([1,3,7,42], 1) == [3,7,42]");
  EXPECT ((colors_discard (p0, 3) == p3),
	  "colors_discard ([1,3,7,42], 3) == [1,7,42]");
  EXPECT ((colors_discard (p0, 42) == p4),
	  "colors_discard ([1,3,7,42], 42) == [1,3,7]");

  EXPECT ((colors_discard (p0, 17) == p0),
	  "colors_discard ([1,3,7], 17) == [1,3,7]");

  /* Has not been specified ! */
  EXPECT ((colors_discard (p0, 69) == p0),
	  "colors_discard ([1,3,7], 69) == [1,3,7]");
  EXPECT ((colors_discard (colors_empty (), 17) == colors_empty ()),
	  "colors_discard ([], 17) == []");

  fputs ("\n", stdout);


  /* Testing colors_is_in */
  /***************************/
  fputs ("colors_is_in\n"
	 "============\n", stdout);

  EXPECT ((colors_is_in (p0, 1)),
	  "colors_is_in([1,3,7,42], 1) == true");
  EXPECT ((colors_is_in (p0, 3)),
	  "colors_is_in([1,3,7,42], 3) == true");
  EXPECT ((colors_is_in (p0, 7)),
	  "colors_is_in([1,3,7,42], 7) == true");
  EXPECT ((colors_is_in (p0, 42)),
	  "colors_is_in([1,3,7,42], 42) == true");

  EXPECT ((!colors_is_in (p0, 2)),
	  "colors_is_in([1,3,7,42], 2) == false");
  EXPECT ((!colors_is_in (p0, 0)),
	  "colors_is_in([1,3,7,42], 0) == false");
  EXPECT ((!colors_is_in (p0, 63)),
	  "colors_is_in([1,3,7,42], 63) == false");

  /* p3 = [0,1,3,7,42,63] */
  p3 = colors_add (colors_add (p0, 0), 63);

  EXPECT ((colors_is_in (p3, 0)),
	  "colors_is_in([0,1,3,7,42,63], 0) == true");
  EXPECT ((colors_is_in (p3, 63)),
	  "colors_is_in([0,1,3,7,42,63], 63) == true");
  EXPECT ((!colors_is_in (p3, 76)),
	  "colors_is_in([0,1,3,7,42,63], 76) == false");

  EXPECT ((!colors_is_in (colors_empty (), 17)),
	  "colors_is_in([], 17) == false");
  EXPECT ((!colors_is_in (colors_empty (), 42)),
	  "colors_is_in([], 42) == false");
  EXPECT ((!colors_is_in (colors_empty (), 0)),
	  "colors_is_in([], 0) == false");
  EXPECT ((!colors_is_in (colors_empty (), 63)),
	  "colors_is_in([], 63) == false");
  EXPECT ((!colors_is_in (colors_empty (), 76)),
	  "colors_is_in([], 76) == false");

  EXPECT ((colors_is_in (colors_full (64), 17)),
	  "colors_is_in([0, ... ,63], 17) == true");
  EXPECT ((colors_is_in (colors_full (64), 42)),
	  "colors_is_in([0, ... ,63], 42) == true");
  EXPECT ((colors_is_in (colors_full (64), 0)),
	  "colors_is_in([0, ... ,63], 0) == true");
  EXPECT ((colors_is_in (colors_full (64), 63)),
	  "colors_is_in([0, ... ,63], 63) == true");
  EXPECT ((!colors_is_in (colors_full (64), 76)),
	  "colors_is_in([0, ... ,63], 76) == false");

  fputs ("\n", stdout);


  /* Testing colors_negate */
  /*************************/
  fputs ("colors_negate\n"
	 "=============\n", stdout);

  EXPECT ((colors_negate (colors_full (9)) == 18446744073709551104ULL),
	  "colors_negate ([0,1, ... ,8])");

  EXPECT ((colors_negate (colors_full (7)) == 18446744073709551488ULL),
	  "colors_negate ([0,1, ... ,6])");

  EXPECT ((colors_negate (colors_full (36)) == 18446744004990074880ULL),
	  "colors_negate ([0,1, ... ,35])");

  EXPECT ((colors_negate (colors_full (33)) == 18446744065119617024ULL),
	  "colors_negate ([0,1, ... ,32])");

  EXPECT ((colors_negate (colors_full (64)) == 0),
	  "colors_negate ([0,1, ... ,63]) == []");

  fputs ("\n", stdout);


  /* Testing colors_and */
  /**********************/
  fputs ("colors_and\n"
	 "==========\n", stdout);

  /* p0 = [1,2,3,5,7,27,60] */
  p0 = colors_add (colors_add (colors_add (colors_set (1), 2), 3), 5);
  p0 = colors_add (colors_add (colors_add (p0, 7), 27), 60);

  /* p1 = [2,6,7,35,60,62] */
  p1 = colors_add (colors_add (colors_add (colors_set (2), 6), 7), 35);
  p1 = colors_add (colors_add (p1, 60), 62);

  /* p2 = [2,7,60] */
  p2 = colors_add (colors_add (colors_set (2), 7), 60);

  EXPECT ((colors_and (p0, p1) == p2),
	  "colors_and([1,2,3,5,7,27,60], [2,6,7,35,60,62]) == [2,7,60]");
  EXPECT ((colors_and (p1, p0) == p2),
	  "colors_and([2,6,7,35,60,62], [1,2,3,5,7,27,60]) == [2,7,60]");

  /* p3 = [17,24,57] */
  p3 = colors_add (colors_add (colors_set (17), 24), 57);

  EXPECT ((colors_and (p0, p3) == colors_empty ()),
	  "colors_and ([1,2,3,5,7,27,60], [17,24,57]) == []");
  EXPECT ((colors_and (p0, p3) == colors_empty ()),
	  "colors_and ([17,24,57], [1,2,3,5,7,27,60]) == []");

  EXPECT ((colors_and (p0, colors_empty ()) == colors_empty ()),
	  "colors_and (([1,2,3,5,7,27,60], []) == []");
  EXPECT ((colors_and (p0, colors_empty ()) == colors_empty ()),
	  "colors_and (([], [1,2,3,5,7,27,60]) == []");

  EXPECT ((colors_and (colors_empty (), colors_empty ()) == colors_empty ()),
	  "colors_and ([], []) == []");

  fputs ("\n", stdout);


  /* Testing colors_or */
  /*********************/
  fputs ("colors_or\n"
	 "=========\n", stdout);

  /* p3 = [1,2,3,5,6,7,27,35,60,62] */
  p3 = colors_add (colors_add (colors_add (p0, 6), 35), 62);

  EXPECT ((colors_or (p0, p1) == p3),
	  "colors_or([1,2,3,5,7,27,60], [2,6,7,35,60,62]) == [1,2,3,5,6,7,27,35,60,62]");
  EXPECT ((colors_or (p1, p0) == p3),
	  "colors_or([2,6,7,35,60,62], [1,2,3,5,7,27,60]) == [1,2,3,5,6,7,27,35,60,62]");
  EXPECT ((colors_or (p1, p2) == p1),
	  "colors_or([2,6,7,35,60,62], [2,7,60]) == [2,6,7,35,60,62]");
  EXPECT ((colors_or (p2, p1) == p1),
	  "colors_or([2,7,60], [2,6,7,35,60,62]) == [2,6,7,35,60,62]");
  EXPECT ((colors_or (p1, colors_empty ()) == p1),
	  "colors_or([2,6,7,35,60,62], []) == [2,6,7,35,60,62]");
  EXPECT ((colors_or (colors_empty (), p1) == p1),
	  "colors_or([], [2,6,7,35,60,62]) == [2,6,7,35,60,62]");
  EXPECT ((colors_or (colors_empty (), colors_empty ()) == colors_empty ()),
	  "colors_or([], []) == []");

  fputs ("\n", stdout);


  /* Testing colors_xor */
  /**********************/
  fputs ("colors_xor\n"
	 "==========\n", stdout);

  /* p3 = p0 ^ p1 = [1,3,5,6,27,35,62] */
  p3 = colors_add (colors_add (colors_add (colors_set (1), 3), 5), 6);
  p3 = colors_add (colors_add (colors_add (p3, 27), 35), 62);

  EXPECT ((colors_xor (p0, p1) == p3),
	  "colors_xor([1,2,3,5,7,27,60], [2,6,7,35,60,62]) == [1,3,5,6,27,35,62]");
  EXPECT ((colors_xor (p1, p0) == p3),
	  "colors_xor([2,6,7,35,60,62], [1,2,3,5,7,27,60]) == [1,3,5,6,27,35,62]");

  /* p3 = p0 ^ p2 = [1,3,5,27] */
  p3 = colors_add (colors_add (colors_add (colors_set (1), 3), 5), 27);

  EXPECT ((colors_xor (p0, p2) == p3),
	  "colors_xor([1,2,3,5,7,27,60], [2,7,60]) == [1,3,5,27]");
  EXPECT ((colors_xor (p2, p0) == p3),
	  "colors_xor([2,7,60], [1,2,3,5,7,27,60]) == [1,3,5,27]");

  /* p3 = p1 ^ p2 = [6,35,62] */
  p3 = colors_add (colors_add (colors_set (6), 35), 62);

  EXPECT ((colors_xor (p1, p2) == p3),
	  "colors_xor([2,6,7,35,60,62], [2,7,60]) == [6,35,62]");
  EXPECT ((colors_xor (p2, p1) == p3),
	  "colors_xor([2,7,60], [2,6,7,35,60,62]) == [6,35,62]");

  EXPECT ((colors_xor (p0, p0) == colors_empty ()),
	  "colors_xor([1,2,3,5,7,27,60], [1,2,3,5,7,27,60]) == []");
  EXPECT ((colors_xor (p0, colors_empty ()) == p0),
	  "colors_xor([1,2,3,5,7,27,60], []) == [1,2,3,5,7,27,60]");
  EXPECT ((colors_xor (colors_empty (), p0) == p0),
	  "colors_xor([], [1,2,3,5,7,27,60]) == [1,2,3,5,7,27,60]");
  EXPECT ((colors_xor (colors_empty (), colors_empty ()) == colors_empty ()),
	  "colors_xor ([], []) == []");

  fputs ("\n", stdout);


  /* Testing colors_subtract */
  /***************************/
  fputs ("colors_subtract\n"
	 "===============\n", stdout);

  /* p3 = p0\p1 = [1,3,5,27] */
  p3 = colors_add (colors_add (colors_add (colors_set (1), 3), 5), 27);

  EXPECT ((colors_subtract (p0, p1) == p3),
	  "colors_subtract([1,2,3,5,7,27,60], [2,6,7,35,60,62]) == [1,3,5,27]");

  /* p3 = p1\p0 = [6,35,62] */
  p3 = colors_add (colors_add (colors_set (6), 35), 62);

  EXPECT ((colors_subtract (p1, p0) == p3),
	  "colors_subtract([2,6,7,35,60,62], [1,2,3,5,7,27,60]) == [6,35,62]");

  /* p3 = p1\p2 = [6,35,62] */
  p3 = colors_add (colors_add (colors_set (6), 35), 62);

  EXPECT ((colors_subtract (p1, p2) == p3),
	  "colors_subtract([2,6,7,35,60,62], [2,7,60]) == [6,35,62]");
  EXPECT ((colors_subtract (p2, p1) == colors_empty ()),
	  "colors_subtract([2,7,60], [2,6,7,35,60,62]) == []");

  EXPECT ((colors_subtract (p0, colors_empty ()) == p0),
	  "colors_subtract([1,2,3,5,7,27,60], []) == [1,2,3,5,7,27,60]");
  EXPECT ((colors_subtract (colors_empty (), p0) == colors_empty ()),
	  "colors_subtract([], [1,2,3,5,7,27,60]) == []");

  EXPECT ((colors_subtract (colors_empty (), colors_empty ()) == colors_empty ()),
	  "colors_subtract([], []) == []");

  fputs ("\n", stdout);


  /* Testing colors_is_equal */
  /***************************/
  fputs ("colors_is_equal\n"
	 "===============\n", stdout);

  /* p3 = p2 = [2, 7, 60] */
  p3 = colors_add (colors_add (colors_set (2), 7), 60);

  EXPECT ((colors_is_equal (p2, p2)),
	  "colors_is_equal([2, 7, 60], [2, 7, 60]) == true");
  EXPECT ((colors_is_equal (p2, p3)),
	  "colors_is_equal([2, 7, 60], [2, 7, 60])* == true");
  EXPECT ((colors_is_equal (p3, p2)),
	  "colors_is_equal([2, 7, 60], [2, 7, 60])** == true");

  EXPECT ((!colors_is_equal (p0, p2)),
	  "colors_is_equal([1,2,3,5,7,27,60], [2, 7, 60]) == false");

  EXPECT ((!colors_is_equal (p0, colors_empty ())),
	  "colors_is_equal([1,2,3,5,7,27,60], []) == false");
  EXPECT ((!colors_is_equal (colors_empty (), p0)),
	  "colors_is_equal([], [1,2,3,5,7,27,60]) == false");

  EXPECT ((!colors_is_equal (p2, colors_add (p2, 63))),
	  "colors_is_equal([2, 7, 60], [2, 7, 60, 63]) == false");
  EXPECT ((!colors_is_equal (p2, colors_add (p2, 0))),
	  "colors_is_equal([2, 7, 60], [0, 2, 7, 60]) == false");

  EXPECT ((colors_is_equal (colors_empty(), colors_empty ())),
	  "colors_is_equal([], []) == true");

  fputs ("\n", stdout);


  /* Testing colors_is_subset */
  /***************************/
  fputs ("colors_is_subset\n"
	 "================\n", stdout);

  EXPECT ((colors_is_subset (p2, p0)),
	  "colors_is_subset([2,7,60], [1,2,3,5,7,27,60]) == true");
  EXPECT ((!colors_is_subset (p0, p2)),
	  "colors_is_subset([1,2,3,5,7,27,60], [2,7,60]) == false");

  EXPECT ((colors_is_subset (p2, p1)),
	  "colors_is_subset([2,7,60], [2,6,7,35,60,62]) == true");
  EXPECT ((!colors_is_subset (p1, p2)),
	  "colors_is_subset([2,6,7,35,60,62], [2,7,60]) == false");

  EXPECT ((colors_is_subset (colors_set (7), p0)),
	  "colors_is_subset([7], [1,2,3,5,7,27,60]) == true");
  EXPECT ((!colors_is_subset (p0, colors_set (7))),
	  "colors_is_subset([1,2,3,5,7,27,60], [7]) == false");

  EXPECT ((!colors_is_subset (colors_set (0), p0)),
	  "colors_is_subset([0], [1,2,3,5,7,27,60]) == false");
  EXPECT ((!colors_is_subset (p0, colors_set (0))),
	  "colors_is_subset([1,2,3,5,7,27,60], [0]) == false");
  EXPECT ((colors_is_subset (colors_set (0), colors_add (p0, 0))),
	  "colors_is_subset([0], [0, 1, 2, 3, 5, 7, 27, 60]) == true");
  EXPECT ((!colors_is_subset (colors_add (p0, 0), colors_set (0))),
	  "colors_is_subset([0, 1, 2, 3, 5, 7, 27, 60], [0]) == false");

  EXPECT ((!colors_is_subset (colors_set (63), p0)),
	  "colors_is_subset([63], [1,2,3,5,7,27,60]) == false");
  EXPECT ((!colors_is_subset (p0, colors_set (63))),
	  "colors_is_subset([1,2,3,5,7,27,60], [63]) == false");
  EXPECT ((colors_is_subset (colors_set (63), colors_add (p0, 63))),
	  "colors_is_subset([63], [1, 2, 3, 5, 7, 27, 60, 63]) == true");
  EXPECT ((!colors_is_subset (colors_add (p0, 63), colors_set (63))),
	  "colors_is_subset([1, 2, 3, 5, 7, 27, 60, 63], [63]) == false");

  EXPECT ((!colors_is_subset (colors_empty (), p0)),
	  "colors_is_subset([], [1,2,3,5,7,27,60]) == false");
  EXPECT ((!colors_is_subset (p0, colors_empty ())),
	  "colors_is_subset([1,2,3,5,7,27,60], []) == false");

  EXPECT ((!colors_is_subset (colors_empty (), colors_empty ())),
	  "colors_is_subset([], []) == false");

  fputs ("\n", stdout);


  /* Testing colors_is_singleton */
  /*******************************/
  fputs ("colors_is_singleton\n"
	 "===================\n", stdout);

  EXPECT ((!colors_is_singleton (p0)),
	  "colors_is_singleton ([1,2,3,5,7,27,60]) == false");

  EXPECT ((colors_is_singleton (colors_set (17))),
	  "colors_is_singleton ([17]) == true");

  EXPECT ((colors_is_singleton (colors_set (0))),
	  "colors_is_singleton ([0]) == true");

  EXPECT ((colors_is_singleton (colors_set (63))),
	  "colors_is_singleton ([63]) == true");

  EXPECT ((!colors_is_singleton (colors_add (colors_set (0), 63))),
	  "colors_is_singleton ([0,63]) == false");

  EXPECT ((!colors_is_singleton (colors_empty ())),
	  "colors_is_singleton ([]) == false");

  fputs ("\n", stdout);


  /* Testing colors_count */
  /************************/
  fputs ("colors_count\n"
	 "============\n", stdout);

  EXPECT (((colors_count (colors_full(0)) == 0)),
	  "colors_count(colors_full(0)) == 0");
  EXPECT (((colors_count (colors_full(1)) == 1)),
	  "colors_count(colors_full(1)) == 1");
  EXPECT (((colors_count (colors_full(4)) == 4)),
	  "colors_count(colors_full(4)) == 4");
  EXPECT (((colors_count (colors_full(9)) == 9)),
	  "colors_count(colors_full(9)) == 9");
  EXPECT (((colors_count (colors_full(16)) == 16)),
	  "colors_count(colors_full(16)) == 16");
  EXPECT (((colors_count (colors_full(18)) == 18)),
	  "colors_count(colors_full(18)) == 18");
  EXPECT (((colors_count (colors_full(25)) == 25)),
	  "colors_count(colors_full(25)) == 25");
  EXPECT (((colors_count (colors_full(36)) == 36)),
	  "colors_count(colors_full(36)) == 36");
  EXPECT (((colors_count (colors_full(49)) == 49)),
	  "colors_count(colors_full(49)) == 49");
  EXPECT (((colors_count (colors_full(52)) == 52)),
	  "colors_count(colors_full(52)) == 52");
  EXPECT (((colors_count (colors_full(64)) == 64)),
	  "colors_count(colors_full(64)) == 64");
  EXPECT (((colors_count (colors_full(73)) == 64)),
	  "colors_count(colors_full(73)) == 64");

  EXPECT (((colors_count (colors_set(0)) == 1)),
	  "colors_count(colors_set(0)) == 1");
  EXPECT (((colors_count (colors_set(7)) == 1)),
	  "colors_count(colors_set(7)) == 1");
  EXPECT (((colors_count (colors_set(18)) == 1)),
	  "colors_count(colors_set(18)) == 1");
  EXPECT (((colors_count (colors_set(32)) == 1)),
	  "colors_count(colors_set(32)) == 1");
  EXPECT (((colors_count (colors_set(43)) == 1)),
	  "colors_count(colors_set(43)) == 1");
  EXPECT (((colors_count (colors_set(63)) == 1)),
	  "colors_count(colors_set(63)) == 1");

  EXPECT (((colors_count (colors_set(67)) == 0)),
	  "colors_count(colors_set(67)) == 0");

  EXPECT (((colors_count (colors_empty()) == 0)),
	  "colors_count(colors_empty()) == 0");

  EXPECT ((colors_count (colors_add (colors_add (colors_set (4), 27), 41)) == 3),
	  "colors_count ([4, 27, 41]) == 3");

  EXPECT ((colors_count (p0) == 7),
	  "colors_count ([1,2,3,5,7,27,60]) == 7");

  fputs ("\n", stdout);


  /* Testing colors_rightmost */
  /****************************/
  fputs ("colors_rightmost\n"
	 "================\n", stdout);

  EXPECT ((colors_rightmost (p0) == colors_set (1)),
	  "colors_rightmost ([1,2,3,5,7,27,60]) == [1]");

  /* p3 = [37,42,60] */
  p3 = colors_add (colors_add (colors_set (37), 42), 60);

  EXPECT ((colors_rightmost (p3) == colors_set (37)),
	  "colors_rightmost ([37,42,60]) == [37]");

  EXPECT ((colors_rightmost (colors_add (colors_set (0), 63)) == colors_set (0)),
	  "colors_rightmost ([0,63]) == [0]");

  EXPECT ((colors_rightmost (colors_set (0)) == colors_set (0)),
	  "colors_rightmost ([0]) == [0]");

  EXPECT ((colors_rightmost (colors_set (63)) == colors_set (63)),
	  "colors_rightmost ([63]) == [63]");

  EXPECT ((colors_rightmost (colors_empty ()) == colors_empty ()),
	  "colors_rightmost ([]) == []");

  fputs ("\n", stdout);


  /* Testing colors_leftmost */
  /***************************/
  fputs ("colors_leftmost\n"
	 "===============\n", stdout);

  EXPECT ((colors_leftmost (p0) == colors_set (60)),
	  "colors_leftmost ([1,2,3,5,7,27,60]) == [60]");

  /* p3 = [7,22,30] */
  p3 = colors_add (colors_add (colors_set (7), 22), 30);

  EXPECT ((colors_leftmost (p3) == colors_set (30)),
	  "colors_leftmost ([7,22,30]) == [30]");

  EXPECT ((colors_leftmost (colors_add (colors_set (0), 63)) == colors_set (63)),
	  "colors_leftmost ([0,63]) == [63]");

  EXPECT ((colors_leftmost (colors_set (0)) == colors_set (0)),
	  "colors_leftmost ([0]) == [0]");

  EXPECT ((colors_leftmost (colors_set (63)) == colors_set (63)),
	  "colors_leftmost ([63]) == [63]");

  EXPECT ((colors_leftmost (colors_empty ()) == colors_empty ()),
	  "colors_leftmost ([]) == []");

  fputs ("\n", stdout);
  
  /* Testing colors_random */
  /*************************/
  fputs ("colors_random\n"
	 "===============\n", stdout);

  EXPECT ((colors_random (colors_empty ()) == colors_empty ()),
	  "colors_random ([]) == []");

  EXPECT ((colors_random (colors_set (0)) == colors_set (0)),
	  "colors_random ([0]) == [0]");

  EXPECT ((colors_random (colors_set (23)) == colors_set (23)),
	  "colors_random ([23]) == [23]");

  EXPECT ((colors_random (colors_set (43)) == colors_set (43)),
	  "colors_random ([43]) == [43]");

  EXPECT ((colors_random (colors_set (63)) == colors_set (63)),
	  "colors_random ([63]) == [63]");

  /* p3 = [7,22,47] */
  p3 = colors_add (colors_add (colors_set (7), 22), 47);

  colors_t random_color = colors_random (p3);
  EXPECT ((random_color == colors_set (7)  ||
	   random_color == colors_set (22) ||
	   random_color == colors_set (47)),
	  "colors_random ([7,22,47]) == [7] || [22] || [47]");

  fputs ("\n", stdout);

  return EXIT_SUCCESS;
}
