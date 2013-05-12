/* See LICENSE file for copyright and license details. */

#include "engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

struct layout_set {
	PangoLayout **layout_list;
	int length;
};

static struct layout_set sets[3];

static cairo_t *cairo_context;
static PangoContext *pango_context;

static int canvas_width;
static int canvas_height;

static double default_foreground[3];
static double default_background[3];

bool engine_init_canvas(cairo_surface_t *surface, int width, int height)
{
	cairo_context = cairo_create(surface);
	pango_context = pango_cairo_create_context(cairo_context);
	canvas_width = width;
	canvas_height = height;
}

static bool create_layout(PangoLayout **location)
{
	if (location == NULL)
		return false;
	if (*location != NULL)
		g_object_unref(*location);

	*location = pango_layout_new(pango_context);
	pango_layout_set_ellipsize(*location, PANGO_ELLIPSIZE_END);

	return true;
}

static void init_set(struct layout_set *set, int length)
{
	set->layout_list = calloc(length, sizeof(PangoLayout *));
	set->length = length;

	for (int j = 0; j < length; ++j)
		create_layout(&set->layout_list[j]);
}

void engine_init_sets(const int sizes[3], const char *default_font,
		const char *foreground, const char *background)
{
	PangoFontDescription *font =
		pango_font_description_from_string(default_font);
	pango_context_set_font_description(pango_context, font);

	parse_color(foreground, default_foreground);
	parse_color(background, default_background);

	for (int i = 0; i < 3; ++i)
		init_set(&sets[i], sizes[i]);
}

static PangoLayout **get_layout_location(int id)
{
	for (int i = 0; i < 3; ++i) {
		if (id < 0)
			break;
		if (id < sets[i].length)
			return &sets[i].layout_list[id];
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

static void draw_text(PangoLayout *layout, int x, int text_height)
{
	cairo_set_source_rgba(cairo_context,
			default_foreground[0],
			default_foreground[1],
			default_foreground[2], 1);
	cairo_move_to(cairo_context, x, (canvas_height - text_height) / 2);

	pango_cairo_update_layout(cairo_context, layout);
	pango_cairo_show_layout(cairo_context, layout);
}

static void draw_set(struct layout_set *set, int lower_limit, int upper_limit)
{
	for (int i = 0; i < set->length; ++i) {
		PangoLayout *current = set->layout_list[i];

		int width, height;
		pango_layout_get_pixel_size(current, &width, &height);
		pango_layout_set_width(current,
				(1 + upper_limit - lower_limit) * PANGO_SCALE);

		draw_text(current, lower_limit, height);
		lower_limit += width;
	}
}

static void draw_sets()
{
	int left_width = get_set_width(&sets[0]);
	int right_width = get_set_width(&sets[1]);
	int center_width = get_set_width(&sets[2]);

	int right_begin = canvas_width - right_width;
	int right_end = canvas_width;
	draw_set(&sets[1], right_begin, right_end);

	int left_begin = 0;
	int left_end = left_width;
	if (left_end > right_begin)
		left_end = right_begin;
	draw_set(&sets[0], left_begin, left_end);

	int center_begin = (canvas_width - center_width) / 2;
	if (center_begin < left_end)
		center_begin = left_end;
	int center_end = right_begin;
	draw_set(&sets[2], center_begin, center_end);
}

static void clean_canvas()
{
	cairo_set_source_rgba(cairo_context,
			 default_background[0],
			 default_background[1],
			 default_background[2], 1);
	cairo_paint(cairo_context);
}

void engine_update(char *string, int id)
{
	PangoLayout **layout_location = get_layout_location(id);

	const char *current = pango_layout_get_text(*layout_location);
	if (current != NULL && strncmp(current, string, BUFSIZ) == 0)
		return;

	create_layout(layout_location);
	pango_layout_set_markup(*layout_location, string, -1);

	clean_canvas();
	draw_sets();
}

void engine_terminate()
{
	cairo_destroy(cairo_context);
	g_object_unref(pango_context);

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < sets[i].length; ++j)
			g_object_unref(sets[i].layout_list[j]);
		free(sets[i].layout_list);
	}
}
