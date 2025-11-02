CXX = clang++
CXXFLAGS = -std=c++20 -O3 -Wall -Wextra
LDFLAGS = -pthread -lsodium

BIN_DIR = bin
TEST_DIR = test
SRC_FILES = server.cpp client.cpp crypto.cpp

SERVER_MAIN = $(TEST_DIR)/main_server.cpp
CLIENT_MAIN = $(TEST_DIR)/main_client.cpp

SERVER_TARGET = $(BIN_DIR)/server
CLIENT_TARGET = $(BIN_DIR)/client

.PHONY: all clean

all: $(BIN_DIR) $(SERVER_TARGET) $(CLIENT_TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(SERVER_TARGET): $(SRC_FILES) $(SERVER_MAIN)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(CLIENT_TARGET): $(SRC_FILES) $(CLIENT_MAIN)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)