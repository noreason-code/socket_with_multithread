CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRC_DIR = src
BIN_DIR = bin

SENDER_SRC = $(SRC_DIR)/sender.cpp
RECEIVER_SRC = $(SRC_DIR)/receiver.cpp
RECEIVER_THREADPOOL_SRC = $(SRC_DIR)/receiver_threadpool.cpp

SENDER_BIN = $(BIN_DIR)/sender
RECEIVER_BIN = $(BIN_DIR)/receiver
RECEIVER_THREADPOOL_BIN = $(BIN_DIR)/receiver_threadpool

all: $(SENDER_BIN) $(RECEIVER_BIN) $(RECEIVER_THREADPOOL_BIN)

$(SENDER_BIN): $(SENDER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(RECEIVER_BIN): $(RECEIVER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(RECEIVER_THREADPOOL_BIN): $(RECEIVER_THREADPOOL_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< 

clean:
	rm -f $(BIN_DIR)/*

.PHONY: all clean
