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

static int last_floor = 10;		// Initialized to not conflict with actual floors.
static int last_button = 10;	// Initialized to not conflict with actual buttons.
static int active_floor = 10;	// Initialized to not conflict with actual floors.

int main() {
      // Initialize hardware.
	evInitialize();

	// Run all polling functions.
    while (TRUE){
		
		poll_buttons();
		poll_stop_button();
		poll_sensors();
		poll_timer();
		
	}

    return FALSE;
}

// Poll for activated buttons.
void poll_buttons(){
	int floor;
	int button;

	for (floor = 0; floor < N_FLOORS; floor++){ 	// Check for all floors.
		for(button = 0; button < N_BUTTONS; button++){ 	// Check for all buttons.
			if(!(floor == FIRST && button == BUTTON_CALL_DOWN) && !(floor == FOURTH && button == BUTTON_CALL_UP)){
				if((elev_get_button_signal(button, floor) == TRUE) && (button != last_button || floor != last_floor)){							
					evButton_pressed((elev_button_type_t)button, floor);
					last_floor = floor;
					last_button = button;											
				}
			}
		}
	}
}

// Poll for activated stop button.
void poll_stop_button(){
		if(elev_get_stop_signal() && last_button != -1){
			evStop_button_signal(TRUE, elev_get_floor_sensor_signal());
			last_button = -1; 	// -1 is not used for other functionality
		} else if(!elev_get_stop_signal() && last_button == -1){
			evStop_button_signal(FALSE, elev_get_floor_sensor_signal());
			last_button = 10;	// Ensures that the the else if statement will not be true until a new button is pressed
		}
}

// Poll for activated sensors.
void poll_sensors(){
	
	int floor = elev_get_floor_sensor_signal();
	if(floor >= FIRST && floor < N_FLOORS && floor != active_floor){
		evFloor_reached(floor);
		active_floor = floor;
	}
}

// Poll for timed out timer.
void poll_timer(){
	if(timer_time_out()){
		evTime_out();
	}
}
