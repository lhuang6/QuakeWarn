# PROJECT MAKE FILE
#
# ------------------
# Defining Variables
# ------------------

# Linux compiler
CC = clang

# Important flags to include when compiling
# Adding all and extra warnings
# Including include
CFLAGS = -Wall -Wextra -Iinclude -Ithird-party/civetweb -Ithird-party/cJSON

# Local libraries to include
LDLIBS = -lcurl

# New directory where all the compiled files go
BUILD_DIR = build

# Name of the compiled binary code
TARGET = quakewarn

# All source code
SRC = $(wildcard src/*.c) third-party/cJSON/cJSON.c

# All intermediate .o files (switching source code files ending to .o)
OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))

# Default condition
all: $(TARGET)

# Compiling intermediate object files into final binary
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDLIBS)

# Creating all object files
# Making all directories and intermediate directories (src) in build
# Compiling them into the build folder (-o $@)
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Ignoring any files named all or clean
.PHONY: all clean

# -------------
# Make Testfile
# -------------
TEST_TARGET = test_json
TEST_SRC = $(wildcard src/*.c) tests/test.c
TEST_OBJ = build/tests/test.o

$(TEST_TARGET): $(TEST_OBJ) build/third-party/cJSON/cJSON.o
	$(CC) $^ -o $@ $(LDLIBS)

build/tests/%.o: tests/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: test

# Cleaning project by deleting all binaries
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)
