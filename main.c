#include <stdio.h>
#include "elev.h"
#include "timer.h"
#include "elev_state_machine.h"
#include "queue.h"

void poll_buttons();
void poll_sensors();
void poll_timer();

static int lastFloor = 0;
static int lastButton = 0;

int main() {
      // Initialize hardware
	evInitialize();
	
    while (1) {
		
		poll_buttons();
		poll_sensors();
		poll_timer();
		
	}

    return 0;
}


void poll_buttons(){
	int floor;
	int button;
	// bør kanskje vaære av denne typen?
//	elev_button_type_t button;
	for (floor = 0; floor < N_FLOORS; floor++){ 
		for(button = 0; button < N_BUTTONS; button++){
			if(!(floor == 0 && button == BUTTON_CALL_DOWN) && !(floor == 3 && button == BUTTON_CALL_UP)){
				if(elev_get_button_signal(button, floor) && button != lastButton){
					printf("%d %d\n", floor, button);
					evButton_pressed((elev_button_type_t)button, floor);
				}
				
				lastButton = button;
			}
		}
	}
	if(elev_get_stop_signal()){
		evStop_button_signal(1);
	}else {
		evStop_button_signal(0);
	}
}

void poll_sensors(){
	
	int floor = elev_get_floor_sensor_signal();
	if(floor >= 0 && floor < N_FLOORS && floor != lastFloor){
		evFloor_reached(floor);
	}
	lastFloor = floor;
}

void poll_timer(){
	if(timer_time_out()){
		evTime_out();
	}
}






