#ifndef OPTIONS_H
#define OPTIONS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct options {
	int sizes[3];
	uint16_t bar_height;
	bool bar_on_bottom;
	char *default_font;
	char *default_foreground;
	char *default_background;
};

extern struct options options;

void options_init(int argc, char **argv);
void options_terminate();

#endif /* OPTIONS_H */
