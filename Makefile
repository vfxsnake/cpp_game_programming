# this make file is meant to be used with x64 native tools command prompt.
# the compiler uses cl.exe targeting the x64 architecture.
# for compiling and running at once type
# make run
# for cleaning:
# make clean

# define which compiler to use
OUTPUT := sfmlgame
OS := $(shell uname)
BIN_DIR := ./bin

CXX := cl
CXX_FLAGS := /O2 /EHsc /std:c++20 /W3 /D "WIN64"
INCLUDES := /I "./src" /I "./external/imgui" /I "./external/imgui-sfml" /I "C:/DEV/SFML_2.6.1/include"
LDFLAGS := /link /LIBPATH:"C:/DEV/SFML_2.6.1/lib" sfml-graphics.lib sfml-window.lib sfml-system.lib sfml-audio.lib opengl32.lib gdi32.lib winmm.lib

# Define source and object files
SRC_FILES := $(wildcard src/*.cpp external/imgui/*.cpp external/imgui-sfml/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

# Generate dependency files
DEP_FILES := $(SRC_FILES:.cpp=.d)
-include $(DEP_FILES)

all: $(BIN_DIR)/$(OUTPUT).exe

# Ensure the bin directory exists
$(BIN_DIR):
	mkdir $(BIN_DIR)

# Link the output binary
$(BIN_DIR)/$(OUTPUT).exe: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(OBJ_FILES) $(LDFLAGS) /OUT:$@

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(INCLUDES) /c $< /Fo$@

# Clean build artifacts
clean:
	-@del /Q $(subst /,\,$(OBJ_FILES))
	-@del /Q $(subst /,\,$(DEP_FILES))
	-@del /Q $(subst /,\,$(BIN_DIR)/$(OUTPUT).exe)

# Run the compiled binary
run: $(BIN_DIR)/$(OUTPUT).exe
	cd bin && $(OUTPUT).exe && cd ..
