# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -Wextra -pthread -fsanitize=address

# Directories
SRC_DIR = Src
INC_DIR = Inc
BUILD_DIR = Build
BIN_DIR = Bin

# Target executable
TARGET = $(BIN_DIR)/process

# Find all src files in Src directory
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Convert src file paths to object file paths
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

#default rule
all: $(TARGET)

#Compile source files into object files
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Link object files to create the final executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Create necessary directoris if they dont exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# To run the executable
run:
	./$(TARGET)
