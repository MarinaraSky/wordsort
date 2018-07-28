CC=gcc
PROGRAMS=ws
CFLAGS = $(CF) -Wall -Wextra -Wpedantic -Wstack-usage=512 -Wwrite-strings -Wfloat-equal -Waggregate-return -Winline 
DEBUG_FLAGS = -g3

ws: Wordsort.c
	$(CC) $(CFLAGS) -o $(PROGRAMS) *.c

debug:
	$(CC) $(DEBUG_FLAGS) $(CFLAGS) -o $(PROGRAMS) *.c
clean:
	@rm -f $(PROGRAMS) *.o


