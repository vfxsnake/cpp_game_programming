# for compiling and running at once type
# make run

# defin whch compiler to use

CXX:= g++
OUTPUT:= sfmlgame
OS:$(shell uname)

#linux compiler and linker flags
ifeq ($(OS), Linux)
	CXX_FLAGS:= -03 -std=c++20 -Wno-unused-result -Wno-deprecated-declarations
	INCLUDES:= -I./src -I ./external/imgui
	LDFLAGS:= -03 -lsfml-graphics -lsfml-window -lsfl-system -lsfml-audio -lGL
endif

SRC_FILES:= $(wildcard src/*.cpp external/imgui/*.cpp)
OBJ_FILES:= $(SRC_FILES:.cpp=.o)

DEP_FILES:= $(SRC_FILES:.cpp=.d)
-include $(DEP_FILES)

all: $(OUTPUT)

$(OUTPUT): $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

.cpp.o:
	$(CXX) -MMD -MP -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f $(OBJ_FILES) $(DEP_FILES) ./bin/$(OUTPUT)

run: $(OUTPUT)
	cd bin && ./$(OUTPUT) && cd ..
