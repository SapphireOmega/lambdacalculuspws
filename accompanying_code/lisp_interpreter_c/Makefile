CC = gcc
CFLAGS = -Wall -std=c99
SRCS = util.c array.c token.c list.c program.c environment.c main.c
OBJS = $(SRCS:.c=.o)
MAIN = lispy

.PHONY: depend clean

all: $(MAIN)
	@echo Compilation successful

debug: CFLAGS += -g
debug: all

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	$(RM) *.o *~
