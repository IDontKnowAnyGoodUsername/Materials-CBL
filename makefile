CC = gcc

CFLAGS = -Wall -D_WIN32 -I./headers -IC:/MinGW/include
LDFLAGS = -LC:/MinGW/lib -lws2_32 -ladvapi32

TARGET = main
SRCS = src/simple_load.c headers/ip_connection.c headers/brick_silent_stepper.c headers/mechanical_tester.c headers/bricklet_load_cell.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
