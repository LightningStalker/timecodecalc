# timecodecalc
Initial release v1.1a (2/13/2009) by The Lightning Stalker (SourceForge)
8/25/2017 Create git repository

This will take a `CellTimes.txt` file and output an NTSC-ND timecode for each
"Cell Time" for programs such as Scenarist.

This program was brought about as a way of translating video time positions
between video editing and DVD authoring suites which tend to use one type
of timecode exclusively.  If you've authored your own DVDs with Scenarist
then you already know what a pain it can be to get the chapter positions
(scenes) in the right places.  It is hoped that through a great deal of
my frustration, this will relieve some of your frustration.

BTW, this is my first C program.  Not bad, eh?



## Build Instructions
	Linux:
	Simply execute `make` in the source directory

	Windows:
	Either use the precompiled `timecodecalc.exe` binary or install `mingw`
		and `msys` and build with the above Linux instructions.

## Install Instructions
	Linux:
	Copy the `timecodecalc` binary to `/usr/local/bin` or wherever you feel
		it should go.

	Windows:
	Copy `timecodecalc.exe` to your `Windows\System` folder.



Usage: `timecodecalc OPTIONS [INFILE] [OUTFILE]...`
Convert movie time position information from one format to another, as read
from INFILE, and send the results to OUTFILE, or standard output.

  -r, --framerate=FPS      frames per second, required

One, and only one, of the following is required:-

      --f2n                convert frame numbers to non-drop timecodes
      --frames2nondrop     ...
      --n2f                convert non-drop timecodes to frame numbers
      --nondrop2frames     ...

## Examples
  `timecodecalc -r30 --f2n CellTimes.txt`
      Read frame numbers from `CellTimes.txt` and output non-drop timecodes to
      standard output based on a 30 frames per second model. (NTSC)

  `timecodecalc -r25 --n2f SceneTimes.txt CellTimes.txt`
  		Read non-drop timecodes from a file named `CellTimes.txt`, and output
      frame numbers to a file named `SceneTimes.txt` based on a 25FPS model.
      (PAL)



### List of programs and the time code formats they are known to use

Program              Format(s) Used
-----------------------------------
Sonic Scenarist      NTSC-ND
IfoEdit              Frame Number
Mpeg2Schnitt         Frame Number
VideoReDo            NTSC-ND
VirtualDub           Multiple



## Version History
v1.1 - Implemented reverse conversion of non-drop timecodes to frame numbers
       Minor bug (feature) fix

v1.0 - Initial release
