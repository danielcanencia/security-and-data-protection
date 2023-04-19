CXX = g++
CXXFLAGS = --std=c++17 -lstdc++fs -Wall

.PHONY : all clean distclean

EXE = kmeans.out
SRC = kmeans.cpp record.cpp group.cpp ../utilities/metrics.cpp ../utilities/input.cpp ../utilities/tree.cpp ../utilities/writeData.cpp ../utilities/hierarchy.cpp
OBJ = $(SRC:.cpp=.o)
INC = $(wildcard *.hpp)

all : $(EXE)
clean :
	$(RM) *.o *.out
distclean : clean
	$(RM) $(EXE)

$(EXE) : $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@
%.o : %.cpp $(INC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

