CC = gcc
CFLAGS = -Wall -I./tinkerforge
LDFLAGS = -lws2_32
TARGET = main

SRCS = src/main.c tinkerforge/ip_connection.c tinkerforge/brick_silent_stepper.c 

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
