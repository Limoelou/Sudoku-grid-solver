#Compilation flags
CFLAGS=-Wall -Wextra -std=c11
CPPFLAGS=-I ../include -DDEBUG
LDFLAGS = -lm

#Special rules and targets
.PHONY: all clean help

#Rules and target

all: grid_tests colors_tests

grid_tests: grid_tests.o grid.o colors.o
	@$(CC) -o grid_tests grid.o colors.o grid_tests.o $(LDFLAGS)

colors_tests: colors_tests.o colors.o grid.o
	@$(CC) -o colors_tests colors.o colors_tests.o grid.o $(LDFLAGS)

grid.o: ../src/grid.c ../include/grid.h ../include/colors.h
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c ../src/grid.c

colors.o: ../src/colors.c ../include/grid.h ../include/colors.h
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c ../src/colors.c

grid_tests.o: grid_tests.c ../include/grid.h ../include/colors.h
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c grid_tests.c

colors_tests.o: colors_tests.c ../include/grid.h ../include/colors.h
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c colors_tests.c

clean:
	@rm -f *.o
	@rm -f colors_tests
	@rm -f grid_tests
