#include "timer.h"

#include <time.h>

// Indication of running timer
static  int time_out = 0;

// 3 second timer
void timer_start(){
	time_out = 0;
	float start_time = clock();
}

// Returns the status of the timer
int timer_time_out(){
	float delta_time = (float)(clock() - start_time) / CLOCKS_PER_SEC;
	if(delta_time < 3){
		time_out = 0;
	}
	else{
		time_out = 1:
	}
	return timer_active;
}
