CC:= g++
# CPPFLAGS:= -std=c++17 -g -Wall -Wextra -pedantic
CPPFLAGS:= -std=c++17 -pthread -g -Wall -Wextra -pedantic

PROJECT := render_engine.exe
SRC_DIR := ..\src
BUILD_DIR := ..\build
OBJ_DIR := ..\build\obj
EXT_DIR := ..\external
 
SRC_FILES := $(wildcard $(SRC_DIR)/**/**/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
 
LIBRAIRIES := -lvulkan-1 -lglfw3dll

INCLUDE :=-I$(VULKAN_SDK)\Include \
	-I$(VULKAN_SDK)\Third-Party\Include \
	-I$(EXT_DIR)\glfw\include \
	-I$(EXT_DIR)\glm \
	-I$(EXT_DIR)\stb \
	-I$(EXT_DIR)\tinyobjloader\include \
	# -I$(EXT_DIR)\assimp\include \
 
LIB :=-L$(VULKAN_SDK)\Lib \
	-L$(VULKAN_SDK)\Third-Party\Bin \
	-L$(EXT_DIR)\glfw\lib-vc2019 \
	# -L$(EXT_DIR)\assimp\lib
 
all: $(PROJECT)
 
$(PROJECT): $(OBJ_FILES)
	$(CC) -o $(BUILD_DIR)/$@ $^ $(LIB) $(LIBRAIRIES)
 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $< $(INCLUDE)