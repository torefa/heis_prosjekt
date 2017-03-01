#include <stdio.h>
#include "elev.h"
#include "timer.h"
#include "elev_state_machine.h"
#include "queue.h"

void poll_buttons();
void poll_sensors();


int main() {
      // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
	
    elev_set_motor_direction(motor_dir);
	while(1){
			if(elev_get_floor_sensor_signal() != -1){
			elev_stop_motor(& motor_dir);
			current_floor = elev_get_floor_sensor_signal();
			break;
		}
	}
	
    while (1) {
		
		poll_buttons();
		poll_sensors();
		
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
			motor_dir = DIRN_DOWN;
        }
		else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
			motor_dir = DIRN_UP;
        }


    }

	
    return 0;
}


void poll_buttons(){
	int floor;
	int button;
	for (floor = 0; floor < N_FLOORS; floor++){
		for(button = 0; button < 3; button++){
			if(elev_get_button_signal(button, floor)){
				evBbutton_pressed(button, floor, motor_dir, current_floor);
			}
		}
	}
	if(elev_get_stop_signal()){
		evStop_button_signal();
	}
}

void poll_sensors(){
	if((elev_get_floor_sensor_signal != -1) && (elev_get_floor_sensor_signal() != current_floor)){
		current_floor = elev_get_floor_sensor_signal();
		evFloor_reached(current_floor, motor_dir);
	}
}








