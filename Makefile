.PHONY: all clean setup run-demo experiments

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c11 -O2 -Iinclude -D_POSIX_C_SOURCE=200809L
LDFLAGS :=

BIN_DIR := bin
SRC_DIR := src
INC_DIR := include

TARGET  := $(BIN_DIR)/findcmp

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/*.h | $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(BIN_DIR)/*.o
	rm -f $(TARGET)

setup:
	bash scripts/generate_testing_tree.sh

run-demo: all
	@echo "=== BFS on wide_shallow ==="
	$(TARGET) --algo bfs --root testing/wide_shallow --target target.txt --out results/demo.csv
	@echo ""
	@echo "=== DFS on wide_shallow ==="
	$(TARGET) --algo dfs --root testing/wide_shallow --target target.txt --out results/demo.csv
	@echo ""
	@echo "=== BFS on deep_narrow ==="
	$(TARGET) --algo bfs --root testing/deep_narrow --target target.txt --out results/demo.csv
	@echo ""
	@echo "=== DFS on deep_narrow ==="
	$(TARGET) --algo dfs --root testing/deep_narrow --target target.txt --out results/demo.csv

experiments:
	bash scripts/run_all_experiments.sh
