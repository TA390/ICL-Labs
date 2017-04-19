GCC = g++
CXXFLAGS = -std=c++11 -Wall -g -MMD
CFLAGS = -c
EXECUTABLE = chess
OBJECTS = ChessMain.o ChessBoard.o \
	  chessPiece.o rook.o knight.o bishop.o king.o queen.o pawn.o \
	  gameInformation.o output.o


$(EXECUTABLE): $(OBJECTS)
	$(GCC) $(OBJECTS) -o $(EXECUTABLE)


%.o: %.
	$(GCC) $(CFLAGS) $<


-include $(OBJECTS:.o=.d)


run:
	./$(EXECUTABLE)


.PHONY: clean


clean:
	rm -f $(OBJECTS) $(OBJECTS:.o=.d) $(EXECUTABLE)
