INC_DIR:= ./inc/ ./inc/tinyxml2/ ./inc/splitTool 
SRC_DIR:= ./src/ ./src/tinyxml2/ ./inc/cppJiebaSplitTool
SRCS:= $(wildcard ./src/*.cpp) $(wildcard ./src/tinyxml2/*.cpp) $(wildcard ./src/cppJiebaSplitTool/*.cpp)
OBJS:= $(patsubst %.cpp, %.o, $(SRCS))
LIBS:= -lpthread -ljson -llog4cpp 

CXX:= g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INX_DIR)) $(LIBS) -Wno-deprecated

EXE:= ./bin/search_engine_server.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(SRCS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)


