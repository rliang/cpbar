/* See LICENSE file for copyright and license details. */

#include "window.h"

struct window xcb_window;

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

static void create_window(xcb_screen_t *s, int16_t x, int16_t y)
{
	xcb_create_window(xcb_window.connection, XCB_COPY_FROM_PARENT,
			xcb_window.window, s->root, x, y,
			xcb_window.window_width, xcb_window.window_height,
			0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
			s->root_visual, XCB_CW_EVENT_MASK,
			(const uint32_t []){ [0] = XCB_EVENT_MASK_EXPOSURE });
	xcb_change_window_attributes(xcb_window.connection, xcb_window.window,
			XCB_CW_OVERRIDE_REDIRECT, (const uint32_t []){ 1 });
	xcb_map_window(xcb_window.connection, xcb_window.window);
}

void window_init(uint16_t height, bool on_bottom)
{
	xcb_window.connection = xcb_connect(NULL, NULL);

	xcb_screen_t *s = xcb_setup_roots_iterator(
			xcb_get_setup(xcb_window.connection)).data;
	xcb_window.visualtype = get_root_visual_type(s);

	xcb_window.window = xcb_generate_id(xcb_window.connection);
	xcb_window.window_width = s->width_in_pixels;
	xcb_window.window_height = height;

	int16_t window_x = 0;
	int16_t window_y = on_bottom ? s->height_in_pixels - height : 0;

	create_window(s, window_x, window_y);
}

void window_flush()
{
	xcb_flush(xcb_window.connection);
}

void window_terminate()
{
	xcb_destroy_window(xcb_window.connection, xcb_window.window);
	xcb_disconnect(xcb_window.connection);
}
