/* See LICENSE file for copyright and license details. */

#ifndef WINDOW_H
#define WINDOW_H

#include <xcb/xcb.h>
#include <stdbool.h>

struct window {
	xcb_connection_t *xcb_connection;
	xcb_visualtype_t *xcb_visualtype;
	xcb_window_t xcb_window;
	int xcb_file_descriptor;
	uint16_t width;
	uint16_t height;
	void(*redraw)();
};

extern struct window window;

/*!
 * Initializes a window with maximum width, and given height.
 * @param height the height of the window.
 * @param on_bottom whether the window should be placed on the bottom of the
 screen.
 * @param redraw the function to redraw the window.
 */
void window_init(uint16_t height, bool on_bottom, void(*redraw)());

/*!
 * Waits for X events and handles them accordingly.
 */
void window_event_wait();

/*!
 * Redraws the window, which should be initialized first.
 */
void window_flush();

/*!
 * Frees resources allocated by window_init().
 */
void window_terminate();

#endif /* WINDOW_H */
