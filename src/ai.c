#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h> // for sleep

#include "ip_connection.h"
#include "bricklet_io4_v2.h"

#define HOST "localhost"
#define PORT 4223
#define UID "27it" // Change XYZ to the UID of your IO-4 Bricklet 2.0"

#define MAX_LOG_ENTRIES 1000

uint32_t millis() {
    static LARGE_INTEGER frequency;
    static BOOL initialized = FALSE;
    LARGE_INTEGER now;

    if (!initialized) {
        QueryPerformanceFrequency(&frequency);
        initialized = TRUE;
    }
    QueryPerformanceCounter(&now);
    return (uint32_t)((now.QuadPart * 1000) / frequency.QuadPart);
}

typedef struct {
    uint32_t timestamp_ms;
    uint8_t channel;
    bool value;
} InputLogEntry;

InputLogEntry log_buffer[MAX_LOG_ENTRIES];
int log_index = 0;

void input_value_callback(uint8_t channel, bool changed, bool value, void *user_data) {
    if (channel != 0) return; // Skip channels 2 and 3
    if (log_index >= MAX_LOG_ENTRIES) return;

    InputLogEntry *entry = &log_buffer[log_index++];
    entry->timestamp_ms = millis();
    entry->channel = channel;
    entry->value = value;
}

int main() {
    IO4V2 io4;
    IPConnection ipcon;

    ipcon_create(&ipcon);
    io4_v2_create(&io4, "27it", &ipcon); // Replace "abc" with your actual UID

    if (ipcon_connect(&ipcon, "localhost", 4223) < 0) {
        printf("Connection failed\n");
        return 1;
    }

    // Register the input value callback
    io4_v2_register_callback(&io4, IO4_V2_CALLBACK_INPUT_VALUE,
                             (void (*)(void))input_value_callback, NULL);

    // Configure all 4 channels
    for (uint8_t channel = 0; channel < 1; channel++) {
        io4_v2_set_input_value_callback_configuration(&io4, channel, 0, true); // 0 ms period??
    }
    

    printf("Logging for 1 seconds...\n");

    // Use Sleep on Windows
    Sleep(1000); // 10 seconds in milliseconds

    // After logging, print out the values
    printf("\n--- Input Log ---\n");
    for (int i = 0; i < log_index; i++) {
        printf("Time: %ums | Channel: %u | Value: %u\n",
               log_buffer[i].timestamp_ms,
               log_buffer[i].channel,
               log_buffer[i].value);
    }

    // Clean up
    io4_v2_destroy(&io4);
    ipcon_destroy(&ipcon);

    return 0;
}
