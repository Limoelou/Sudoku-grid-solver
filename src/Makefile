#Variables
EXE=sudoku
#Usual compilation flags
CFLAGS = -std=c11 -g -Wall -Wextra -O2
CPPFLAGS = -I../include  -DDEBUG
LDFLAGS = -lm

#Special rules and targets
.PHONY: all clean help  

all: $(EXE)

$(EXE): sudoku.o grid.o colors.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^  $(LDFLAGS)

sudoku.o: sudoku.c sudoku.h ../include/grid.h ../include/colors.h
	$(CC) -c $< -o $@ $(CLFAGS) $(CPPFLAGS)

grid.o: grid.c ../include/grid.h ../include/colors.h
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

colors.o: colors.c ../include/colors.h ../include/grid.h
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean: 
	rm -f *.o $(EXE)

help:
	@echo "Usage:"
	@echo "make [all]\t\tBuild the software"
	@echo "make clean\t\tRemove unnecessary files"


