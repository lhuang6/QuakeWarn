CC = clang
CFLAGS = -Wall -Wextra -Iinclude -Ithird-party/civetweb -Ithird-party/cJSON

BUILD_DIR = build
TARGET = quakewarn

SRC = $(wildcard src/*.c) third-party/cJSON/cJSON.c
OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean

TEST_TARGET = test_json
TEST_SRC = tests/test.c
TEST_OBJ = build/tests/test.o

$(TEST_TARGET): $(TEST_OBJ) build/third-party/cJSON/cJSON.o
	$(CC) $^ -o $@

build/tests/%.o: tests/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: test
