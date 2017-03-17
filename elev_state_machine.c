// The elevator state machine.

#include "elev_state_machine.h"

static void drive(int current_floor, int motor_dir);

// Defining the different states.
typedef enum {
		S_IDLE,
		S_MOVING,
		S_AT_FLOOR,
		S_STOPBUTTON
} ELState;

// Initializing variables.
static ELState el_state = S_IDLE;

static int current_floor;
static int motor_dir = DIRN_UP;

#define OFF 0
#define ON 1


/*
Initializing hardware and elevator.
*/
void evInitialize(){
	// Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize hardware\n");
    }
	
	//initialize elevator
    elev_set_motor_direction(motor_dir);
	while(TRUE){
		if(elev_get_floor_sensor_signal() != -1){
			elev_set_motor_direction(DIRN_STOP);
			current_floor = elev_get_floor_sensor_signal();
			break;
		}
	}
	el_state = S_IDLE;
}




/*
	Event floor reached
Check queue for the floor. Stop elevator and start timer if floor is in the queue for the current direction.
*/
void evFloor_reached(int floor){
	elev_set_floor_indicator(floor);
	current_floor = floor;

	if (queue_check_floor(current_floor, motor_dir) == TRUE){ // Do if current floor is in the queue of the current direction.
		elev_set_motor_direction(DIRN_STOP);
		timer_start();
		elev_set_door_open_lamp(ON);
		elev_turn_off_button_lamp(current_floor);
		queue_delete_floor(current_floor);			

		el_state = S_AT_FLOOR;

	} else if (floor == FIRST || floor == FOURTH){
			drive(current_floor, motor_dir); //Stops the elevator if the queue is empty when reaching 1st ot 4th floor
	}
}

/*
	Event button is pressed
Updates the relevant queue. If the elevator is idle, it will start the motor in the direction the button indicates.
*/
void evButton_pressed(elev_button_type_t button, int floor){
	
	//Check button type and set the relevant lamp. Ensures only valid buttons are passed as function calls.
	if(floor != -1 && !(floor == FIRST && button == BUTTON_CALL_DOWN) && !(floor == FOURTH	&& button == BUTTON_CALL_UP)){
		elev_set_button_lamp(button, floor, ON);
	}
	
	
	//Check button type and set the relevant queue.
	if (button == BUTTON_CALL_UP && floor != FOURTH){ // Put order in UP queue.
		queue_set_up_queue(floor);
	} else if (button == BUTTON_CALL_DOWN && floor != FIRST){ // Put order in DOWN queue.
		queue_set_down_queue(floor);		
	} else if (button == BUTTON_COMMAND){	// Put order in queue for internal commands.	
		queue_set_queue(floor, current_floor);
	}
	
	//Start elevator in direction of button order if not already moving.
	switch(el_state){
	case S_IDLE:
		if (floor - current_floor > 0){
			queue_set_up_queue(floor);	
			motor_dir = DIRN_UP;
			elev_set_motor_direction(motor_dir);
			
			el_state = S_MOVING;

		} else if (floor - current_floor < 0){
			queue_set_down_queue(floor);
			motor_dir = DIRN_DOWN;
			elev_set_motor_direction(motor_dir);
			
			el_state = S_MOVING;

		} else{
			timer_start();
			elev_set_door_open_lamp(ON);
			elev_turn_off_button_lamp(current_floor);
			
			el_state = S_AT_FLOOR;

			//Remove floor from queue.
			queue_delete_floor(current_floor);
		}
		break;
		
	default:
		break;
	}
}

/*
	Event time is out
When time is out, close door and start elevator if there are any orders.
*/
void evTime_out(){
	elev_set_door_open_lamp(OFF);
	drive(current_floor, motor_dir);
}

/*
	Event for stop button
If the button is pressed: Stop motor, light stop button lamp and erase queue. Open door if the elevator is at a floor.
If the button is released: Turn off stop button lamp, close the door (regardless) and set the elevator to idle.
*/
void evStop_button_signal(int stop_signal, int floor_signal){
		if (stop_signal == TRUE){
			elev_set_motor_direction(DIRN_STOP);
			elev_set_stop_lamp(ON);
			queue_delete_queue();
			elev_clear_all_button_lamps();
			if (floor_signal > -1){	// TRUE if elevator is at a floor. Opens door.
				elev_set_door_open_lamp(ON);
			}
			
			el_state = S_STOPBUTTON;
		} else{
			elev_set_stop_lamp(OFF);
			elev_set_door_open_lamp(OFF);
			
			el_state = S_IDLE;
		}
}

// Function for checking the queues and running the elevator.
static void drive(int temp_floor, int temp_dir){
	if (queue_get_queue(temp_floor, temp_dir) == TRUE){ // Check if there are any orders in the queue for the current direction.
			elev_set_motor_direction(motor_dir);	
					
			el_state = S_MOVING;
	} else if (queue_get_queue(temp_floor, -temp_dir) == TRUE){ // Check if there are any orders in the opposite queue.
			motor_dir = -temp_dir;
			elev_set_motor_direction(motor_dir);
			
			el_state = S_MOVING;
	} else{
		
		el_state = S_IDLE;
	}
}
