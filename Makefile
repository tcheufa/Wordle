SRCS = main.c dict.c LinkedList.c wordle.c solver.c
OBJS = $(SRCS:%.c=%.o)
CFLAGS = -Wall -Wextra -Wmissing-prototypes --pedantic -std=c99
LDFLAGS = -lm
CC = gcc
PROG = wordle

.PHONY: all clean run

all: $(PROG)

$(PROG): $(OBJS)

run: $(PROG)
	./wordle --mode human \
		 	 --answers-file data/possible_answers.txt \
			 --guesses-file data/possible_guesses.txt
clean:
	rm $(OBJS) $(PROG)

LinkedList.o: LinkedList.c LinkedList.h
dict.o: dict.c dict.h
main.o: main.c wordle.h solver.h
wordle.o: wordle.c dict.h wordle.h
solver.o: solver.c LinkedList.h dict.h wordle.h solver.h

