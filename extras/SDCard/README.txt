DueVGA SDCard
=============

To use DueVGA in conjunction with an SD card, you need to do the following:

1) Use VGA_COLOUR mode. VGA_MONO won't work, because it needs exclusive access
   to the SPI pins, which the SD card needs to use too.
   NTSC mode hasn't been tested yet. PAL works, but is comically slow :-)
   
2) Download and install the SdFat library SdFatBeta20130207
   http://code.google.com/p/beta-lib/downloads/detail?name=SdFatBeta20130207.zip&can=2&q=
   
3) You need to change a line of code in the SdFat library.
   In file Sd2Card.cpp on line 85:
   Change "#define USE_SAM3X_DMAC 1" to "#define USE_SAM3X_DMAC 0"
   
   
Anim demo
=========

To run this you will need an SD card adaptor connected to the SPI header (see the 
SdFat instructions for how to do this). Copy the file anim.dat to the root directory 
of the SD card. Then upload the sketch.

The Python script makeanim.py can be used to convert animated GIF files into an anim.dat
file suitable for the Anim demo. Not all animated GIFs will work properly due to a bug
in the Python Imaging Library.