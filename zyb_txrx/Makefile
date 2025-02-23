CC = gcc
CFLAGS = -Wall -g
LIBS = -lkisszyb

TARGET = transmit
OBJS = transmit.o config.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

transmit.o: transmit.c config.h
	$(CC) $(CFLAGS) -c transmit.c

config.o: config.c config.h
	$(CC) $(CFLAGS) -c config.c

clean:
	rm -f $(OBJS) $(TARGET)

