/* See LICENSE file for copyright and license details. */

#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cairo-xcb.h>

#include "options.h"
#include "window.h"
#include "engine.h"

/*!
 * Initializes all modules. After their use, terminate should be called.
 * @param argc argc from the main function.
 * @param argv argv from the main function.
 * @see main()
 * @see terminate()
 * @see options_init()
 * @see window_init()
 * @see engine_init_canvas()
 * @see engine_init_sets()
 * @return true if the initialization has succeeded, otherwise false.
 */
static bool init(int argc, char *argv[])
{
	options_init(argc, argv);
	window_init(options.bar_height, options.bar_on_bottom, engine_refresh);

	cairo_surface_t *surface = cairo_xcb_surface_create(
			window.xcb_connection, window.xcb_window,
			window.xcb_visualtype, window.width, window.height);

	if (!engine_init_canvas(surface, window.width, window.height))
		return false;

	engine_init_sets(options.sizes, options.default_font,
			options.default_foreground, options.default_background);

	window_flush();
	return true;
}

/*!
 * Executes the main loop until it receives null input from STDIN.
 Requires all modules to have been initialized.
 * @see init()
 */
static void main_loop()
{
	struct pollfd polls[2] = {
		{ STDIN_FILENO, POLLIN, 0 },
		{ xcb_get_file_descriptor(window.xcb_connection), POLLIN, 0 }
	};

	for (;;) {
		if (!poll(polls, 2, -1))
			continue;

		if (polls[0].revents & POLLHUP)
			polls[0].fd = -1;
		if (polls[0].revents & POLLIN)
			engine_input_wait();
		if (polls[1].revents & POLLIN)
			window_event_wait();

		window_flush();
	}
}

/*!
 * Frees resources allocated by init. At the time of writing, not guaranteed
 to succeed if init fails.
 * @see init()
 */
static void terminate()
{
	options_terminate();
	engine_terminate();
	window_terminate();
}

/*!
 * Main function. Calls init, main_loop and terminate.
 * @see init()
 * @see main_loop()
 * @see terminate()
 */
int main(int argc, char *argv[])
{
	atexit(terminate);

	if (init(argc, argv))
		main_loop();
}
