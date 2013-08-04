/*
 * Copyright (C) 2013 Ricardo Liang <ricardoliang@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "options.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "utils.h"

#define DEFAULT_HEIGHT 16
#define DEFAULT_ON_BOTTOM false
#define DEFAULT_FONT "Sans-Serif Bold 9"
#define DEFAULT_FOREGROUND "#ffffff"
#define DEFAULT_BACKGROUND "#000000"
#define USAGE "\
Usage: %s pbar [-h] [-m] [-g HEIGHT] [-t FONT] [-f FOREGROUND] [-b BACKGROUND]\n\
\t-h --help\t\t\tDisplay this help\n\
\t-m --bottom\t\t\tPlace bar on the bottom of the screen\n\
\t-g --height HEIGHT\t\tSet bar height to HEIGHT. Default 16\n\
\t-t --font FONT\t\t\tSet the default font to FONT. Default Sans-Serif Bold 9\n\
\t-f --foreground FOREGROUND\tSet the default foreground color to FOREGROUND. Default #FFFFFF\n\
\t-b --background BACKGROUND\tSet the default background color to BACKGROUND. Default #000000\n\
\nThis program displays three sets of textual information, on the left of the bar,\
right and center. Input must start with the position -- 'l', 'r' or 'c'; the\
index, and a non-digit separator character before the textual information.\
e.g.:\n\
\t$ %s\n\
\tl0 my info\n\
\tr1 info on right\n\
\tr2 more on right\n\
\tc0 center info\n\
\tr2 updated info\n\
Result: [my info][]				[center info]		[][info on right][updated info]\n\
"

/*!
 * Allocates and sets options to their defaults, so they can be safely freed.
 * @see options_destroy
 */
static struct options *options_allocate()
{
	struct options *new = malloc(sizeof(struct options));
	if (new == NULL)
		return NULL;

	new->bar_height = DEFAULT_HEIGHT;
	new->bar_on_bottom = DEFAULT_ON_BOTTOM;
	new->default_font = strdup(DEFAULT_FONT);
	new->default_foreground = strdup(DEFAULT_FOREGROUND);
	new->default_background = strdup(DEFAULT_BACKGROUND);

	return new;
}

struct options *options_new(int argc, char **argv)
{
	struct options *options = options_allocate();
	if (options == NULL)
		return NULL;

	struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "height", required_argument, 0, 'g' },
		{ "bottom", no_argument, 0, 'm' },
		{ "font", required_argument, 0, 't' },
		{ "foreground", required_argument, 0, 'f' },
		{ "background", required_argument, 0, 'b' }
	};

	while (1) {
		int arg = getopt_long(argc, argv, "hmg:t:f:b:",
				long_options, NULL);
		if (arg == -1)
			break;
		switch (arg) {
		case 'g':
			options->bar_height = atoi(optarg);
			break;
		case 'm':
			options->bar_on_bottom = true;
			break;
		case 't':
			free(options->default_font);
			options->default_font = strdup(optarg);
			break;
		case 'f':
			free(options->default_foreground);
			options->default_foreground = strdup(optarg);
			break;
		case 'b':
			free(options->default_background);
			options->default_background = strdup(optarg);
			break;
		default:
			printf(USAGE, argv[0], argv[0]); 
			options_destroy(options);
			return NULL;
		}
	}

	return options;
}

void options_destroy(struct options *options)
{
	char *strings[] = {
		options->default_font,
		options->default_foreground,
		options->default_background
	};

	/*
	 *for (int i = 0; i < 3; ++i)
	 *        free(strings[i]);
	 */

	free(options);
}
