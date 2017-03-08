#include "timer.h"

#include <time.h>

// Indication of running timer
static int time_out = 0;
// Variable to indicate if the timer has been read or not
// Exists to not trig event evTimeout for each poll
static int time_read = 0;

float start_time;

void timer_start(){
	time_read = 0;
	time_out = 0;
	start_time = clock(); // start_time set to current time
}

int timer_time_out(){
	float delta_time = (float)(clock() - start_time) / CLOCKS_PER_SEC; //Time since start_time, converted to seconds.
	if(delta_time < 3){
		time_out = 0;
	}
	else if(delta_time > 3 && time_read == 0){ 
	// Returns true only if 3 seconds has elapsed, and time_read has been reset since last poll.
		time_out = 1;
		time_read = 1;
	}
	return time_out;
}
