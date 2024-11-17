# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -g

# Source files
SRCS = client.c dispatcher.c server.c shared_memory.c semaphore.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable names
CLIENT = client
DISPATCHER = dispatcher
SERVER = server

# Default target
all: $(CLIENT) $(DISPATCHER) $(SERVER)

# Client build target
$(CLIENT): client.o shared_memory.o semaphore.o
	$(CC) $(CFLAGS) -o $(CLIENT) client.o shared_memory.o semaphore.o

# Dispatcher build target
$(DISPATCHER): dispatcher.o shared_memory.o semaphore.o
	$(CC) $(CFLAGS) -o $(DISPATCHER) dispatcher.o shared_memory.o semaphore.o

# Server build target
$(SERVER): server.o shared_memory.o semaphore.o
	$(CC) $(CFLAGS) -o $(SERVER) server.o shared_memory.o semaphore.o

# Generic build rule for C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executables
clean:
	rm -f $(OBJS) $(CLIENT) $(DISPATCHER) $(SERVER)

# Rebuild everything from scratch
rebuild: clean all
