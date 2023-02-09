CXX = g++
CXXFLAGS = --std=c++17 -lstdc++fs -Wall

.PHONY : all clean distclean

EXE = kmember.out
SRC = kmember.cpp ../utilities/hierarchy.cpp ../utilities/writeData.cpp distances.cpp info.cpp
OBJ = $(SRC:.cpp=.o)
INC = $(wildcard *.hpp)

all : $(EXE)
clean :
	$(RM) *.o *.out
	$(RM) ../utilities/*.o

distclean : clean
	$(RM) $(EXE)

$(EXE) : $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@
%.o : %.cpp $(INC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

