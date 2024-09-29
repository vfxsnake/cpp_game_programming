# for compiling and running at once type
# make run

# define which compiler to use
CXX := g++
OUTPUT := sfmlgame
OS := $(shell uname)

# Linux compiler and linker flags
ifeq ($(OS), Linux)
	CXX_FLAGS := -O3 -std=c++20 -Wno-unused-result -Wno-deprecated-declarations
	INCLUDES := -I./src -I./external/imgui -I./external/imgui-sfml -I/usr/include/SFML
	LDFLAGS := -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL -lGLEW -ldl
endif

# Define source and object files
SRC_FILES := $(wildcard src/*.cpp external/imgui/*.cpp external/imgui-sfml/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

# Generate dependency files
DEP_FILES := $(SRC_FILES:.cpp=.d)
-include $(DEP_FILES)

all: $(OUTPUT)

# Link the output binary
$(OUTPUT): $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ_FILES) $(DEP_FILES) ./bin/$(OUTPUT)

# Run the compiled binary
run: $(OUTPUT)
	cd bin && ./$(OUTPUT) && cd ..
