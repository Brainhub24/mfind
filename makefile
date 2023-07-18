CC = gcc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -L. -llogger
SHARED_LIB = liblogger.so

all: mfind $(SHARED_LIB)

mfind: mfind.o $(SHARED_LIB)
	$(CC) $(CFLAGS) -o $@ mfind.o $(LDFLAGS)

mfind.o: mfind.c
	$(CC) $(CFLAGS) -c $<

$(SHARED_LIB): logger.o
	$(CC) $(CFLAGS) -shared -o $@ logger.o

logger.o: logger.c logger.h
	$(CC) $(CFLAGS) -fPIC -c logger.c

clean:
	rm -f mfind *.o $(SHARED_LIB)

.PHONY: all clean
