/* See LICENSE file for copyright and license details. */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct options {
	uint16_t bar_height;
	bool bar_on_bottom;
	char *default_font;
	char *default_foreground;
	char *default_background;
};

/*!
 * Parses and initializes the options struct.
 * @param argc argc from the main function.
 * @param argv argv from the main function.
 * @return the newly allocated options, or NULL.
 */
struct options *options_new(int argc, char **argv);

/*!
 * Frees resources allocated by options_new().
 */
void options_destroy(struct options *options);

#endif /* OPTIONS_H */
