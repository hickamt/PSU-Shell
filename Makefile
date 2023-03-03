# Todd Hickam: Created for PSU CS 333 Winter Term 2022 - 2023
# 

CC = gcc
DEBUG = -g
CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
-Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
-Wdeclaration-after-statement -Wno-return-local-addr \
-Wuninitialized -Wextra -Wunused

# MAIN = main(target) entry
MAIN = psush
FILE2 = cmd_parse
FILE3 = exec_cmd
FILE4 = pipe_cmd
TEST = cmd_test
OUT = psush
TAR_FILE = ${LOGNAME}_$(MAIN).tar.gz
COMMENT = "This is a comment"

all: $(MAIN)

$(MAIN): $(MAIN).o $(FILE2).o $(FILE3).o $(FILE4).o $(TEST).o
	$(CC) $(CFLAGS) -o $(OUT) $(MAIN).o $(FILE2).o $(FILE3).o $(FILE4).o $(TEST).o
	chmod og-rx $(OUT)

# MAIN depends on FILE2.h, does not have its own .h :(
$(MAIN).o: $(MAIN).c $(FILE2).h
	$(CC) $(CFLAGS) -c $(MAIN).c

# Where the magic happens FILE2
$(FILE2).o: $(FILE2).c $(FILE2).h
	$(CC) $(CFLAGS) -c $(FILE2).c

# Abstracting the mess of exec commands
$(FILE3).o: $(FILE3).c $(FILE3).h
	$(CC) $(CFLAGS) -c $(FILE3).c

# Abstracting Multi Pipe Commands
$(FILE4).o: $(FILE4).c $(FILE4).h
	$(CC) $(CFLAGS) -c $(FILE4).c

# Test Commands 
$(TEST).o: $(TEST).c $(TEST).h
	$(CC) $(CFLAGS) -c $(TEST).c


# clean up the compiled files and editor chaff
clean cls:
	rm -f $(OUT) *.txt *.o *~ \#*

ci:
	if [ ! -d RCS ] ; then mkdir RCS; fi
	ci -t-none -m"lazy-checkin" -l *.[ch] ?akefile *.bash

DIR = $(shell basename $(PWD))
# Will Clean and Make the .tar.gz for Canvas, placed in upper directory, $ cd ..
canvas:
	make clean
	cd .. ; tar --exclude-backups --exclude-vcs -c -a -f ./${LOGNAME}-$(DIR)-psush.tar.gz $(DIR)

# opt: clean
# 	make DEBUG=-O3

# my .git is at /CS333_Labs and not Lab2
# git get gat:
# 	if [ ! -d .git ] ; then git init; fi
# 	git add *.[ch] ?akefile
# 	git commit -m "standard save"
