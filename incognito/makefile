CXX = g++
CXXFLAGS = --std=c++17 -lstdc++fs -Wall

.PHONY : all clean distclean

EXE = incognito.out
SRC = incognito.cpp graph.cpp graphNode.cpp edge.cpp graphData.cpp ../utilities/hierarchy.cpp ../utilities/analysis.cpp ../utilities/frequencies.cpp ../utilities/tree.cpp ../utilities/writeData.cpp
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

