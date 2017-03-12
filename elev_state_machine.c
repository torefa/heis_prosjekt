// The elevator state machine

#include "elev_state_machine.h"

static void drive(int current_floor, int motor_dir);

typedef enum {
		S_IDLE,
		S_MOVING,
		S_AT_FLOOR,
		S_STOPBUTTON,
		S_STOPBUTTON_AT_FLOOR,
		S_TIMEOUT
} ELState;



static ELState el_state = S_IDLE;

static int current_floor;
static int motor_dir = DIRN_UP;

static int FALSE = 0;
static int TRUE = 1;

static int OFF = 0;
static int ON = 1;




void evInitialize(){
	// Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
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

void evFloor_reached(int floor){
	printf("evFloor_reached: %d, motor dir = %d\n",floor,motor_dir);
	elev_set_floor_indicator(floor);
	current_floor = floor;
	printf("Er det en bestilling her? %d\n", queue_check_floor(current_floor, motor_dir));
	if (queue_check_floor(current_floor, motor_dir) == TRUE){
			elev_set_motor_direction(DIRN_STOP);
			timer_start();
			printf("HA!\n");
			elev_set_door_open_lamp(ON);
			printf("Nei?\n");
		
			//Turn off button lamps for the floor
			elev_turn_off_button_lamp(current_floor);
			
			//Set state
			el_state = S_AT_FLOOR;
		
			//erase floor from queue.
			queue_delete_floor(current_floor);
			
			if(!queue_get_queue(floor, motor_dir) && !queue_get_queue(floor, -motor_dir)){ 	//Trengs denne?
				el_state = S_IDLE;															//Men hvis vi skal ha den, burde vi ha true og false
			}
	}	
	
	
	
/*	
	// Change direction when we reach top/bottom floor
    if (floor == N_FLOORS - 1 ) {
        elev_set_motor_direction(DIRN_DOWN);
		motor_dir = DIRN_DOWN;
    }
	else if (floor == 0) {
        elev_set_motor_direction(DIRN_UP);
		motor_dir = DIRN_UP;
    }
*/
}


void evButton_pressed(elev_button_type_t button, int floor){
	printf("evButton_pressed\n");
	
	//Check button type and set the relevant lamp.
	//Trengs egentlig if-statementen? BLir jo sjekket før funksjonskallet. Og for-løkka tillater ikke floor = -1
	if(floor != -1 && !(floor == 0 && button == BUTTON_CALL_DOWN) && !(floor == 3 && button == BUTTON_CALL_UP)){
		elev_set_button_lamp(button, floor, ON);
	}
	
	
	//Check button type and set the relevant queue.
	if (button == BUTTON_CALL_UP && floor != 3){	//Kan fc ta inn dette?
		queue_set_up_queue(floor);
		printf("Satt opp-queue!\n");
	} else if (button == BUTTON_CALL_DOWN && floor != 0){ //Kan fc ta inn dette?
		queue_set_down_queue(floor);
		printf("Satt ned-queue!\n");		
	} else if (button == BUTTON_COMMAND){		
		queue_set_queue(floor, current_floor);
		printf("Satt begge queues!\n");
	}
	
	//Start elevator in direction of button order if not already moving.
	switch(el_state){		
	case S_IDLE:
		printf("CASE: IDLE\n");
		if (floor - current_floor > 0){
		printf("Opp\n");
			elev_set_motor_direction(DIRN_UP);
				motor_dir = DIRN_UP;

		} else if (floor - current_floor < 0){
		printf("Ned\n");
			elev_set_motor_direction(DIRN_DOWN);
				motor_dir = DIRN_DOWN;
		} else{
			timer_start();
			printf("HA!\n");
			elev_set_door_open_lamp(ON);
			printf("Nei?\n");
		
			//Turn off button lamps for the floor
			elev_turn_off_button_lamp(current_floor);
			
			//Set state
			el_state = S_AT_FLOOR;
		
			//erase floor from queue.
			queue_delete_floor(current_floor);
		}
		break;
		
	case S_TIMEOUT:
		drive(current_floor, motor_dir);
		
	default:
		break;
	}
}


//When time is out, close door and start elevator if there is any orders.
void evTime_out(){
	el_state = S_TIMEOUT;
	printf("evTime_out\n");
	elev_set_door_open_lamp(OFF);
	drive(current_floor, motor_dir);
}


//Event for activated stop button
void evStop_button_signal(int signal){
	printf("evStop_button_signal: \n", signal);
		if (signal == TRUE){
		// Stop motor, light stop button and erase queue.
			elev_set_motor_direction(DIRN_STOP);
			elev_set_stop_lamp(ON);
			queue_delete_queue();
			elev_clear_all_button_lamps();
			if (el_state == S_AT_FLOOR){
				elev_set_door_open_lamp(ON);
				el_state = S_STOPBUTTON_AT_FLOOR;
			} else{		
					el_state = S_STOPBUTTON;
			}
	} else{
		elev_set_stop_lamp(OFF);
		
		if(el_state == S_STOPBUTTON_AT_FLOOR){
			el_state = S_IDLE;
			elev_set_door_open_lamp(OFF);
			
		} else if(el_state == S_STOPBUTTON){
			el_state = S_IDLE;
		}
		
	}
}

static void drive(int current_floor, int motor_dir){
	if (queue_get_queue(current_floor, motor_dir)){
			elev_set_motor_direction(motor_dir);			
			el_state = S_MOVING;
			
	} else if (queue_get_queue(current_floor, -motor_dir)){
			motor_dir = -motor_dir;
			elev_set_motor_direction(motor_dir);
			el_state = S_MOVING;
			
	} else{
		el_state = S_IDLE
	}
}


