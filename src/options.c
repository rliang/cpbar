/* See LICENSE file for copyright and license details. */

#include "options.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "utils.h"

#define DEFAULT_HEIGHT 16
#define DEFAULT_SIZES 0
#define DEFAULT_ON_BOTTOM false
#define DEFAULT_FONT "Sans-Serif Bold 9"
#define DEFAULT_FOREGROUND "#ffffff"
#define DEFAULT_BACKGROUND "#000000"

struct options options = {
	.sizes[0] = DEFAULT_SIZES,
	.sizes[1] = DEFAULT_SIZES,
	.sizes[2] = DEFAULT_SIZES,
	.bar_height = DEFAULT_HEIGHT,
	.bar_on_bottom = DEFAULT_ON_BOTTOM,
	.default_font = DEFAULT_FONT,
	.default_foreground = DEFAULT_FOREGROUND,
	.default_background = DEFAULT_BACKGROUND
};

static struct option long_options[] = {
	{ "help", no_argument, 0, 'h' },
	{ "left", required_argument, 0, 'l' },
	{ "right", required_argument, 0, 'r' },
	{ "center", required_argument, 0, 'c' },
	{ "height", required_argument, 0, 'g' },
	{ "bottom", no_argument, 0, 'm' },
	{ "font", required_argument, 0, 't' },
	{ "foreground", required_argument, 0, 'f' },
	{ "background", required_argument, 0, 'b' }
};

void options_init(int argc, char **argv)
{
	while (1) {
		int arg = getopt_long(argc, argv, "hu:l:r:c:mg:t:f:b:",
				long_options, NULL);
		if (arg == -1)
			break;
		switch (arg) {
		case 'l':
			options.sizes[0] = atoi(optarg);
			break;
		case 'r':
			options.sizes[1] = atoi(optarg);
			break;
		case 'c':
			options.sizes[2] = atoi(optarg);
			break;
		case 'g':
			options.bar_height = atoi(optarg);
			break;
		case 'm':
			options.bar_on_bottom = true;
			break;
		case 't':
			options.default_font = strdup(optarg);
			break;
		case 'f':
			options.default_foreground = strdup(optarg);
			break;
		case 'b':
			options.default_background = strdup(optarg);
			break;
		default:
			printf ("Usage: %s [-h] [-m] [-g HEIGHT] [-l LEFT] [-r RIGHT] [-c CENTER] [-t FONT] [-f FOREGROUND] [-b BACKGROUND]\n"
				"\t-h --help\t\t\tDisplay this help\n"
				"\t-m --bottom\t\t\tPlace bar on the bottom of the screen\n"
				"\t-g --height HEIGHT\t\tSet bar height to HEIGHT\n"
				"\t-l --left LEFT\t\t\tSet first LEFT identifiers to the left part of the bar\n"
				"\t-r --right RIGHT\t\tSet the following RIGHT identifiers to the right part of the bar\n"
				"\t-c --center CENTER\t\tSet the last CENTER identifiers to the center part of the bar\n"
				"\t-t --font FONT\t\t\tSet the default font to FONT\n"
				"\t-f --foreground FOREGROUND\tSet the default foreground color to FOREGROUND, in format #rrggbb\n"
				"\t-b --background BACKGROUND\tSet the default background color to BACKGROUND, in format #rrggbb\n"
				"\nIdentifiers are integers starting from 0, in the order Left->Right->Center, each of these sections being drawn left-to-right.\n"
				"Input must start with the identifier and a space. Example:\n"
				"\t%s -l 2 -r 3 -c 1\n"
				"\t0 my info\n"
				"\t3 info on right\n"
				"\t4 more on right\n"
				"\t5 center info\n"
				"\t0 updated info\n"
				"Result: [updated info][]\t\t[center info]\t\t[][info on right][more on right]\n"
				,argv[0], argv[0]); 
			exit(EXIT_SUCCESS);
		}
	}
}

void options_terminate()
{
	free(options.default_font);
	free(options.default_foreground);
	free(options.default_background);
}
