SRC_DIR := ./src
OBJ_DIR := ./obj
#LIB_DIR := ./lib
INC_DIR := ./include
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
#LDFLAGS := -L $(LIB_DIR)
INCFLAGS := -I $(INC_DIR)
LDLIBS := -static -lpthread -mwindows
CXXFLAGS := -MMD -g -std=c++17 -static-libgcc -static-libstdc++

./build/YGO_LOTD_LE_YDC_Converter.exe: $(OBJ_FILES)
	g++ -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CXXFLAGS) $(INCFLAGS) -c -o $@ $<

-include $(OBJ_FILES:.o=.d)