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

#include <poll.h>
#include <stdlib.h>
#include <unistd.h>
#include <cairo-xcb.h>

#include "options.h"
#include "window.h"
#include "engine.h"

/*!
 * Initializes all modules. After their use, terminate should be called.
 * @param argc argc from the main function.
 * @param argv argv from the main function.
 * @return true if the initialization has succeeded, otherwise false.
 * @see main()
 * @see terminate()
 * @see options_init()
 * @see window_init()
 * @see engine_init_canvas()
 * @see engine_init_sets()
 */
static bool init(int argc, char *argv[])
{
	struct options *options = options_new(argc, argv);
	if (options == NULL)
		return false;

	window_init(options->bar_height, options->bar_on_bottom, engine_refresh);

	cairo_surface_t *surface = cairo_xcb_surface_create(
			window.xcb_connection, window.xcb_window,
			window.xcb_visualtype, window.width, window.height);

	if (!engine_init_canvas(surface, window.width, window.height))
		return false;

	engine_init_sets(options->default_font,
			options->default_foreground, options->default_background);

	options_destroy(options);

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
		{ window.xcb_file_descriptor, POLLIN, 0 }
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
