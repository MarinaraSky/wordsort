CC=gcc
PROGRAMS=ws
CFLAGS = $(CF) -Wall -Wextra -Wpedantic -Wstack-usage=512 -Wwrite-strings -Wfloat-equal -Waggregate-return -Winline 

ws: Wordsort.c
	$(CC) $(CFLAGS) -o $(PROGRAMS) *.c

clean:
	@rm -f $(PROGRAMS) *.o
