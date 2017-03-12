#include "queue.h"

#include <stdio.h>

static int NO_ORDER = 0;
static int ORDER = 1;

static int up_queue[4] = {NO_ORDER};
static int down_queue[4] = {NO_ORDER};


void queue_set_queue(int order, int pos){
	if(order == pos){			//If order is for the same floor as elevators current position
		up_queue[order] = down_queue[order] = ORDER;
	} else if (order > pos){	//If order is over elevators current position
		up_queue[order] = ORDER;
	} else if (order < pos){	//If order is under elevators current position
		down_queue[order] = ORDER;
	}
	
}

void queue_set_up_queue(int order){
	up_queue[order] = ORDER;
}

void queue_set_down_queue(int order){
	down_queue[order] = ORDER;
}

void queue_delete_floor(int floor){
	up_queue[floor] = down_queue[floor] = NO_ORDER;
}

void queue_delete_queue(void){
	int i;
	for (i = 0; i < 4; i++){
		queue_delete_floor(i);
	}
}

int queue_check_floor(int floor, int motor_dir){
	if (motor_dir > 0){
		return up_queue[floor];
	} else if (motor_dir < 0){
		return down_queue[floor];

	}
	return NO_ORDER;
}

int queue_get_queue(int floor, int motor_dir){
	int i;
	for (i = floor; (i > -1) && (i < 4); i = i + motor_dir){	//Checks all floors in direction elevator is moving for orders
		printf("i = %d\n", i);
		if(queue_check_floor(i, motor_dir) == ORDER){
			return ORDER;
			break;
		}
	}
	return NO_ORDER;
}
