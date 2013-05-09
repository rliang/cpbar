/* See LICENSE file for copyright and license details. */

#include "engine.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

struct layout_set {
	PangoLayout **layout_list;
	int length;
};

static cairo_t *context;
static int canvas_width;
static int canvas_height;
static double default_foreground[3];
static double default_background[3];
static struct layout_set sets[3];

void engine_init_canvas(cairo_surface_t *surface, int width, int height,
		const char *foreground, const char *background)
{
	context = cairo_create(surface);
	canvas_width = width;
	canvas_height = height;
	parse_color(foreground, default_foreground);
	parse_color(background, default_background);
}

void engine_init_sets(int sizes[3], const char *default_font)
{
	PangoFontDescription *font =
		pango_font_description_from_string(default_font);
	for (int i = 0; i < 3; ++i) {
		int length = sizes[i];
		sets[i].layout_list = calloc(length, sizeof(PangoLayout *));
		sets[i].length = length;
		for (int j = 0; j < length; ++j) {
			sets[i].layout_list[j] =
				pango_cairo_create_layout(context);
			pango_layout_set_font_description(
					sets[i].layout_list[j], font);
			/* pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END); */
		}
	}
}

static PangoLayout *get_layout(int id)
{
	for (int i = 0; i < 3; ++i) {
		if (id < sets[i].length)
			return sets[i].layout_list[id];
		id -= sets[i].length;
	}
	return NULL;
}

static int get_set_width(struct layout_set *set)
{
	int total = 0;
	for (int i = 0; i < set->length; ++i) {
		int width = 0;
		pango_layout_get_pixel_size(set->layout_list[i], &width, NULL);
		total += width;
	}
	return total;
}

void draw_text(PangoLayout *layout, int x, int text_height)
{
	cairo_set_source_rgba(context,
			default_foreground[0],
			default_foreground[1],
			default_foreground[2], 1);
	cairo_move_to(context, x, (canvas_height - text_height) / 2);
	pango_cairo_update_layout(context, layout);
	pango_cairo_show_layout(context, layout);
}

static void draw_set(struct layout_set *set, int lower_limit, int upper_limit)
{
	for (int i = 0; i < set->length; ++i) {
		PangoLayout *current = set->layout_list[i];
		int width, height;
		pango_layout_get_pixel_size(current, &width, &height);
		pango_layout_set_width(current,
				(upper_limit - lower_limit) * PANGO_SCALE);
		draw_text(current, lower_limit, height);
		lower_limit += width;
	}
}

static void draw_sets()
{
	int lower_limit = get_set_width(&sets[0]);
	draw_set(&sets[0], 0, lower_limit);
	int upper_limit = canvas_width - get_set_width(&sets[1]);
	draw_set(&sets[1], upper_limit, canvas_width);
	int center_w = get_set_width(&sets[2]);
	int center_x = (canvas_width - center_w) / 2;
	draw_set(&sets[2], center_x, upper_limit);
}

void clean_canvas()
{
	 cairo_set_source_rgba(context,
			 default_background[0],
			 default_background[1],
			 default_background[2], 1);
	 cairo_paint(context);
}

void engine_update(char *string, int id)
{
	if (id < 0)
		return;
	PangoLayout *layout = get_layout(id);
	if (layout == NULL)
		return;
	pango_layout_set_markup(layout, string, -1);
	clean_canvas();
	draw_sets();
}

void engine_terminate()
{
	cairo_destroy(context);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < sets[i].length; ++j)
			g_object_unref(sets[i].layout_list[j]);
		free(sets[i].layout_list);
	}
}
