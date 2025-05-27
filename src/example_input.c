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

	for(int i = 0; i <100; i++){
		io4_v2_get_value(&io, value);
		printf("%d, %d\n", value[0],value[1]);
	}
	/*printf("Channel 0: %s\n", value[0] ? "true" : "false");
	printf("Channel 1: %s\n", value[1] ? "true" : "false");
	printf("Channel 2: %s\n", value[2] ? "true" : "false");
	printf("Channel 3: %s\n", value[3] ? "true" : "false");
	*/
	/*
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
		*/
	/*int pulse_counter = 0;
		for (int i = 0; i < 300; i++) {
    	int counter = 0;

    	// Wait for clock to go LOW
    	do {
        	io4_v2_get_value(&io, value);
        	Sleep(1);  // 0.1 ms = avoid CPU hammering
    	} while (value[1] == 1);
    	// Clock is LOW, measure how long it's LOW
    	do {
        	io4_v2_get_value(&io, value);
        	Sleep(1);  // 0.1 ms
        	counter++;
    	} while (value[1] == 0);

   	 // Clock went HIGH = rising edge = sample data
    	if (counter > 5) {
    	    printf("   %d pulses,(clock low for ~%d * 100us)\n", pulse_counter, counter);
			pulse_counter =0;
    	}

    	// Sample data at the moment clock goes high
    	printf("%d", value[0]);
	}*/
	bool string[300];
	for(int i = 0; i<300; i++){
		io4_v2_get_value(&io, value);
		string[i] = value[1];
	}
	for(int i = 0; i <300; i++){
		printf("%d", string[i]);
	}

	/*for(int i = 0; i <50; i++){
		int zero_time =0;
		//bool string[300];
		do{
			io4_v2_get_value(&io, value);
			while(value[1]==0){		//wait until high
				io4_v2_get_value(&io, value);
				zero_time++;
			}
			printf("%d", value[0]);				//sample at edge
			while(value[1] ==1){	//wait until low
				io4_v2_get_value(&io, value);
			}
		} while (zero_time<8);
		printf("\n");
	}*/

	io4_v2_destroy(&io);
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
