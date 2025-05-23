#include <stdio.h>
#include <math.h>

#include "ip_connection.h"
#include "bricklet_io4_v2.h"
#include "mechanical_tester.h"

#define HOST "localhost"
#define PORT 4223
#define UID "27it" // Change XYZ to the UID of your IO-4 Bricklet 2.0"

#define BITSIZE 4

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

	printf("Channel 0: %s\n", value[0] ? "true" : "false");
	printf("Channel 1: %s\n", value[1] ? "true" : "false");
	printf("Channel 2: %s\n", value[2] ? "true" : "false");
	printf("Channel 3: %s\n", value[3] ? "true" : "false");

	bool button[BITSIZE];
	char cmd;
	while(cmd != 'q'){
		for(int i = 0; i<BITSIZE; i++){
			io4_v2_get_value(&io, value);
			button[i] = value[0];
			printf("%d", button[i]);
			printf(" ");
			Sleep(1000);
		}
		printf("\n%d\n", binaryToDecimal(button));
		printf("Shutting down\n");
		scanf(" %c", &cmd);
	}

	io4_v2_destroy(&io);
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
