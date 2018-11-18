// jeff@gritch.org
// $Id:$

/*
 * Talk to a pololu 8-servo serial controller.
 */

#include "libpololu.h"
/*
 * Supply serial port, azimuth and elevation on the command line.
 * Note that az/el are not degrees, they're percentage of travel.
 * Speed is a value between 1 and 127.
 */
int main(int argc, char *argv[]) {
  int fd,i;
  float speed,tmp;
  float positions[SERVOS];

  if((argc<4)||(argc>10)) {
    fprintf(stderr, "Usage:  %s [serial_device] [speed] [servo0]\n",argv[0]);
    exit(0);
  }

  sscanf(argv[2],"%f",&speed);

  if((speed<1)||(speed>127)) {
    fprintf(stderr, "Speed must be between 1 and 127\n");
    exit(2);
  }

  for(i=0;i<SERVOS;i++) positions[i]=-1;

  for(i=0;i<argc-3;i++) {
    sscanf(argv[i+3],"%f",&tmp);
    positions[i]=tmp;
    //printf("Servo %d = %f\n",i,tmp);
    if((positions[i]<0.0)||(positions[i]>100.0)) {
      fprintf(stderr, "Servo position must be between 0.0 and 100.0\n");
      exit(1);
    }
  }

  if((fd=open_serial(argv[1])) < 0) {
    fprintf(stderr, "Error opening serial port\n");
    exit(3);
  }

  /*
   * Set each servo's speed.
   */
  for(i=0;i<SERVOS;i++) {
    if(positions[i]>=0.0) {
      if(set_servo_speed(fd,i,(int)speed)) {
	fprintf(stderr, "Error sending data\n");
	close(fd);
	exit(2);
      }
      set_servo_position(fd,i,positions[i]);
    }
  }

  close(fd);
  exit(0);
}
