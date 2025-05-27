#include <stdio.h>
#include <math.h>

#include "ip_connection.h"
#include "bricklet_io4_v2.h"
#include "mechanical_tester.h"

#define HOST "localhost"
#define PORT 4223
#define UID "27it" // Change XYZ to the UID of your IO-4 Bricklet 2.0"

#define BITSIZE 4
#define MAX_LOG_ENTRIES 1000

typedef struct {
    uint32_t timestamp_ms;
    uint8_t channel;
    bool value;
} InputLogEntry;

InputLogEntry log_buffer[MAX_LOG_ENTRIES];
int log_index = 0;

uint32_t millis() {
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return (uint32_t)(spec.tv_sec * 1000 + spec.tv_nsec / 1000000);
}

void input_value_callback(uint8_t channel, bool changed, bool value, void *user_data) {
    if (log_index >= MAX_LOG_ENTRIES) return;

    InputLogEntry *entry = &log_buffer[log_index++];
    entry->timestamp_ms = millis();
    entry->channel = channel;
    entry->value = value;
}

int binaryToDecimal(bool values[4]){
	int result = 0;
	for(int i = BITSIZE; i>=0; i--){
		if(values[i]){
			result += pow(2, BITSIZE-1-i);
		}
	}
	return result;
}

int main(void) {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	IO4V2 io;
	io4_v2_create(&io, UID, &ipcon);

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		return 1;
	}
	// Don't use device before ipcon is connected

	// Get current value
	bool value[4];
	if(io4_v2_get_value(&io, value) < 0) {
		fprintf(stderr, "Could not get value, probably timeout\n");
		return 1;
	}

	io4_v2_register_callback(&io, IO4_V2_CALLBACK_INPUT_VALUE, (void(*)(void))input_value_callback, NULL);

	for(uint8_t channel = 0; channel <2; channel++){
		io4_v2_set_input_value_callback_configuration(&io, channel, 100, false);
	}

	printf("Logging for 10 seconds\n");
	Sleep(10000);

	printf("\n--- Input Log ---\n");
    for (int i = 0; i < log_index; i++) {
        printf("Time: %ums | Channel: %u | Value: %u\n",
               log_buffer[i].timestamp_ms,
               log_buffer[i].channel,
               log_buffer[i].value);
    }

	/*printf("Channel 0: %s\n", value[0] ? "true" : "false");
	printf("Channel 1: %s\n", value[1] ? "true" : "false");
	printf("Channel 2: %s\n", value[2] ? "true" : "false");
	printf("Channel 3: %s\n", value[3] ? "true" : "false");
	*/


	io4_v2_destroy(&io);
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
