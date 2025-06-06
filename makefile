CC = gcc

CFLAGS = -Wall -D_WIN32 -I./headers -IC:/MinGW/include
LDFLAGS = -LC:/MinGW/lib -lws2_32 -ladvapi32

TARGET = main
SRCS = src/main.c headers/ip_connection.c headers/brick_silent_stepper.c headers/mechanical_tester.c headers/bricklet_load_cell_v2.c headers/bricklet_io4_v2.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
