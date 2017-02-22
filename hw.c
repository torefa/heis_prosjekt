#include<stdlib.h>
#include "io.h"


int get_sensor_status(){
	return elev_get_floor_sensor_status();
}

int get_button_status(){
	
}

void set_floor_indicator(int current_floor){
	elev_set_floor_indicator(current_floor);
}

void set_button_lamp(int lamp){
	elev_set_door_open_lamp(int lamp);
	elev_set_stop_lamp(int lamp);
}

void run_motor(int motor_dir){
	elev_set_motor_direction(motor_dir);
}

