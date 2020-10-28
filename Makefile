#Variables
EXE=sudoku

#Special rules  and targets
.PHONY: all build check clean help


#Rules and targets

all: 
	 cd src && $(MAKE)
	 cp -f src/$(EXE) ./

clean:
	cd src && $(MAKE) clean
	rm -rf $(EXE)

help:
	@echo "Usage:"
	@echo  "make [all]\t\tBuild the software "
	@echo "make clean\t\tRemove unnecessary files" 
	@echo "make help\t\tDisplay the help window "
	
