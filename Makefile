# Define the source and build directories
SRC_DIR := src
BUILD_DIR := build

# Find all C++ source files
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")

# Generate the list of object files by replacing 'src' with 'build' and '.cpp' with '.o'
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Generate the list of dependency files (.d)
DEPS := $(OBJS:.o=.d)

# A special phony target to build the project
.PHONY: all
	all: lit

# Main rule to link the executable
lit: $(OBJS)
	g++ $(OBJS) -o $@ -lz

# ---
## Rules for creating directories

# Get a list of all unique directories where object files will be stored
OBJ_DIRS := $(sort $(dir $(OBJS)))

# Define a phony rule to ensure Make always tries to create the directories
.PHONY: $(OBJ_DIRS)
$(OBJ_DIRS):
	mkdir -p $@

# ---
## Rules for compiling source files

# The pattern rule for compiling source files into object files
# The `|` makes `OBJ_DIRS` an order-only prerequisite, so the directories are created before compiling.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIRS)
	g++ -Wall -Wextra -std=c++20 -MMD -MP -c $< -o $@

# ---
## Rules for cleanup

# Phony target to clean up all generated files

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) lit

# Include the generated dependency files
-include $(DEPS)
