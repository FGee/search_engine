INC_DIR:= ./
SRC_DIR:= ./
SRCS:=$(wildcard *.cpp)
OBJS:= $(patsubst %.cpp, %.o, $(SRCS))
LIBS:= -ljson 

CXX:=g++

CXXFLAGS:= -w -g  $(addprefix -I, $(INC_DIR)) $(LIBS) 

EXE:=./myClient.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
