/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <cairo-xcb.h>

#include "options.h"
#include "window.h"
#include "engine.h"

bool init(int argc, char *argv[])
{
	options_init(argc, argv);
	window_init(options.bar_height, options.bar_on_bottom);

	int window_width = window.window_width;
	int window_height = options.bar_height;

	cairo_surface_t *surface = cairo_xcb_surface_create(
			window.connection, window.window,
			window.visualtype, window_width, window_height);

	engine_init_canvas(surface, window_width, window_height,
			options.default_foreground, options.default_background);
	engine_init_sets(options.sizes, options.default_font);

	window_flush();

	return true;
}

void main_loop()
{
	int id;
	char buffer[BUFSIZ];
	for (;;) {
		if (scanf("%d %s", &id, buffer) != 2)
			continue;
		buffer[BUFSIZ - 1] = '\0';
		engine_update(buffer, id);
		window_flush();
	}
}

void terminate()
{
	options_terminate();
	engine_terminate();
	window_terminate();
}

int main(int argc, char *argv[])
{
	if (init(argc, argv))
		main_loop();
	terminate();
}
