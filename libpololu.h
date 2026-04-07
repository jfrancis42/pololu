// jeff@gritch.org

#ifndef LIBPOLOLU_H
#define LIBPOLOLU_H

#define SERVOS 8

int open_serial(char *device);
int set_servo_position(int fd, int servo, int percent);
int set_servo_speed(int fd, int servo, int speed);

#endif /* LIBPOLOLU_H */
