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
	
	
	if (get_queue(current_floor, current_dir){
		elev_stop_motor(current_dir);
		timer_start();
		elev_set_door_open_lamp(1);
		
		//Turn off button lamps for the floor
		elev_turn_off_button_lamp(current_floor);
		
		
		el_state = S_AT_FLOOR;
		
		//erase floor from queue.
		delete_floor(current_floor);
	}	
}


void evButton_pressed(){
	elev_set_button_lamp(active_button, active_button_floor, 1);
	
	if (current_floor > active_button){
		set
	
	//tenger kanskje ikke switch her..
	switch(el_state){
	case S_IDLE
		
		break;
	case S_MOVING
		
		break;
	case S_AT_FLOOR
		if (current_dir >0 && get_queue(current_floor, current_dir)){
			elev_Set_motor_direction(DIRN_UP);
			el_state = S_MOVING;
		
		}else if (current_dir < 0 && get_queue(current_floor, current_dir)){
			elev_Set_motor_direction(DIRN_DOWN);
			el_state = S_MOVING;
		}
		break;
	}
}

void evTime_out(){
	elev_set_door_open_lamp(0);
	if (current_dir >0 && get_queue(current_floor, current_dir)){
		elev_Set_motor_direction(DIRN_UP);
		el_state = S_MOVING;
		
	}else if (current_dir < 0 && get_queue(current_floor, current_dir)){
		elev_Set_motor_direction(DIRN_DOWN);
		el_state = S_MOVING;
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




