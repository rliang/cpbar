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

extern struct options options;

/*!
 * Parses and initializes the options struct.
 * @param argc argc from the main function.
 * @param argv argv from the main function.
 * @return whether options were correctly initialized.
 */
bool options_init(int argc, char **argv);

/*!
 * Frees resources allocated by options_init().
 */
void options_terminate();

#endif /* OPTIONS_H */
