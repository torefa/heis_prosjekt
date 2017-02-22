// Header for the queue file

/* 
Adds floor order to up or down queue based on elevators relative position
*/
void set_queue(int order, int pos);


/*
Adds floor order to up queue
*/
void set_up_queue(int order);

/*
Adds floor order to down queue
*/
void set_down_queue(int order);

/*
Deletes floor order from both queues
*/
void delete_floor(int floor);

/*
Deletes all floor orders from both queues
*/
void delete_queue(void);

/*
Checks all floors orders in the direction the elevator is/was moving, and returns true if there is a floor order.
If none of the floors in the given direction are orderd, the function returns -1.
*/
int get_queue(int floor, int current_dir);

/*
Chechs queue for given direction and floor. Returns true in there is an order for given floor in respective queue.
*/
int check_floor(int floor, int current_dir);