#include "timer.h"

// Variable to indicate if the timer has been read or not.
// Exists to not trig evTimeout for each poll.
static int time_out = TRUE;

float start_time = 0;

// Start timer.
void timer_start(){
	time_out = FALSE;
	start_time = clock(); // start_time set to current time.
}

// Return TRUE(1) if timer has timed out.
int timer_time_out(){
	float delta_time = 0;
	delta_time = (float)(clock() - start_time) / CLOCKS_PER_SEC; //Time since start_time, converted to seconds. 
	if (delta_time > 3 && time_out == FALSE) {	// True after 3 sec.
		time_out = TRUE;
		return TRUE;
	}
	return FALSE;
}
