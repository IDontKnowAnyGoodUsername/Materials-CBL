#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "ip_connection.h"
#include "bricklet_io4_v2.h"

#define HOST "localhost"
#define PORT 4223
#define UID "27it"
#define MAX_SAMPLES 1000

uint8_t data_channel[4][MAX_SAMPLES];
uint64_t timestamps[MAX_SAMPLES];
int sample_index = 0;


void cb_capture_input(const uint8_t *data, uint16_t length, void *user_data) {
    (void)user_data;

    for (uint16_t i = 0; i < length; i++) {
        uint8_t d = data[i];

        if (sample_index + 2 > MAX_SAMPLES) {
            break;
        }

        // First sample (lower 4 bits)
        for (int ch = 0; ch < 4; ch++) {
            data_channel[ch][sample_index] = (d >> ch) & 1;
            data_channel[ch][sample_index] = (d >> (ch + 4)) & 1;
        }
        sample_index++;
    }
}


int main() {
    IPConnection ipcon;
    IO4V2 io4;

    ipcon_create(&ipcon);
    io4_v2_create(&io4, UID, &ipcon);

    if (ipcon_connect(&ipcon, HOST, PORT) < 0) {
        fprintf(stderr, "Connection failed\n");
        return 1;
    }

    // Register callback
    io4_v2_register_callback(&io4, IO4_V2_CALLBACK_CAPTURE_INPUT, (void (*)(void))cb_capture_input, NULL);

    // Start capture with 20 µs interval
    io4_v2_set_capture_input_callback_configuration(&io4, true, 20);

    // Wait for data (increase time to get more samples)
    Sleep(10); // 100 ms

    // Stop capture
    io4_v2_set_capture_input_callback_configuration(&io4, false, 20);

    printf("Captured %d samples\n", sample_index);
    printf("Sample\tTimestamp\tCH0\tCH1\n");

    for (int i = 0; i < sample_index; i++) {
    printf("%d\t%" PRIu64 "\t%d\t%d\n", 
        i, 
        timestamps[i], 
        data_channel[0][i], 
        data_channel[1][i]);
    }

    io4_v2_destroy(&io4);
    ipcon_destroy(&ipcon);
    return 0;
}
