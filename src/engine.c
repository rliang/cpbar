/* See LICENSE file for copyright and license details. */

#include "engine.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "layout_set.h"
#include "utils.h"

static cairo_t *cairo_context;
static PangoContext *pango_context;

static struct layout_set *left_set = NULL;
static struct layout_set *right_set = NULL;
static struct layout_set *center_set = NULL;

static double default_foreground[3];
static double default_background[3];

static int canvas_width;
static int canvas_height;

bool engine_init_canvas(cairo_surface_t *surface, int width, int height)
{
	if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS)
		return false;

	cairo_context = cairo_create(surface);
	cairo_surface_destroy(surface);

	if (cairo_status(cairo_context) != CAIRO_STATUS_SUCCESS)
		return false;

	pango_context = pango_cairo_create_context(cairo_context);

	canvas_width = width;
	canvas_height = height;

	return true;
}

void engine_init_sets(const int sizes[3], const char *default_font,
		const char *foreground, const char *background)
{
	PangoFontDescription *font =
		pango_font_description_from_string(default_font);
	pango_context_set_font_description(pango_context, font);

	parse_color(foreground, default_foreground);
	parse_color(background, default_background);

	left_set = layout_set_new(pango_context);
	right_set = layout_set_new(pango_context);
	center_set = layout_set_new(pango_context);
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
	for ( ; set != NULL; set = layout_set_get_next(set)) {
		PangoLayout *current = layout_set_get_layout(set);

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
	int left_width = layout_set_get_pixel_width(left_set);
	int right_width = layout_set_get_pixel_width(right_set);
	int center_width = layout_set_get_pixel_width(center_set);

	int right_begin = canvas_width - right_width;
	int right_end = canvas_width;
	draw_set(right_set, right_begin, right_end);

	int left_begin = 0;
	int left_end = left_width;
	if (left_end > right_begin)
		left_end = right_begin;
	draw_set(left_set, left_begin, left_end);

	int center_begin = (canvas_width - center_width) / 2;
	if (center_begin < left_end)
		center_begin = left_end;
	int center_end = right_begin;
	draw_set(center_set, center_begin, center_end);
}

static void clean_canvas()
{
	cairo_set_source_rgba(cairo_context,
			 default_background[0],
			 default_background[1],
			 default_background[2], 1);
	cairo_paint(cairo_context);
}

static char engine_parse_position(char **input, int *length)
{
	if (*length < 1)
		return '\0';

	char position = (*input)[0];

	*length -= 1;
	*input += 1;

	return position;
}

static int engine_parse_index(char **input, int *length)
{
	int i = 0;
	while (i < *length && isdigit((*input)[i]))
		i += 1;

	(*input)[i] = '\0';
	int index = atoi(*input);

	*length -= i + 1;
	*input += i + 1;

	return index;
}

static struct layout_set *engine_find_position(char position)
{
	switch (position) {
	case 'l':
		return left_set;
	case 'r':
		return right_set;
	case 'c':
		return center_set;
	}
	return NULL;
}

void engine_update(char *input, int length)
{
	if (length < 3)
		return;

	char position = engine_parse_position(&input, &length);
	if (position == '\0')
		return;

	struct layout_set *set = engine_find_position(position);
	if (set == NULL)
		return;

	int index = engine_parse_index(&input, &length);
	if (index < 0)
		return;

	layout_set_text_update(set, index, input);

	engine_refresh();
}

void engine_input_wait()
{
	int length = BUFSIZ;
	char buffer[length];

	if (fgets(buffer, length, stdin) == NULL)
		return;
	buffer[strlen(buffer) - 1] = '\0';

	engine_update(buffer, length);
}

void engine_refresh()
{
	clean_canvas();
	draw_sets();
}

void engine_terminate()
{
	cairo_destroy(cairo_context);
	g_object_unref(pango_context);

	layout_set_destroy(left_set);
	layout_set_destroy(right_set);
	layout_set_destroy(center_set);
}
