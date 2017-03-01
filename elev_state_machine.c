// The elevator stat machine

#include "elev_state_machine.h"

#include <stdio.h>

#include "elev.h"
#include "timer.h"
#include "queue.h"

typedef enum {
		S_IDLE,
		S_MOVING,
		S_AT_FLOOR,
		S_STOPBUTTON,
		S_STOPBUTTON_AT_FLOOR
} ELState;

static ELState el_state = S_IDLE;

static int current_floor;
static int motor_dir = DIRN_UP;


void evInitialize(){
	// Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
    }
	
	//initialize elevator
    elev_set_motor_direction(motor_dir);
	while(1){
		if(elev_get_floor_sensor_signal() != -1){
			elev_stop_motor(motor_dir);
			current_floor = elev_get_floor_sensor_signal();
			break;
		}
	}
}

void evFloor_reached(int floor){
	elev_set_floor_indicator(floor);
	current_floor = floor;
	
	if (queue_get_queue(current_floor, motor_dir)){
		elev_stop_motor(motor_dir);
		timer_start();
		elev_set_door_open_lamp(1);
		
		//Turn off button lamps for the floor
		elev_turn_off_button_lamp(floor);
		
		//Set state
		el_state = S_AT_FLOOR;
		
		//erase floor from queue.
		queue_delete_floor(floor);
	}	
	
	    // Change direction when we reach top/bottom floor
    if (floor == N_FLOORS - 1) {
        elev_set_motor_direction(DIRN_DOWN);
		motor_dir = DIRN_DOWN;
    }
	else if (floor == 0) {
        elev_set_motor_direction(DIRN_UP);
		motor_dir = DIRN_UP;
    }
	
	
	
/*	    // Change direction when we reach top/bottom floor
    if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
        elev_set_motor_direction(DIRN_DOWN);
		motor_dir = DIRN_DOWN;
    }
	else if (elev_get_floor_sensor_signal() == 0) {
        elev_set_motor_direction(DIRN_UP);
		motor_dir = DIRN_UP;
    }
	*/
	
}


void evButton_pressed(elev_button_type_t button, int floor){
	elev_set_button_lamp(button, floor, 1);
	
	//Check button type and set the relevant queue
	if (button == BUTTON_CALL_UP){
		queue_set_up_queue(floor);
	} else if (button == BUTTON_CALL_DOWN){
		queue_set_down_queue(floor);
	} else if (button == BUTTON_COMMAND){
		queue_set_queue(floor, current_floor);
	}
	
	//Start elevator in direction of button order if not already moving.
	switch(el_state){
	case S_STOPBUTTON:
		break;
	case S_IDLE:
		break;
	case S_STOPBUTTON_AT_FLOOR:
		break;
	case S_MOVING:
		break;
	default:
		
		if (motor_dir > 0 && queue_get_queue(floor, motor_dir)){
			elev_set_motor_direction(DIRN_UP);
			motor_dir = DIRN_UP;
			el_state = S_MOVING;
		
		}else if (motor_dir < 0 && queue_get_queue(floor, motor_dir)){
			elev_set_motor_direction(DIRN_DOWN);
			motor_dir = DIRN_DOWN;
			el_state = S_MOVING;
		}
		break;
	}
}


//When time is out, close door and start elevator if there is any orders.
void evTime_out(){
	elev_set_door_open_lamp(0);
	if (motor_dir >0 && queue_get_queue(current_floor, motor_dir)){
		elev_set_motor_direction(DIRN_UP);
		motor_dir = DIRN_UP;
		el_state = S_MOVING;
		
	}else if (motor_dir < 0 && queue_get_queue(current_floor, motor_dir)){
		elev_set_motor_direction(DIRN_DOWN);
		motor_dir = DIRN_DOWN;
		el_state = S_MOVING;
	}
}


//Event for activated stop button
void evStop_button_signal(){
	
	switch(el_state){
	case S_STOPBUTTON:
		break;
	case S_IDLE:
		break;
	case S_STOPBUTTON_AT_FLOOR:
		break;
	case S_MOVING:
	// Stop motor, light stop button and erase queue.
		elev_stop_motor(motor_dir);
		elev_set_stop_lamp(1);
		queue_delete_queue();
		el_state = S_STOPBUTTON;
		break;
	case S_AT_FLOOR:
		// Make sure motorvis stopped, light stop button, open door and erase queue.
		elev_stop_motor(motor_dir);
		elev_set_stop_lamp(1);
		elev_set_door_open_lamp(1);
		queue_delete_queue();
		el_state = S_STOPBUTTON_AT_FLOOR;

	}
}




