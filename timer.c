#include "timer.h"

// Variable to indicate if the timer has been read or not
// Exists to not trig event evTimeout for each poll
static int time_out = TRUE;



float start_time = 0;

void timer_start(){
	printf("timerstart\n");
	time_out = FALSE;
	start_time = clock(); // start_time set to current time
}

int timer_time_out(){
	float delta_time = 0;
	delta_time = (float)(clock() - start_time) / CLOCKS_PER_SEC; //Time since start_time, converted to seconds. 
	if (delta_time > 3 && time_out == FALSE) {
		time_out = TRUE;
		printf("Timeout\n");
		return TRUE;
	}
	return FALSE;
}
