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
	}

    return 0;
}


void poll_buttons(){
	int floor;
	int button;
	// bør kanskje vaære av denne typen?
//	elev_button_type_t button;
	for (floor = 0; floor < N_FLOORS; floor++){
		for(button = 0; button < 3; button++){
			if(!floor == button && elev_get_button_signal(button, floor)){
				evButton_pressed(button, floor);
			}
		}
	}
	if(elev_get_stop_signal()){
		evStop_button_signal();
	}
}

void poll_sensors(){
	if(elev_get_floor_sensor_signal() != -1){
		evFloor_reached(elev_get_floor_sensor_signal());
	}
}

void poll_timer(){
	if(timer_time_out()){
		evTime_out();
	}
}






