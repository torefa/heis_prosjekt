#include "timer.h"

#include <unistd.h>

// Indication of running timer
static  int timer_active;

// 3 second timer
void timer_start(){
	timer_active = 1;
	sleep(3);
	timer_active = 0;
}

// Returns the status of the timer
int time_out(){
	return timer_active;
}