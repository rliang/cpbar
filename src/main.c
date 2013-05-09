#include <stdio.h>
#include <stdlib.h>
#include <cairo-xcb.h>

#include "options.h"
#include "xcbwindow.h"
#include "cairopango.h"

int main(int argc, char *argv[])
{
	options_init(argc, argv);
	xcbwindow_init(options.bar_height, options.bar_on_bottom);
	int window_width = xcb_window.window_width;
	int window_height = options.bar_height;
	cairo_surface_t *surface = cairo_xcb_surface_create(
			xcb_window.connection, xcb_window.window,
			xcb_window.visualtype, window_width, window_height);
	cairopango_init(surface, window_width, window_height, options.sizes,
			options.default_font, options.default_foreground,
			options.default_background);
	xcbwindow_flush();
	int id;
	char buffer[BUFSIZ];
	for(;;) {
		scanf("%d %s", &id, buffer);
		cairopango_update(buffer, id);
		xcbwindow_flush();
	}
	options_terminate();
	cairopango_terminate();
	xcbwindow_terminate();
	return 0;
}
