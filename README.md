# pololu
C library to talk to a Pololu serial servo controller.

I originally wrote this in 2009 to control a homemade az/el rotator
made out of r/c airplane servos with a webcam on top. Along with some
crufty CGI scripts, it allowed a remote user to control the camera and
take pictures of my office.

While the az/el controller is long gone, I just ran across this code
and thought it might be useful to somebody somewhere.

While I haven't gotten 'round to documenting it yet (and let's be
honest, I might never get around to that), there's at least a simple
sample app called servo.c included that illustrates how to use the
library.

It's better than writing this yourself from scratch, right?

Update: It now builds a shared library so I can use it from Common
Lisp using CFFI (which doesn't change anything about the way you'd use
it, otherwise).

Here's how to do that:

```
CL-USER> (ql:quickload :cffi)
To load "cffi":
  Load 1 ASDF system:
    cffi
; Loading "cffi"
.
(:CFFI)
CL-USER> (in-package :cffi)
#<PACKAGE "CFFI">
CFFI> (define-foreign-library pololu (:unix "/tmp/pololu/libpololu.so"))
POLOLU
CFFI> (use-foreign-library pololu)
#<FOREIGN-LIBRARY POLOLU "libpololu.so">
CFFI> (defcfun "open_serial" :int (device :string))
OPEN-SERIAL
CFFI> (defcfun "set_servo_speed" :int (fd :int) (servo :int) (speed :int))
SET-SERVO-SPEED
CFFI> (defcfun "set_servo_position" :int (fd :int) (servo :int) (percent :int))
SET-SERVO-POSITION
CFFI> (defparameter *fd* (open-serial "/dev/ttyS0"))
*FD*
CFFI> (set-servo-speed *fd* 0 63)
0
CFFI> (set-servo-position *fd* 0 42)
0
CFFI> 
```
