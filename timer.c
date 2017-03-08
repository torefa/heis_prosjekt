#include "timer.h"

#include <time.h>

// Indication of running timer
static  int timer_active = 0;

// 3 second timer
void timer_start(){
	timer_active = 1;
	float start_time = clock();
}

// Returns the status of the timer
int timer_time_out(){
	float delta_time = (float)(clock() - start_time) / CLOCKS_PER_SEC;
	if(delta_time < 3){
		timer_active = 1;
	}
	else{
		timer_active = 0:
	}
	return timer_active;
}


