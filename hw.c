#include<stdlib.h>
#include "io.h"


int get_sensor_status(){
	
}

int get_button_status(){
	
}

void set_floor_status(int button){
	
}

void set_button_lamp(int lamp){
	elev_set_door_open_lamp(int lamp);
	elev_set_stop_lamp();
}

void run_motor(int motor_dir){
	elev_set_motor_direction(motor_dir);
}

void stop_motor(int current_dir){
	elev_set_motor_direction(- current_dir); 		// Reverses direction of the motor for 0.1 seconds
	delay(100);
	elev_set_motor_durection(0); 						// Stops motor 
};