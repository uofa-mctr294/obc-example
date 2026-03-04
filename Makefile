CXXFLAGS = -g -O

all: exalta3 gs

OBC.hpp: Opcodes.hpp
PayloadHandler.hpp: Message.hpp

EX3_OBJ = exalta3.o OBC.o Message.o PayloadHandler.o

OBC.o: OBC.hpp Message.hpp PayloadHandler.hpp
Message.o: Message.hpp
PayloadHandler.o: PayloadHandler.hpp

exalta3: $(EX3_OBJ)
	$(CXX) -o $@ $^

gs: gs.o
	$(CXX) -o $@ $^

.PHONY: clean
clean:
	-rm gs exalta3 *.o
