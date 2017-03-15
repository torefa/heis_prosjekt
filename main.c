#include <stdio.h>
#include "elev.h"
#include "timer.h"
#include "elev_state_machine.h"
#include "queue.h"
#include "definitions.h"

void poll_buttons();
void poll_stop_button();
void poll_sensors();
void poll_timer();

static int last_floor = 10;		// Initialized to not conflict with actual floors
static int last_button = 10;	// Initialized to not conflict with actual buttons
static int a = 10;				// Dårlig for lesbarhet, men nå funker polling av sensor

int main() {
      // Initialize hardware
	evInitialize();

	
    while (TRUE){
		
		poll_buttons();
		poll_stop_button();
		poll_sensors();
		poll_timer();
		
	}

    return FALSE;
}


void poll_buttons(){
	int floor;
	int button;

	for (floor = 0; floor < N_FLOORS; floor++){ 
		for(button = 0; button < N_BUTTONS; button++){
			if(!(floor == FIRST && button == BUTTON_CALL_DOWN) && !(floor == FOURTH && button == BUTTON_CALL_UP)){
				if((elev_get_button_signal(button, floor) == TRUE) && (button != last_button || floor != last_floor)){
					printf("%d %d\n", floor, button);								
					evButton_pressed((elev_button_type_t)button, floor);
					last_floor = floor;
					last_button = button;											
				}
			}
		}
	}
}

void poll_stop_button(){
		if(elev_get_stop_signal() && last_button != -1){
		evStop_button_signal(TRUE, elev_get_floor_sensor_signal());
		last_button = -1; 	// -1 is not used for other functionality
		} else if(!elev_get_stop_signal() && last_button == -1){
		evStop_button_signal(FALSE, elev_get_floor_sensor_signal());
		last_button = 10;	// Ensures that the the else if statement will not be true until a new button is pressed
		}
}

void poll_sensors(){
	
	int floor = elev_get_floor_sensor_signal();
	if(floor >= FIRST && floor < N_FLOORS && floor != a){
		evFloor_reached(floor);
		a = floor;
	}
}

void poll_timer(){
	if(timer_time_out()){
		evTime_out();
	}
}
