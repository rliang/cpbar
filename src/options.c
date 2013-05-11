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

struct options options;

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

static void set_defaults()
{
	options.sizes[0] = DEFAULT_SIZES;
	options.sizes[1] = DEFAULT_SIZES;
	options.sizes[2] = DEFAULT_SIZES;
	options.bar_height = DEFAULT_HEIGHT;
	options.bar_on_bottom = DEFAULT_ON_BOTTOM;
	options.default_font = strdup(DEFAULT_FONT);
	options.default_foreground = strdup(DEFAULT_FOREGROUND);
	options.default_background = strdup(DEFAULT_BACKGROUND);
}

void options_init(int argc, char **argv)
{
	set_defaults();
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
			printf("Usage: %s pbar [-h] [-m] [-g HEIGHT] [-l LEFT] [-r RIGHT] [-c CENTER] [-t FONT] [-f FOREGROUND] [-b BACKGROUND]\n"
				"\t-h --help\t\t\tDisplay this help\n"
				"\t-m --bottom\t\t\tPlace bar on the bottom of the screen. Default 16\n"
				"\t-g --height HEIGHT\t\tSet bar height to HEIGHT. Default 16\n"
				"\t-l --left LEFT\t\t\tSet first LEFT identifiers to the left part of the bar. Default 0\n"
				"\t-r --right RIGHT\t\tSet the following RIGHT identifiers to the right part of the bar. Default 0\n"
				"\t-c --center CENTER\t\tSet the last CENTER identifiers to the center part of the bar. Default 0\n"
				"\t-t --font FONT\t\t\tSet the default font to FONT. Default Sans-Serif Bold 9\n"
				"\t-f --foreground FOREGROUND\tSet the default foreground color to FOREGROUND. Default #FFFFFF\n"
				"\t-b --background BACKGROUND\tSet the default background color to BACKGROUND. Default #000000\n"
				"\nStarting from 0, the first LEFT identifiers are allocated for the left part"
				"of the panel, then the next RIGHT for the right part, and the remaining"
				"CENTER for the center. Each part is drawn left-to-right. Input must start with"
				"the identifier and a non-digit character. e.g.:\n"
				"\tpbar --center 1 --left 2 --right 3\n"
				"\t0 my info\n"
				"\t3 info on right\n"
				"\t4 more on right\n"
				"\t5 center info\n"
				"\t4 updated info\n"
				"Result: [my info][]				[center info]		[][info on right][updated info]\n"
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
