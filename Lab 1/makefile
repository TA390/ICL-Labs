CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
STD = -std=c++1y

OBJS = main.o tube.o

tube: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o tube $(STD)

main.o: main.cpp tube.h
	$(CC) $(CFLAGS) main.cpp $(STD)

tube.o: tube.cpp tube.h
	$(CC) $(CFLAGS) tube.cpp $(STD)

run:
	./tube

clean:
	rm -f *.o tube
