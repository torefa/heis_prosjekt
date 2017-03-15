// The elevator state machine

#include "elev_state_machine.h"

static void drive(int current_floor, int motor_dir);

typedef enum {
		S_IDLE,
		S_MOVING,
		S_AT_FLOOR,
		S_STOPBUTTON,
		S_STOPBUTTON_AT_FLOOR,	//Har fjernet tilstanden fra koden
		S_TIMEOUT 				//Trengs nok ikke allikevel
} ELState;

static ELState el_state = S_IDLE;

static int current_floor;
static int motor_dir = DIRN_UP;

#define OFF  0;
#define ON 1;




void evInitialize(){
	// Initialize hardware
	printf("Initialiserer\n");
    if (!elev_init()) {
        printf("Klarte ikke å iniatilisere hardware\n");
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
	printf("el_state = S_IDLE\n");
	printf("Initialisering vellykket\n");
}

void evFloor_reached(int floor){
	printf("evFloor_reached: %d, motor dir = %d\n", floor, motor_dir);
	elev_set_floor_indicator(floor);
	current_floor = floor;
	printf("Er det en bestilling her? %d\n", queue_check_floor(current_floor, motor_dir));

	if (queue_check_floor(current_floor, motor_dir) == TRUE){
		elev_set_motor_direction(DIRN_STOP);
		timer_start();
		elev_set_door_open_lamp(ON);
		printf("Dør åpen\n");
		
		//Turn off button lamps for the floor
		elev_turn_off_button_lamp(current_floor);
			
		//Set state
		el_state = S_AT_FLOOR;
		printf("el_state = AT_FLOOR\n");
		
		//Remove floor from queue.
		queue_delete_floor(current_floor);
			
		//Tror denne følgende del av koden gjør at heisen går inn i S_IDLE før evTime_out, 
		// og at heisen dermed kjører med åpen dør.
		
		// if(!queue_get_queue(floor, motor_dir) && !queue_get_queue(floor, -motor_dir)){ 
		//	el_state = S_IDLE;
		//	printf("el_state = S_IDLE\n");
		}
	}	else if (floor == FIRST || floor == FOURTH){
			drive(current_floor, motor_dir); //Stops the elevator if the queue is empty when reaching 1st ot 4th floor
	}
}


void evButton_pressed(elev_button_type_t button, int floor){
	printf("evButton_pressed\n");
	
	//Check button type and set the relevant lamp.
	//Trengs egentlig if-statementen? BLir jo sjekket før funksjonskallet. Og for-løkka tillater ikke floor = -1
	if(floor != -1 && !(floor == FIRST && button == BUTTON_CALL_DOWN) && !(floor == FOURTH	&& button == BUTTON_CALL_UP)){
		elev_set_button_lamp(button, floor, ON);
	}
	
	
	//Check button type and set the relevant queue.
	if (button == BUTTON_CALL_UP && floor != FOURTH){	//Kan fc ta inn feil verdier?
		queue_set_up_queue(floor);
	} else if (button == BUTTON_CALL_DOWN && floor != FIRST){ //Kan fc ta inn feil verdier?
		queue_set_down_queue(floor);		
	} else if (button == BUTTON_COMMAND){		
		queue_set_queue(floor, current_floor);
	}
	
	//Start elevator in direction of button order if not already moving.
	switch(el_state){ //Holder kanskje med en if?
	case S_IDLE:
		printf("CASE: IDLE\n");
		if (floor - current_floor > 0){
			printf("Opp\n");
			queue_set_up_queue(floor);	//Det finnes kanskje en bedre måte å løse dette problemet på?
			motor_dir = DIRN_UP;
			elev_set_motor_direction(motor_dir);
			el_state = S_MOVING;
			printf("el_state = S_MOVING;\n");

		} else if (floor - current_floor < 0){
			printf("Ned\n");
			queue_set_down_queue(floor);
			motor_dir = DIRN_DOWN;
			elev_set_motor_direction(motor_dir);
			el_state = S_MOVING;
			printf("el_state = S_MOVING;\n");

		} else{
			timer_start();
			elev_set_door_open_lamp(ON);
			printf("Dør åpen\n");
			
			//Turn off button lamps for the floor
			elev_turn_off_button_lamp(current_floor);
			
			//Set state
			el_state = S_AT_FLOOR;
			printf("el_state = S_AT_FLOOR\n");

			//Remove floor from queue.
			queue_delete_floor(current_floor);
		}
		break;
		
	case S_TIMEOUT:
		drive(current_floor, motor_dir);
		break;

	default:
		break;
	}
}


//When time is out, close door and start elevator if there are any orders.
void evTime_out(){
	el_state = S_TIMEOUT; //Tror ikke denne tilstanden egentlig trengs allikevel
	printf("el_state = S_TIMEOUT\n");
	printf("evTime_out\n");
	elev_set_door_open_lamp(OFF);
	printf("Dør lukket\n");
	drive(current_floor, motor_dir);
}


//Event for activated stop button
void evStop_button_signal(int stop_signal, int floor_signal){
	printf("evStop_button_signal. Stop_signal: %d, Floor_signal: %d\n", stop_signal, floor_signal);
		if (stop_signal == TRUE){
		// Stop motor, light stop button and erase queue.
			el_state = S_STOPBUTTON;
			printf("el_state = S_STOPBUTTON\n");
			elev_set_motor_direction(DIRN_STOP);
			elev_set_stop_lamp(ON);
			queue_delete_queue();
			elev_clear_all_button_lamps();
			if (floor_signal > -1){	// TRUE if elevator is at a floor. Opens door.
				elev_set_door_open_lamp(ON);
				printf("Dør åpen\n")
			}
		} else{
			elev_set_stop_lamp(OFF);
			elev_set_door_open_lamp(OFF);
			printf("Dør lukket\n");	//Ooops. Dette skrives ut også hvis døren var lukket til å begynne med...
			el_state = S_IDLE;
			printf("el_state = S_IDLE\n");
		}
}

static void drive(int current_floor, int temp_motor_dir){
	if (queue_get_queue(current_floor, temp_motor_dir) == TRUE){
			elev_set_motor_direction(motor_dir);			
			el_state = S_MOVING;
			printf("el_state = S_MOVING\n");
			
	} else if (queue_get_queue(current_floor, -temp_motor_dir) == TRUE){
			motor_dir = -temp_motor_dir;
			elev_set_motor_direction(motor_dir);
			el_state = S_MOVING;
			printf("el_state = S_MOVING\n");

	} else{
		el_state = S_IDLE;
		printf("el_state = S_IDLE\n");
	}
}


