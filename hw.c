#include<stdlib.h>
#include "io.h"
#include ""

int get_sensor_status(){
	
}

int get_button_status(){
	
}

void set_floor_status(nt button){
	
}

void set_button_lamp(int lamp){
	
}

void run_motor(int motor_dir){
	
}

void stop_motor(int current_dir){
	elev_set_motor_direction(- current_dir); 		// Reverses direction of the motor for 0.1 seconds
	delay(100);
	elev_set_motor_durection(0); 						// 
};