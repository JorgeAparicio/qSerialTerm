ABOUT
=====

qSerialTerm is a Qt based serial port terminal emulator.

qSerialTerm can interpret the incoming serial data as:

+ Text
+ Plots
+ Images

qSerial can send data as:

+ Text
+ Frames

The frame mode is useful for servomotor control.

DEPENDENCIES
============

+ Qwt library
+ QtSerialPort API

For installation steps, check the wiki at:
https://github.com/JorgeAparicio/qSerialTerm/wiki

BUILDING
========
Maybe you'll have to compile qwt from sources.
I tested with qwt6 static lib compiled from sources on
Debian Jessie.

 $ qmake
 $ make

DEVELOPMENT
===========

For further development Qt Creator is recommended.

LICENSE
=======

The qSerialTerm code is released under the terms of the GNU General Public
License (GPL) version 3.

See COPYING.GPL3.txt for more details.
