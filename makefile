CC = gcc
SDK_VER = 10.0.26100.0
SDK_BASE = /c/Program\ Files\ \(x86\)/Windows\ Kits/10

CFLAGS = -Wall \
    -I./tinkerforge \
    -I$(SDK_BASE)/Include/$(SDK_VER)/um \
    -I$(SDK_BASE)/Include/$(SDK_VER)/shared

LDFLAGS = \
    -L$(SDK_BASE)/Lib/$(SDK_VER)/um/x64 \
    -lws2_32 -ladvapi32

TARGET = main

SRCS = src/main.c tinkerforge/ip_connection.c tinkerforge/brick_silent_stepper.c 

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
