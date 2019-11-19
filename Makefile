CC = gcc
CFLAGS = -Wall
DEPS = pagetable.h
OBJ = tp3virtual.o pagetable.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

tp3virtual: $(OBJ)
	gcc $(CFLAGS) -o $@ $^