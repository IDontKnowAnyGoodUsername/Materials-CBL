    //#define IPCON_EXPOSE_MILLISLEEP

    #include <stdio.h>
    #include <stdbool.h>

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>

    #include "ip_connection.h"
    #include "brick_silent_stepper.h"
    #include "mechanical_tester.h"
    #include "bricklet_load_cell_v2.h"

    #define HOST "localhost"
    #define PORT 4223
    #define UID_stepper "5VGrE3" // Change XXYYZZ to the UID of your Silent Stepper Brick
    #define UID_load "29ia"



    int main(void){

        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            printf("WSAStartup failed: %d\n", WSAGetLastError());
            return -1;
        }
        // Create IP connection
        IPConnection ipcon;
        ipcon_create(&ipcon);

        // Create device object
        SilentStepper ss;
        silent_stepper_create(&ss, UID_stepper, &ipcon);

        LoadCellV2 lc;
        load_cell_v2_create(&lc, UID_load, &ipcon);

        // Connect to brickd
        if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
            fprintf(stderr, "Could not connect\n");
            return 1;
        }
        // Don't use device before ipcon is connected


        silent_stepper_set_motor_current(&ss, 800); // 800 mA
        silent_stepper_set_step_configuration(&ss, SILENT_STEPPER_STEP_RESOLUTION_8,
                                            true); // 1/x steps (interpolated). //Higher number => smaller steps. USE CAUTION
        silent_stepper_set_max_velocity(&ss, 5000); // Velocity 2000 steps/s

        // Slow acceleration (500 steps/s^2),
        // Fast deacceleration (5000 steps/s^2)
        silent_stepper_set_speed_ramping(&ss, 500, 5000);

        silent_stepper_enable(&ss); // Enable motor power
        


        //------------------------------------------------------------------
        
        char cmd;

        while(cmd != 'q'){
            printf("Command?\n");
            scanf(" %c", &cmd);

            switch(cmd){
                case 't':
                printf("Target?\n");
                int  target;
                scanf("%d", &target);
                silent_stepper_target_info(&ss, target, &lc);     //positive steps moves the arrowhead UP!
                break;
                case '0':
                silent_stepper_zero_info(&ss, &lc);
                break;
                case 'p':
                Sleep(1);
                int pos;
                silent_stepper_get_current_position(&ss, &pos);
                printf("%d\n", pos);
                break;
            }
        }

        printf("Shutting down");
        
        //--------------------------------------------------------

        // Stop motor before disabling motor power
        silent_stepper_stop(&ss); // Request motor stop
        silent_stepper_set_speed_ramping(&ss, 500,
                                        5000); // Fast deacceleration (5000 steps/s^2) for stopping
        Sleep(400); // Wait for motor to actually stop: max velocity (2000 steps/s) / decceleration (5000 steps/s^2) = 0.4 s
        silent_stepper_disable(&ss); // Disable motor power

        load_cell_v2_destroy(&lc);


        silent_stepper_destroy(&ss);
        ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally

        WSACleanup();
        return 0;
    }

