// Header for the queue file

/* 
	Adds floor order to up or down queue based on elevators relative position
*/
void queue_set_queue(int order, int pos);


/*
	Adds floor order to up queue
*/
void queue_set_up_queue(int order);

/*
	Adds floor order to down queue
*/
void queue_set_down_queue(int order);

/*
	Deletes floor order from both queues
*/
void queue_delete_floor(int floor);

/*
	Deletes all floor orders from both queues
*/
void queue_delete_queue(void);

/*
	Checks all floors orders in the direction the elevator is/was moving, and returns true if there is a floor order in the given direction.
	If none of the floors in the given direction are orderd, the function returns false.
*/
int queue_get_queue(int floor, int motor_dir);

/*
	Chechs queue for given direction and floor. Returns true in there is an order for given floor in respective queue.
*/
int queue_check_floor(int floor, int motor_dir);
