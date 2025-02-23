CC = gcc
CFLAGS = -Wall -g
LIBS = -lkisszyb

TARGETS = transmit recv
OBJS_TRANSMIT = transmit.o config.o
OBJS_RECV = recv.o config.o

all: $(TARGETS)

transmit: $(OBJS_TRANSMIT)
	$(CC) $(CFLAGS) -o $@ $(OBJS_TRANSMIT) $(LIBS)

recv: $(OBJS_RECV)
	$(CC) $(CFLAGS) -o $@ $(OBJS_RECV) $(LIBS)

transmit.o: transmit.c config.h
	$(CC) $(CFLAGS) -c transmit.c

recv.o: recv.c config.h
	$(CC) $(CFLAGS) -c recv.c

config.o: config.c config.h
	$(CC) $(CFLAGS) -c config.c

clean:
	rm -f $(OBJS_TRANSMIT) $(OBJS_RECV) $(TARGETS)

