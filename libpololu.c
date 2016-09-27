// jeff@gritch.org
// $Id:$

#include "libpololu.h"

/* Serial port variables */
static struct termios oldtio,newtio;

/*
 * Set the servo position.  Requires file descriptor, servo number,
 * and the percent of travel.
 */
int set_servo_position(int fd, int servo, int percent) {
  int position;
  char buffer[6];

  /* Check for out-of-bounds position. */
  if((percent<0)||(percent>100)) {
    return(-1);
  }

  /* Check for out-of-bounds servo. */
  if((servo<0)||(servo>7)) {
    return(-2);
  }

  /*
   * Calculate the position the servo should be in.  The units are in
   * 1/2 microseconds.  Per the controller (and the generally
   * recognized industry standard), the minimum allowed period is
   * 250us (500 units) and the maximum is 2250us (5500 units).  50% is
   * 1250us (2500 units), which is a bit off from the normal value of
   * 1500us (3000 units) for most common servos.  This will almost
   * certainly need tweaking for your specific servos.  At some point,
   * I'll either add a .pololurc file where these values can be
   * specified, or allow for specifying on the command line.  For now,
   * you'll need to tweak the source.  You'll notice that with the
   * servos I'm using (Futaba FP-S148), 2250us is more than the servo
   * can handle.  2000us (4000 units) is about the actual limit.
   */
  position=500+((4000-500)*(percent/100.0));

  /* Construct the string of bytes to be sent. */
  buffer[0]=0x80; // start byte
  buffer[1]=0x01; // device ID
  buffer[2]=0x04; // command
  buffer[3]=servo; // which servo
  buffer[4]=(char)((position&0xff80)>>7);
  buffer[5]=(char)(position&0x007f);

  /* Write the data to the servo. */
  if(write(fd,buffer,6)!=6) {
    return(-3);
  }

  return(0);
}

/*
 * Set the servo speed.  Requires file descriptor, servo number,
 * and speed (0 == fastest, 127 == slowest).
 */
int set_servo_speed(int fd, int servo, int speed) {
  char buffer[5];

  /* Check for out-of-bounds position. */
  if((speed<0)||(speed>127)) {
    return(-1);
  }

  /* Check for out-of-bounds servo. */
  if((servo<0)||(servo>7)) {
    return(-2);
  }

  /* Construct the string of bytes to be sent. */
  buffer[0]=0x80; // start byte
  buffer[1]=0x01; // device ID
  buffer[2]=0x01; // command
  buffer[3]=servo; // which servo
  buffer[4]=speed;

  /* Write the data to the servo. */
  if(write(fd,buffer,5)!=5) {
    return(-3);
  }

  return(0);
}

/*
 * Open the serial port and initialize the controller.  I stole this
 * code from code I originally wrote for gpsd (in case you noticed
 * that it looks familiar).
 */
int open_serial(char* device) {
  int fd;

  /* 
   * Open serial device for reading and writing and not as controlling
   * tty.
   */
  if ((fd = open(device, O_RDWR|O_NOCTTY)) < 0) {
    fprintf(stderr, "Error opening serial port\n");
    return(-1);
  }

  /* Save current serial port settings for later. */
  if (tcgetattr(fd, &oldtio) != 0) {
    fprintf(stderr, "Error reading serial port settings\n");
    return(-2);
  }

  /* Clear struct for new port settings. */
  bzero(&newtio, sizeof(newtio));

  /* Make it raw. */
  (void)cfmakeraw(&newtio);

  /* Set speed. */
  (void)cfsetospeed(&newtio, BAUDRATE);

  /* Clear the modem line and activate the settings for the port. */
  (void)tcflush(fd,TCIFLUSH);

  if (tcsetattr(fd,TCSANOW,&newtio) != 0) {
    (void)fprintf(stderr, "Error configuring serial port\n");
    return(-3);
  }

  return(fd);
}
