/* See LICENSE file for copyright and license details. */

#ifndef XCBWINDOW_H
#define XCBWINDOW_H

#include <xcb/xcb.h>
#include <stdbool.h>

struct xcbwindow {
	xcb_connection_t *connection;
	xcb_visualtype_t *visualtype;
	xcb_window_t window;
	uint16_t window_width;
};

extern struct xcbwindow xcb_window;

void xcbwindow_init(uint16_t height, bool on_bottom);
void xcbwindow_flush();
void xcbwindow_terminate();

#endif /* XCBWINDOW_H */
