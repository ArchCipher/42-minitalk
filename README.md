# <p align="center"> Minitalk - Inter-Process Communication via Signals </p>

**42 Project | Signal Programming & IPC**  
**Objective:** Create a communication program where a client and server exchange messages using only UNIX signals (`SIGUSR1` and `SIGUSR2`).

---

## Overview

**Minitalk** is a project that demonstrates inter-process communication (IPC) using only signals. The client sends messages to the server using binary signal encoding, and the server receives and reconstructs the messages. This project teaches signal handling, bit manipulation, and process synchronization.

---

## Skills Demonstrated

- **Signal Programming:** Using `SIGUSR1` and `SIGUSR2` for communication
- **Bit Manipulation:** Encoding/decoding data as binary signals
- **Process Synchronization:** Ensuring reliable message transmission
- **Signal Handling:** Proper signal handler implementation and blocking
- **Client-Server Architecture:** Two-process communication model
- **Error Handling:** Robust error handling for signal delivery failures

---

## How It Works

### Communication Protocol

**Binary Encoding:**
- `SIGUSR1` = bit `0`
- `SIGUSR2` = bit `1`

**Message Transmission:**
1. Client sends server PID to establish connection
2. Client sends message length (integer)
3. Client sends message character by character
4. Each character is sent bit by bit (8 bits per character)
5. Server receives signals and reconstructs the message

### Signal Flow

```
Client                          Server
  |                               |
  |-- SIGUSR1 (bit 0) ----------->|
  |<-- SIGUSR1 (ack) -------------|
  |-- SIGUSR2 (bit 1) ----------->|
  |<-- SIGUSR1 (ack) -------------|
  |          ...                  |
```

---

## Project Structure

```
minitalk/
├── server.c                 # Server implementation (receives signals)
├── client.c                 # Client implementation (sends signals)
├── utils.c                  # Utility functions
├── minitalk.h               # Header file with declarations
├── ft_printf/               # Printf library for output
└── Makefile                 # Build configuration
```

**Key Files:**
- **server.c** - Receives signals and reconstructs messages
- **client.c** - Sends messages encoded as binary signals
- **utils.c** - Helper functions for signal handling

---

## Program Structure

### Server (`server.c`)
- **Function:** Receives signals and reconstructs messages
- **Process:**
  1. Display server PID
  2. Wait for signals
  3. Decode binary signals to characters
  4. Print received messages
  5. Send acknowledgment after each bit

### Client (`client.c`)
- **Function:** Sends messages to server
- **Process:**
  1. Take server PID and message as arguments
  2. Send server PID
  3. Send message length
  4. Send message bit by bit
  5. Wait for acknowledgment after each bit

---

## Technical Implementation

### Bit Transmission

```c
// Client: Send character bit by bit
void send_char(int pid, char c)
{
    int i = 0;
    while (i < 8)
    {
        if (c & (1 << i))
            kill(pid, SIGUSR2);  // Send 1
        else
            kill(pid, SIGUSR1);  // Send 0
        i++;
        usleep(100);  // Wait for acknowledgment
    }
}
```

### Bit Reception

```c
// Server: Receive bit and reconstruct character
void handle_signal(int sig)
{
    static char c = 0;
    static int bit_count = 0;
    
    if (sig == SIGUSR2)
        c |= (1 << bit_count);  // Set bit to 1
    
    bit_count++;
    if (bit_count == 8)
    {
        write(1, &c, 1);  // Print character
        c = 0;
        bit_count = 0;
    }
    
    kill(client_pid, SIGUSR1);  // Send acknowledgment
}
```

---

## Features

### Mandatory Version
- ✅ Basic client-server communication
- ✅ Message transmission via signals
- ✅ Character-by-character reconstruction
- ✅ Server displays received messages

### Bonus Version
- ✅ Unicode support (handles multi-byte characters)
- ✅ Server acknowledgment system
- ✅ Client receives server confirmation
- ✅ Enhanced error handling
- ✅ Support for special characters and emojis

---

## Compilation & Usage

```bash
# Build server and client
make

# Build bonus version
make bonus

# Terminal 1: Start server
./server
# Output: Server PID: 12345

# Terminal 2: Send message from client
./client 12345 "Hello, World!"

# Server output:
# Hello, World!
```

**Example with Unicode (Bonus):**
```bash
./client 12345 "Hello 🌍 世界 🚀"
```

---

## Key Challenges & Solutions

### Challenge 1: Signal Delivery Reliability
**Problem:** Signals can be lost or delivered out of order  
**Solution:** Implement acknowledgment system - server confirms each bit received

### Challenge 2: Synchronization
**Problem:** Client and server must stay synchronized  
**Solution:** Client waits for acknowledgment before sending next bit, use `usleep()` for timing

### Challenge 3: Process ID Exchange
**Problem:** Client needs server PID to send signals  
**Solution:** Server prints PID on startup, client takes PID as command-line argument

### Challenge 4: Bit Manipulation
**Problem:** Encoding characters as binary signals  
**Solution:** Use bitwise operations (`&`, `|`, `<<`) to extract and set bits

### Challenge 5: Unicode Support (Bonus)
**Problem:** Handling multi-byte UTF-8 characters  
**Solution:** Send message length first, then send bytes (not characters) bit by bit

### Challenge 6: Signal Handler Limitations
**Problem:** Signal handlers have restrictions on what functions can be called  
**Solution:** Use `write()` instead of `printf()`, set global flags, process in main loop

---

## Technical Highlights

- **Signal Safety:** Proper signal handler implementation following async-signal-safe guidelines
- **Bit Manipulation:** Efficient encoding/decoding using bitwise operations
- **Synchronization:** Reliable message transmission with acknowledgment system
- **Error Handling:** Robust error checking for signal delivery failures
- **Unicode Support:** Bonus version handles multi-byte characters correctly

---

## Testing

The project can be tested with:
- **[Minitalk-Tester](https://github.com/suspectedoceano/minitalk/blob/main/command.py)** - Python command for testing
- Custom test scripts for different message types
- Stress testing with long messages
- Unicode character testing (bonus)

**Test Cases:**
- ✅ Short messages
- ✅ Long messages
- ✅ Special characters
- ✅ Unicode characters (bonus)
- ✅ Empty messages
- ✅ Multiple clients

---

## Project Status

✅ **Completed** - Both mandatory and bonus versions implemented

---

*This project demonstrates advanced signal programming, bit manipulation, and inter-process communication techniques in C.*
