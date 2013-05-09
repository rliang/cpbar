/* See LICENSE file for copyright and license details. */

#ifndef window_H
#define window_H

#include <xcb/xcb.h>
#include <stdbool.h>

struct window {
	xcb_connection_t *xcb_connection;
	xcb_visualtype_t *xcb_visualtype;
	xcb_window_t xcb_window;
	uint16_t width;
	uint16_t height;
};

extern struct window window;

void window_init(uint16_t height, bool on_bottom);
void window_flush();
void window_terminate();

#endif /* window_H */
