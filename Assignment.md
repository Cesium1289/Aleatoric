# HW 4 Aleatoric

In this assignment you will create a generator of randomized “music.” You will learn about the major scale, about beats and measures, and about the complexity of detail in music generation.

## Background
Aleatoric music (Links to an external site.) is a fancy name for music produced partly at random, but with a human providing the organizing principles.

One of the easiest kinds of aleatoric music to produce is to pick random notes from a scale. Doing just this leads to really boring barely-music. Breaking the notes up into measures with a fixed number of beats per measure (usually 3, 4, 6 or 8) will give a sense of rhythm and flow: the measure starts can be identified by accenting (strengthening somehow) the first beat of each measure. Making the first beat always be at the “root pitch” (starting pitch) of the chosen scale makes the output sound more like music.

## The Assignment
You will build a generator that plays a sequence of notes, most of which are randomly selected from a major scale.

### Note Pitches
Your program will continuously play random measures of music. Each measure will start with the root note of the major scale, and then continue with randomly-selected notes of the major scale for the remaining beats. These randomly-selected tones should be chosen from scale notes 2..8.

For example, if the user has selected a C[5] major scale (key 72) and four beats per measure, your notes for a measure might be

* C[5] (key 72)
* F[5] (key 77)
* C[6] (key 84)
* A[5] (key 81)
### Beat Time
Beats will be played at a tempo (time per beat) selected by the user. Tempo is normally specified in “beats per minute” (BPM) which is a bit of an annoying unit. Divide 60 by BPM to get seconds per beat.

### Measure Accents and Volume
The first beat of each measure will be accented in two ways.

The first beat will be a square wave tone, the rest of the beats will be sine wave tones.

The first beat may have a different volume than the remaining beats. The user can select the volume for the first beat and for the remaining beats.

As we have discussed many times, the volume of a note is logarithmic in the amplitude of that note. For this assignment, the user will be able to set a volume in the range 0 to 10, where 10 is full volume and 0 is 60dB down from that volume. (This means that the volume at 0 is not quite 0, but quiet enough to hardly hear. See this great TI Blog Post (Links to an external site.) for an interesting discussion on this topic.) The formula you want for a given amplitude  (0..1) given a volume knob setting  (0..10) is:\
A = 10^( -6(10 - v) /20)


For our example, with user volumes 5 and 8, the output will thus be\

* C[5] (key 72), square wave, A = 0.03162277660168379
* F[5] (key 77), sine wave, A = 0.251188643150958
* C[6] (key 84), sine wave, A = 0.251188643150958
* A[5] (key 81), sine wave, A = 0.251188643150958
### Note Envelope
If you don’t use some kind of envelope, played notes will click at the start and end. Use a trapezoidal attack-release envelope: ramp the envelope up from 0.0 to 1.0 over the attack period at the start of the note, and ramp back down from 1.0 to 0.0 over the release period at the end of the note. Make the attack and release times the same. Graphically, it looks like this:


Trapezoidal AR Envelope
Apply this envelope to the note by multiplying sample-by-sample.

### Pseudocode
In the end, your program should look roughly like this:

repeat until interrupted
    play root frequency for beat interval (square wave, accent volume, ramp)
    for remaining beats
        k ← random key from scale up 1 to 8 scale steps from root
        play frequency k for beat interval (sine wave, volume, ramp)

### Command
Your program should be named aleatoric — for example, aleatoric.py if you are writing Python, or aleatoric.cpp if you are writing C++.

Your program should accept any combination of five command-line arguments. The default value to use if the argument is not given is in square brackets. Default numbers without a decimal point indicate integer arguments; default numbers with a decimal point indicate floating-point arguments.

--root KEYNUMBER: Use MIDI key number KEYNUMBER as the root tone of the scale. [48]

--beats SIG: Use a “time signature” of SIG beats per measure. [8]

--bpm BPM: Use a beat frequency of BPM beats per minute. [90.0]

--ramp FRAC: Use FRAC as a fraction of the beat time for the attack and release time for the note envelope. [0.5]

--accent VOLUME: Use the given VOLUME from 0..10 as the note volume for the first (accent) beat of each measure. [5.0]

--volume VOLUME: Use the given VOLUME from 0..10 as the note volume for the unaccented beats of each measure. [8.0]

Keep the program textually quiet: no output should be printed during operation.
### Hints
* Remember the frequency formula: the frequency  for MIDI key number  is\
  f = 440 * 2^{(k - 69) / 12}
* You can check that the BPM for your program is what you asked for by tapping along with your program on this BPM website (Links to an external site.).
* Wikipedia provides this odd but fun formula (slightly modified by me) for the value  of a square wave with frequency  at time . ( is the “floor” of : the greatest integer less than or equal to .)\
y = 4[ft] - 2[2ft] + 1

*  For a sine wave, of course, the formula is just\
 y = sin(2 * pi * ft)

* Don’t be fooled if your volume controls don’t match the way you would expect. A square wave at a given amplitude sounds much louder than a sine wave at the same amplitude, for reasons we have discussed.

### Turn-in
Please submit your work here as a single ZIP archive. Your archive should contain:

The source code and build files for your aleatoric program. Please include anything you wrote: all source code, etc.

If you wrote in a compiled language, provide a build tool script that can build your program if needed. For C/C++ specifically, provide a working Makefile. For Rust, provide a working Cargo.toml. Java and Go are fine without tools.

Your program should run in the Linux Lab environment: if it will not, please carefully note in the README.md what needs to be done to make it run there. Remember that we may not have seen your unusual language or libraries.

A Markdown (Links to an external site.) file named README.md with your name near the top. It should start with the project name and your name. Then briefly describe what you did, how it went, and what is still to be done. The README.md should also include detailed build-and-run instructions.

Please do not include executables, WAV files other than the ones asked for, .git, those horrible MacOS resource fork files, binaries, etc.

tl;dr: Your submission should be a ZIP file containing the following

* ./README.md
* ./mycode.myprogramminglanguage\
and whatever build files and stuff are absolutely needed.
