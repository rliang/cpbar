/* See LICENSE file for copyright and license details. */

#ifndef window_H
#define window_H

#include <xcb/xcb.h>
#include <stdbool.h>

struct window {
	xcb_connection_t *connection;
	xcb_visualtype_t *visualtype;
	xcb_window_t window;
	uint16_t window_width;
	uint16_t window_height;
};

extern struct window window;

void window_init(uint16_t height, bool on_bottom);
void window_flush();
void window_terminate();

#endif /* window_H */
