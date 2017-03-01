#include <stdio.h>
#include "elev.h"
#include "timer.h"
#include "elev_state_machine.h"
#include "queue.h"

void poll_buttons();
void poll_sensors();
void poll_timer();

int main() {
      // Initialize hardware
	evInitialize();
	
    while (1) {
		
		//poll_buttons();
		//poll_sensors();
		//poll_timer();
		
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
	if((elev_get_floor_sensor_signal != -1)){
		evFloor_reached(elev_get_floor_sensor_signal());
	}
}

void poll_timer(){
	if(timer_time_out()){
		evTime_out;
	}
}






