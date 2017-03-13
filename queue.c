#include "queue.h"

#include <stdio.h>

static int NO_ORDER = 0;
static int ORDER = 1;

static int up_queue[4] = {0};
static int down_queue[4] = {0};


void queue_set_queue(int order, int pos){
	if(order == pos){			//If order is for the same floor as elevators current position
		up_queue[order] = down_queue[order] = ORDER;
		printf("Satt begge queues!\n");
	} else if (order > pos){	//If order is over elevators current position
		up_queue[order] = ORDER;
		printf("Satt opp-queue!\n");
	} else if (order < pos){	//If order is under elevators current position
		down_queue[order] = ORDER;
		printf("Satt ned-queue!\n");
	}
	printf("U: [%d, %d, %d, %d]\nD: [%d, %d, %d, %d]\n", up_queue[0], up_queue[1], up_queue[2], up_queue[3], down_queue[0], down_queue[1], down_queue[2], down_queue[3]);
}

void queue_set_up_queue(int order){
	printf("Satt opp-queue!\n");
	up_queue[order] = ORDER;
	printf("U: [%d, %d, %d, %d]\nD: [%d, %d, %d, %d]\n", up_queue[0], up_queue[1], up_queue[2], up_queue[3], down_queue[0], down_queue[1], down_queue[2], down_queue[3]);
}

void queue_set_down_queue(int order){
	printf("Satt down-queue!\n");
	down_queue[order] = ORDER;
	printf("U: [%d, %d, %d, %d]\nD: [%d, %d, %d, %d]\n", up_queue[0], up_queue[1], up_queue[2], up_queue[3], down_queue[0], down_queue[1], down_queue[2], down_queue[3]);
}

void queue_delete_floor(int floor){
	up_queue[floor] = down_queue[floor] = NO_ORDER;
}

void queue_delete_queue(void){
	int i;
	for (i = FIRST; i < 4; i++){
		queue_delete_floor(i);
	}
}

int queue_check_floor(int floor, int motor_dir){
	if (motor_dir == 1){
		return up_queue[floor];
	} else if (motor_dir == -1){
		return down_queue[floor];

	}
	return NO_ORDER;
}

int queue_get_queue(int floor, int motor_dir){
	int i;
	int temp;
	printf("U: [%d, %d, %d, %d]\nD: [%d, %d, %d, %d]\n", up_queue[0], up_queue[1], up_queue[2], up_queue[3], down_queue[0], down_queue[1], down_queue[2], down_queue[3]);
	for (i = floor; (i > -1) && (i < 4); i = i + motor_dir){	//Checks all floors in direction elevator is moving for orders
		//fprintf("i = %d\n", i);
		if(queue_check_floor(i, motor_dir) == ORDER){
			temp = ORDER;
			break;
		} else{
			temp = NO_ORDER;
		}
	}
	return temp;
}
