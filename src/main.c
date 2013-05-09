/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <cairo-xcb.h>

#include "options.h"
#include "xcbwindow.h"
#include "engine.h"

bool init(int argc, char *argv[])
{
	options_init(argc, argv);
	xcbwindow_init(options.bar_height, options.bar_on_bottom);

	int window_width = xcb_window.window_width;
	int window_height = options.bar_height;

	cairo_surface_t *surface = cairo_xcb_surface_create(
			xcb_window.connection, xcb_window.window,
			xcb_window.visualtype, window_width, window_height);

	engine_init_canvas(surface, window_width, window_height,
			options.default_foreground, options.default_background);
	engine_init_sets(options.sizes, options.default_font);

	xcbwindow_flush();

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
		xcbwindow_flush();
	}
}

void terminate()
{
	options_terminate();
	engine_terminate();
	xcbwindow_terminate();
}

int main(int argc, char *argv[])
{
	if (init(argc, argv))
		main_loop();
	terminate();
}
