# pololu

C library for communicating with Pololu serial servo controllers (tested with
the SSC03A). Supports all 8 channels. Originally written in 2009 to drive a
homemade az/el rotator made from RC airplane servos with a webcam on top.

## Building

```sh
make
```

This produces:

- `libpololu.a` — static library
- `libpololu.so` / `libpololu.so.1` — shared library
- `servo` — command-line example

## Usage

### Command-line tool

```
./servo <serial_device> <speed> <servo0> [servo1 ...]
```

- `serial_device`: path to the serial port (e.g. `/dev/ttyUSB0`)
- `speed`: 1–127, where 1 is fastest and 127 is slowest
- `servoN`: position as percent of travel (0.0–100.0)

Example — move servo 0 to 50% at speed 63:

```sh
./servo /dev/ttyUSB0 63 50.0
```

### C API

```c
#include "libpololu.h"

int fd = open_serial("/dev/ttyUSB0");
set_servo_speed(fd, 0, 63);       // servo 0, medium speed
set_servo_position(fd, 0, 50);    // servo 0, center
close(fd);
```

All three functions return 0 on success and a negative value on error.

### Common Lisp (CFFI)

```lisp
(ql:quickload :cffi)
(in-package :cffi)

(define-foreign-library pololu (:unix "/path/to/libpololu.so"))
(use-foreign-library pololu)

(defcfun "open_serial"        :int (device :string))
(defcfun "set_servo_speed"    :int (fd :int) (servo :int) (speed :int))
(defcfun "set_servo_position" :int (fd :int) (servo :int) (percent :int))

(defparameter *fd* (open-serial "/dev/ttyUSB0"))
(set-servo-speed    *fd* 0 63)
(set-servo-position *fd* 0 42)
```

## Notes

Servo position timing is calculated as 500–4000 half-microsecond units (250us–2000us).
This range works well with Futaba FP-S148 servos; you may need to adjust the
constants in `libpololu.c` for other servos.

The hardware guide for the SSC03A is in `docs/ssc03a_guide.pdf`.
