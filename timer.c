#include "timer.h"

#include <time.h>

// Indication of running timer
static int time_out = 0;
// Variable to indicate if the timer has been read or not
// Exists to not trig event evTimoout for each poll
static int time_read = 0;

/* 
	Function resets variables time_read and time_out to default values and sets start_time to current time.
*/
void timer_start(){
	time_read = 0;
	time_out = 0;
	float start_time = clock();
}

/*
	Function returns the timer status
	Function only returns 1 if the timer has been re-started and 3 seconds has elapsed since last poll.
*/ 
int timer_time_out(){
	float delta_time = (float)(clock() - start_time) / CLOCKS_PER_SEC; //Time since timer_start() converted to seconds.
	if(delta_time < 3){
		time_out = 0;
	}
	else if(delta_time > 3 && time_read == 0){
		time_out = 1:
		time_read = 1;
	}
	return timer_active;
}
