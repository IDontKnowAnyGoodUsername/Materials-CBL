#include "mechanical_tester.h"


bool silent_stepper_is_moving(SilentStepper *silent_stepper){
    int32_t remaining;
    uint16_t cur_vel;

    silent_stepper_get_remaining_steps(silent_stepper, &remaining);
    silent_stepper_get_current_velocity(silent_stepper, &cur_vel);

    return(remaining != 0 || cur_vel != 0);
}


void silent_stepper_target_info(SilentStepper *silent_stepper, int32_t target_pos, LoadCellV2 *lc) {
    int32_t current_pos = 0;
    int32_t necessary_steps;
    int32_t pos;
    int32_t ret_steps;
    uint16_t cur_vel;
    int32_t weight;
    
    silent_stepper_get_current_position(silent_stepper, &current_pos);
    necessary_steps = target_pos - current_pos;

    if(necessary_steps != 0){
        silent_stepper_set_steps(silent_stepper, necessary_steps);
    }

    while(silent_stepper_is_moving(silent_stepper)){
        silent_stepper_get_current_position(silent_stepper, &pos);
        silent_stepper_get_remaining_steps(silent_stepper, &ret_steps);
        silent_stepper_get_current_velocity(silent_stepper, &cur_vel);
        load_cell_v2_get_weight(lc, &weight);
        printf("\rRemaining steps:%d      Current velocity: %d      Current position: %d    Weight: %d mg", ret_steps, cur_vel, pos, weight - 8400000 + 24000);
        fflush(stdout);
        
        #ifdef _WIN32
        Sleep(1000);
        #else
        usleep(1000*1000);
        #endif
    }

    printf("\n");

}

void silent_stepper_zero_info(SilentStepper *silent_stepper, LoadCellV2 *lc){
    int32_t current_pos = 0;
    int32_t pos;
    int32_t ret_steps;
    uint16_t cur_vel;
    int32_t weight;

    silent_stepper_get_current_position(silent_stepper, &current_pos);
    
    silent_stepper_set_steps(silent_stepper, -current_pos);
    while(silent_stepper_is_moving(silent_stepper)) {
        
        silent_stepper_get_current_position(silent_stepper, &pos);
        silent_stepper_get_remaining_steps(silent_stepper, &ret_steps);
        silent_stepper_get_current_velocity(silent_stepper, &cur_vel);
        load_cell_v2_get_weight(lc, &weight);
        printf("\rRemaining steps:%d      Current velocity: %d      Current position: %d    Weight: %d mg", ret_steps, cur_vel, pos, weight - 8400000 + 28400);
        fflush(stdout);
        
        #ifdef _WIN32
        Sleep(1000);
        #else
        usleep(1000*1000);
        #endif
    }
    
    printf("\n");
    

}

//next step is to test and calibrate the load cell to accurately measure correct values for the 

/*int32_t load_cell_v2_calibrated_weight (LoadCellV2 *lc, int32_t weight){
    return cal_weight;
}*/