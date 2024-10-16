# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRCS = server.c admin.c

# Header files
HDRS = admin.h

# Output executable
TARGET = server

# Default rule to compile the program
all: $(TARGET)

# Linking object files to create the final executable
$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean rule to remove the executable and object files
clean:
	rm -f $(TARGET)

# Rule to run the server
run: $(TARGET)
	./$(TARGET)
