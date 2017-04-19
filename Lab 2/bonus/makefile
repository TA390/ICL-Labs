GCC = g++
CFLAGS = -c
CXXFLAGS = -std=c++11 -Wall -g -MMD
OBJS = networkMain.o message.o host.o router.o database.o \
       local_network.o errors.o network_message.o
EXECUTABLE = network


$(EXECUTABLE): $(OBJS)
	$(GCC) $(OBJS) -o $(EXECUTABLE)


%.o: %.
	$(GCC) $(CFLAGS) $<


-include $(OBJS:.o=.d)


run:
	./$(EXECUTABLE)


.PHONY: clean


clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(EXECUTABLE)
