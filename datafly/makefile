CXX = g++
CXXFLAGS = --std=c++17 -lstdc++fs -Wall

.PHONY : all clean distclean

EXE = datafly.out
SRC = datafly.cpp generalize.cpp evaluate.cpp ../utilities/hierarchy.cpp ../utilities/input.cpp ../utilities/analysis.cpp ../utilities/tree.cpp ../utilities/frequencies.cpp ../utilities/metrics.cpp ../utilities/writeData.cpp
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

