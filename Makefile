CC=gcc
PROGRAMS=ws
CFLAGS = $(CF) -Wall -Wextra -Wpedantic -Wstack-usage=512 -Wwrite-strings -Wfloat-equal -Waggregate-return -Winline

wordsort: *.c
	$(CC) -o $(PROGRAMS) $(CFLAGS) *.c

clean:
	@rm -f $(PROGRAMS) *.o
