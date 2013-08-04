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

#include "window.h"

struct window window;

/*!
 * Some drawing surfaces such as Cairo require the visual type.
 * @param s the screen to obtain the visual type from.
 * @return pointer to the visual type on success, or NULL.
 * @see window_init()
 */
static xcb_visualtype_t *get_root_visual_type(xcb_screen_t *s)
{
	for (xcb_depth_iterator_t di =
			xcb_screen_allowed_depths_iterator(s);
			di.rem ; xcb_depth_next(&di)) {
		for (xcb_visualtype_iterator_t vi =
				xcb_depth_visuals_iterator(di.data);
				vi.rem; xcb_visualtype_next(&vi)) {
			if (s->root_visual == vi.data->visual_id)
				return vi.data;
		}
	}
	return NULL;
}

/*!
 * Creates the XCB window from parameters and the window struct's members.
 * @param s the screen to create the XCB window from.
 * @param x the position in pixels of the X coordinate to place it.
 * @param y the position in pixels of the y coordinate to place it.
 */
static void create_window(xcb_screen_t *s, int16_t x, int16_t y)
{
	xcb_create_window(window.xcb_connection, XCB_COPY_FROM_PARENT,
			window.xcb_window, s->root, x, y,
			window.width, window.height,
			0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
			s->root_visual, XCB_CW_EVENT_MASK,
			(const uint32_t []){ XCB_EVENT_MASK_EXPOSURE });
	xcb_change_window_attributes(window.xcb_connection, window.xcb_window,
			XCB_CW_OVERRIDE_REDIRECT, (const uint32_t []){ 1 });
	xcb_map_window(window.xcb_connection, window.xcb_window);
}

void window_init(uint16_t height, bool on_bottom, void(*redraw)())
{
	window.xcb_connection = xcb_connect(NULL, NULL);
	window.xcb_file_descriptor =
		xcb_get_file_descriptor(window.xcb_connection);

	xcb_screen_t *s = xcb_setup_roots_iterator(
			xcb_get_setup(window.xcb_connection)).data;
	window.xcb_visualtype = get_root_visual_type(s);

	window.xcb_window = xcb_generate_id(window.xcb_connection);
	window.width = s->width_in_pixels;
	window.height = height;

	int16_t window_x = 0;
	int16_t window_y = on_bottom ? s->height_in_pixels - height : 0;

	create_window(s, window_x, window_y);

	window.redraw = redraw;
}

void window_event_wait()
{
	xcb_generic_event_t *xcb_event;
	xcb_expose_event_t *xcb_expose_event;
	while ((xcb_event = xcb_poll_for_event(window.xcb_connection))) {
		xcb_expose_event = (xcb_expose_event_t *)xcb_event;
		switch (xcb_event->response_type & 0x7F) {
		case XCB_EXPOSE:
			if (xcb_expose_event->count == 0)
				window.redraw();
		}
	}
}

void window_flush()
{
	xcb_flush(window.xcb_connection);
}

void window_terminate()
{
	xcb_destroy_window(window.xcb_connection, window.xcb_window);
	xcb_disconnect(window.xcb_connection);
}
