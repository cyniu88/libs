CXX = g++
CXXFLAGS = -std=c++2a -Wall -Wextra -O2 -pthread
TARGET = threadpool
TEST_TARGET = test_threadpool
SOURCES = master.cpp ThreadPool.cpp
TEST_SOURCES = test_threadpool.cpp ThreadPool.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

$(TEST_TARGET): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SOURCES) -lgtest -lgtest_main

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all test clean