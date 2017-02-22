// The elevator stat machine

#include "elev_state_machine.h"

#include <sdtio.h>

#include "timer.h"
#include "queue.h"

typedef enum {
		S_IDLE,
		S_MOVING,
		S_AT_FLOOR,
		S_STOPBUTTON,
		S_STOPBUTTON_AT_FLOOR
} ELState

static ELState el_state = S_IDLE;

void evFloor_reached(){
	elev_set_floor_indicator(current_floor);
	
	if (queue[0] == current_floor){
		elev_stop_motor(current_dir);
		timer_start();
		elev_set_door_open_lamp(1);
		elev_set_button_lamp(active_button, current_floor, 0);
		
		el_state = S_AT_FLOOR;
		
		//erase floor from queue.
		set_queue(current_floor);
	}	
}


void evButton_pressed(){
	elev_set_button_lamp(active_button);
	set_queue(active_button_floor);
	switch(el_state)
	case S_IDLE
		
		break;
	case S_MOVING
		set_queue(active_button_floor);
		
		break;
	case S_AT_FLOOR
		set_queue(active_button_floor);
		
		el_state = S_MOVING;
		break;
}

void evTime_out(){
	elev_set_door_open_lamp(0);
	if (queue[0] - curretn_floor > 1){
		elev_Set_motor_direction(DIRN_UP);
	}
	
}


//Event for activated stop button
void evStop_button_signal(){
	
	switch(el_state)
	case S_MOVING
	// Stop motor, light stop button and erase queue.
		elev_stop_motor(current_dir);
		elev_set_stop_lamp(1);
		set_queue();
		el_state = S_STOPBUTTON;
		break;
	case S_AT_FLOOR
		// Stop motor, light stop button, open door and erase queue.
		elev_stop_motor(current_dir);
		elev_set_stop_lamp(1);
		elev_set_door_open_lamp(1);
		set_queue();
		el_state = S_STOPBUTTON_AT_FLOOR;
}




