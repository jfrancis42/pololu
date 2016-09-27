// jeff@gritch.org
// $Id:$

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>

#define SERVOS 8

#define BAUDRATE B9600

int set_servo_position(int fd, int servo, int percent);
int set_servo_speed(int fd, int servo, int speed);
int open_serial(char* device);
