# socket_with_multithread
This is a socket communication tool implemented in C++, which includes senders and receivers.
The receiver uses thread to support simultaneous processing of connection requests from multiple senders.
The receiver executable file uses ordinary multithreading, and the receiver_threadpool executable file uses thread pool.

## How to use
```bash
# Compile 
make

# Start the receiver
./receiver_threadpool 127.0.0.1 1234

# Start the sender
./sender 127.0.0.1 1234 hello
```

