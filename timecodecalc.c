/*********
 *
 * timecodecalc - A video timecode converter (calculator)
 * Version: 1.1 (2/9/2008)
 * Author: The Lightning Stalker (en-US)
 * WWW: http://beam.to/lightningstalker
 *
 * In loving memory of the white hen.
 *
 *********/

/* This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

#include <math.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


char program_name[128];
FILE *infile, *outfile; //They have to be global for our functions to use them



/* Function for calculating a non-drop timecode from
 * a frame number, result to stdout */
void frames2ndrop ( int frames, int framerate )
{

	int ff, ss, mm, hh, seconds;
	seconds = frames / framerate;
	ff = fmod(frames, framerate);
	ss = fmod(seconds, 60);
	mm = fmod(seconds / 60, 60);
	hh = seconds / 3600;
	fprintf(outfile, "%02d:%02d:%02d:%02d\n", hh, mm, ss, ff);
	return;
}



/* Function for calculating a frame number from a
 * non-drop timecode, result to stdout */

void ndrop2frames ( char nondrop[11], int framerate )
{
	int ff, ss, mm, hh, frames;
	sscanf(nondrop, "%d:%d:%d:%d", &hh, &mm, &ss, &ff);  // Read the hours,
	                       // minutes, seconds, from the passed in timecode
	frames = ff;  // Put in the frames
	frames = frames + (ss * framerate);  // Put in seconds times our rate
	frames = frames + (mm * 60 * framerate);  // Minutes
	frames = frames + (hh * 3600 * framerate);  // Hours
	fprintf(outfile, "%d\n", frames);  // Output frame number to file
	return;
}



int usage (int status)
{
  if (status != EXIT_SUCCESS)
    fprintf (stderr, "Try `%s --help' for more information.\n", program_name);
  else
    {
      printf ("Usage: %s OPTIONS [INFILE] [OUTFILE]...\n", program_name);
      fputs ("Convert movie time position information from one format to another, as read\nfrom INFILE, and send the results to OUTFILE, or standard output.\n\n  -r, --framerate=FPS      frames per second, required\n\nOne, and only one, of the following is required:-\n\n      --f2n                convert frame numbers to non-drop timecodes\n      --frames2nondrop     ...\n      --n2f                convert non-drop timecodes to frame numbers\n      --nondrop2frames     ...\n", stdout);
      printf ("\nExamples:\n  %s -r30 --f2n CellTimes.txt\n      Read frame numbers from CellTimes.txt and output non-drop timecodes to\n      standard output based on a 30 frames per second model. (NTSC)\n\n  %s -r25 --n2f SceneTimes.txt CellTimes.txt\n      Read non-drop timecodes from a file named CellTimes.txt, and output\n      frame numbers to a file named SceneTimes.txt based on a 25FPS model.\n      (PAL)\n", program_name, program_name);
      puts("\nThe Lightning Stalker (http://beam.to/lightningstalker)");
    }
  exit (status);
}



int main(int argc, char **argv)
{
	/* The flollowing emulates basename to get the filename of timecodecalc
	 * in case someone renamed it. */

#ifdef NIX
   char *program_nameptr = strrchr(argv[0], '/'); //*nix
#endif
#ifdef WIN
   char *program_nameptr = strrchr(argv[0], '\\'); //windows
#endif
   program_nameptr++; //increment past the leading backslash
	strcpy (program_name, program_nameptr); //copy it into program_name
	program_name[strcspn (program_name, ".")] = '\0'; //replace . with null pointer

	/* Delcare and initialize framerate and mode - you'll see why later */
	int framerate = 0;
	int mode = 0;


	/* Get Command Line Paramaters */

	int c;

	while(1)
	{
		static struct option const long_options[] =
		{
			{"framerate", required_argument, 0, 'r'},
			{"frames2nondrop", no_argument, 0, 'f'},
			{"f2n", no_argument, 0, 'f'},
			{"nondrop2frames", no_argument, 0, 'n'},
			{"n2f", no_argument, 0, 'n'},
			{"help", no_argument, 0, 'h'},
			{"version", no_argument, 0, 'v'},
			{0, 0, 0, 0}
		};

		int option_index = 0;

		c = getopt_long(argc, argv, "r:", long_options, &option_index);

		/*Detect the end of the options.*/
		if (c == -1)
			break;

		switch(c)
		{
			case 'r':
				framerate = atoi(optarg); //atoi = string to int
				break;
			case 'f':
				mode = 1;
				break;
			case 'n':
				mode = 2;
				break;
			case 'h':
				usage(0);
				break;
			case '?':
				usage(1);
				break;
			default:
				printf ("?? getopt returned character code 0%o ??\n", c);
				break;
		}
	}

	/* If framerate or mode are still 0 then that means the user didn't
	 * specify them. */
	if (framerate == 0)
	{
		fprintf(stderr, "You must specify a framerate.  Try '%s --help' for more information.\n", program_name);
		exit(1);
	}
	if (mode == 0)
	{
		fprintf(stderr, "You must specify a mode.  Try '%s --help' for more information.\n", program_name);
		exit(1);
	}

	/* Set our file input options to their defaults, then read the command
	 * line to see if input and output files were specified, and if they
	 * were, put them into the variables infilename and outfilename using
	 * the "interesting" method. */

	char infilename[128], outfilename[128];

	if (optind < argc)
	{
		sprintf(infilename, "%s", argv[optind++]);
		if ((infile = fopen (infilename, "r")) == NULL)
		{
			fprintf(stderr, "\nCannot open input file: %s", infilename);
			exit(1);
		}
		if (optind < argc)
		{
			sprintf(outfilename, "%s", argv[optind++]);
			if ((outfile = fopen (outfilename, "w")) == NULL)
			{
				fprintf(stderr, "\nCannot open output file: %s", outfilename);
				exit(1);
			}
		}
		else
		{
			outfile = stdout;
		}

	}
	else
	{
		fprintf(stderr, "You must specify an input file.  Try '%s --help' for more information.\n", program_name);
		exit(1);
	}
	if (strcmp (infilename, outfilename) == 0)
	{
		fputs("Input and output files cannot be the same.\n", stderr);
		exit(1);
	}


	/* Here are the main loops for reading in a line, fixing the timecode,
	 * and spitting it out.  Note that the raw lines must be converted with
	 * atoi to make them into integers for the math operations. */

	int count = 0;
	int LINE_MAX = 32;
	char line[LINE_MAX];

	while (fgets(line, LINE_MAX, infile) != NULL) {


		switch(mode)
		{
			case 1:
				frames2ndrop(atoi (line), framerate);
				break;
			case 2:
				ndrop2frames(line, framerate);
				break;
		}

		count++;  //Count those calculations!

	}
	fprintf(stderr, "\n\n%i timecodes converted.\n", count);
exit(0);
}
