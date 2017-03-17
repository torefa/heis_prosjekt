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
static int stop_button = FALSE;	// Variable to keep track of the last stop button configuration.

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

	for (floor = 0; floor < N_FLOORS; floor++){ 		// Check for all floors.
		for(button = 0; button < N_BUTTONS; button++){ 	// Check for all buttons.
			if(!(floor == FIRST && button == BUTTON_CALL_DOWN) && !(floor == FOURTH && button == BUTTON_CALL_UP)){	
					// Ensures only valid buttons are passed as function calls.
				if((elev_get_button_signal(button, floor) == TRUE)){							
					evButton_pressed((elev_button_type_t)button, floor);								
				}
			}
		}
	}
}

// Poll the status of the stop button.
void poll_stop_button(){
		if(elev_get_stop_signal() && stop_button != TRUE){ // The stop button is pressed.
			evStop_button_signal(TRUE, elev_get_floor_sensor_signal());
			stop_button = TRUE; 	// Ensures that evStop_button() is not called continously while the button is pressed.
			
		} else if(!elev_get_stop_signal() && stop_button == TRUE){ // The stop button is released.
			evStop_button_signal(FALSE, elev_get_floor_sensor_signal());
			stop_button = 10;
		}
}

// Poll for activated sensors.
void poll_sensors(){
	int floor = elev_get_floor_sensor_signal();
	if(floor >= FIRST && floor < N_FLOORS && floor != last_floor){ // Ensures that evFloor_reached() is called once per floor.
		evFloor_reached(floor);
		last_floor = floor;
	}
}

// Poll for timed out timer.
void poll_timer(){
	if(timer_time_out()){
		evTime_out();
	}
}
