#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ip_connection.h"
#include "bricklet_io4_v2.h"

#define HOST "localhost"
#define PORT 4223
#define UID "27it" // Change XYZ to the UID of your IO-4 Bricklet 2.0"

void cb_input_value(uint8_t channel, bool changed, bool value, void *user_data) {
    // Filter to log only channel 0 or 1
    if ((channel == 0 || channel == 1) && changed) {
        printf("Channel %d changed to %s\n", channel, value ? "HIGH" : "LOW");
    }
}

int main(void) {
    IPConnection ipcon;
    IO4V2 io4_v2;
    const char *uid = "XYZ"; // Change XYZ to your device UID

    ipcon_create(&ipcon);
    io4_v2_create(&io4_v2, uid, &ipcon);

    if (ipcon_connect(&ipcon, "localhost", 4223) < 0) {
        fprintf(stderr, "Could not connect to Brick Daemon\n");
        return 1;
    }

    // Configure callback: enable periodic input monitoring for channels 0-3
    // (100 ms period, value changes trigger callback)
    io4_v2_set_input_value_callback_configuration(&io4_v2, 0, 100, true);
    io4_v2_set_input_value_callback_configuration(&io4_v2, 1, 100, true);

    // Register the callback function
    io4_v2_register_callback(&io4_v2,
                             IO4_V2_CALLBACK_INPUT_VALUE,
                             (void (*)(void))cb_input_value,
                             NULL);

    printf("Logging changes on IO0 and IO1. Press Enter to quit...\n");
    getchar();

    io4_v2_destroy(&io4_v2);
    ipcon_destroy(&ipcon); // Clean up

    return 0;
}