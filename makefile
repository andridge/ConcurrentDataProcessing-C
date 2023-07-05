CC = g++
CFLAGS = -std=c++11

APP_NAME = 0p3n53450n5
BUILD_DIR = ./bin

SRC_DIR = ./src
OBJ_DIR = $(BUILD_DIR)/obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

$(APP_NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(APP_NAME) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean
