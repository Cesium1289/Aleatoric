# Project Name: HW 4 Aleatoric
**Name: Carson Hansen**


**Description**\
 This program will take user input and plays a sequence of notes, most of which are randomly selected from a major scale.\
**How it went**\
This program for the most part went fairly smooth. It took a lot of time to properly get everything to work, but it was worth it.
I'm happy with how it turned out and I believe everything works properly.

**What needs to be done**\
Nothing more needs to be done to this program. Everything that was asked for should work!

**To build this program**\
To be able to build this program, you will need to install the following to get the program to run. Once these are installed, you will want to create the .exe by typing 'make'. Once the makefile has been created, you can run the program like a normal .exe and will need to pass the WAV as an argument to get an altered WAV file.

*Installs the sfml library that is used:*\
**sudo apt-get install libsfml-dev** 

*Installs the g++ compiler that is used to compile the program:*
**sudo apt-get install g++**

**How to use command line arguments**\
You can use any amount of the following command line arguments or none if you choose and default values will be used.\
***--root[x]*** This is the root tone of the scale, where x is a valid MIDI key from 21-115\
***--beats[x]*** Number of beats per measure, where x is a whole number greater than 0\
***--bpm[x]*** number of beats per minute, where x is a value greater than 0.0\
***--ramp[x]*** fraction of the betat time for the attack and release time for the note evelope, where x is between 0.0 - 0.5\
***--accent[x]*** volume of the first beat of each measure, where x is between 0.0 - 10.0\
***--volume[x]*** volume of the unaccented beats of each measure, where x is between 0.0-10.0\
\
**Example of how a command line arguments are used**\
./Aleatoirc --bpm[120] --volume[8] --root[64]






