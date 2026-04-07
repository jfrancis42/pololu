// jeff@gritch.org

/*
 * Command-line tool to control a Pololu 8-servo serial controller.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libpololu.h"

/*
 * Supply serial port, speed, and servo positions on the command line.
 * Positions are percentage of travel (0.0-100.0), not degrees.
 * Speed is 1 (fastest) to 127 (slowest).
 */
int main(int argc, char *argv[]) {
  int fd, i;
  int speed;
  float tmp;
  float positions[SERVOS];

  if ((argc < 4) || (argc > 10)) {
    fprintf(stderr, "Usage: %s <serial_device> <speed> <servo0> [servo1 ...]\n",
            argv[0]);
    fprintf(stderr, "  speed: 1-127 (1=fastest, 127=slowest)\n");
    fprintf(stderr, "  servoN: 0.0-100.0 (percent of travel)\n");
    exit(1);
  }

  speed = atoi(argv[2]);

  if ((speed < 1) || (speed > 127)) {
    fprintf(stderr, "Speed must be between 1 and 127\n");
    exit(1);
  }

  for (i = 0; i < SERVOS; i++) positions[i] = -1.0;

  for (i = 0; i < argc - 3; i++) {
    sscanf(argv[i + 3], "%f", &tmp);
    if ((tmp < 0.0) || (tmp > 100.0)) {
      fprintf(stderr, "Servo position must be between 0.0 and 100.0\n");
      exit(1);
    }
    positions[i] = tmp;
  }

  if ((fd = open_serial(argv[1])) < 0) {
    fprintf(stderr, "Error opening serial port\n");
    exit(1);
  }

  for (i = 0; i < SERVOS; i++) {
    if (positions[i] >= 0.0) {
      if (set_servo_speed(fd, i, speed) != 0) {
        fprintf(stderr, "Error setting servo %d speed\n", i);
        close(fd);
        exit(1);
      }
      if (set_servo_position(fd, i, (int)positions[i]) != 0) {
        fprintf(stderr, "Error setting servo %d position\n", i);
        close(fd);
        exit(1);
      }
    }
  }

  close(fd);
  return 0;
}
