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

#include "engine.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "layout_set.h"
#include "utils.h"

static cairo_t *cairo_context;

static struct layout_set *left_set = NULL;
static struct layout_set *right_set = NULL;
static struct layout_set *center_set = NULL;

static double default_foreground[3];
static double default_background[3];

static unsigned int canvas_width;
static unsigned int canvas_height;

bool engine_init_canvas(cairo_surface_t *surface,
		unsigned int width, unsigned int height)
{
	if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS)
		return false;

	cairo_context = cairo_create(surface);
	cairo_surface_destroy(surface);

	if (cairo_status(cairo_context) != CAIRO_STATUS_SUCCESS)
		return false;

	canvas_width = width;
	canvas_height = height;

	return true;
}

void engine_init_sets(const char *default_font,
		const char *foreground, const char *background)
{
	PangoFontDescription *font =
		pango_font_description_from_string(default_font);
	PangoContext *pango_context =
		pango_cairo_create_context(cairo_context);
	pango_context_set_font_description(pango_context, font);

	parse_color(foreground, default_foreground);
	parse_color(background, default_background);

	left_set = layout_set_new(pango_context);
	right_set = layout_set_new(pango_context);
	center_set = layout_set_new(pango_context);
}

/*!
 * Renders a single PangoLayout horizontally. Places text on the vertical
 middle of the canvas.
 * @param layout the PangoLayout to draw.
 * @param x the X coordinate on the canvas to draw.
 * @param text_height height of the text in pixels.
 */
static void engine_draw_text(PangoLayout *layout,
		int x, unsigned int text_height)
{
	cairo_set_source_rgba(cairo_context,
			default_foreground[0],
			default_foreground[1],
			default_foreground[2], 1);
	cairo_move_to(cairo_context, x, (canvas_height - text_height) / 2);

	pango_cairo_update_layout(cairo_context, layout);
	pango_cairo_show_layout(cairo_context, layout);
}

/*!
 * Draw a layout_set, from left to right.
 * @param set the layout_set.
 * @param lower_limit the X coordinate on the canvas to start drawing.
 * @param upper_limit the X coordinate on the canvas to stop drawing.
 * @see engine_draw_text
 */
static void engine_draw_set(struct layout_set *set,
		int lower_limit, int upper_limit)
{
	for ( ; set != NULL; set = layout_set_get_next(set)) {
		PangoLayout *current = layout_set_get_layout(set);

		int width, height;
		pango_layout_get_pixel_size(current, &width, &height);
		pango_layout_set_width(current,
				(1 + upper_limit - lower_limit) * PANGO_SCALE);

		engine_draw_text(current, lower_limit, height);
		lower_limit += width;
	}
}

/*!
 * Draws all the layout_sets. To prevent overlapping, the left set "pushes" the
 center set to the right, and the right set always supercedes the others.
 * @see engine_draw_set
 */
static void engine_draw_sets()
{
	unsigned int left_width = layout_set_get_pixel_width(left_set);
	unsigned int right_width = layout_set_get_pixel_width(right_set);
	unsigned int center_width = layout_set_get_pixel_width(center_set);

	int right_begin = canvas_width - right_width;
	int right_end = canvas_width;
	engine_draw_set(right_set, right_begin, right_end);

	int left_begin = 0;
	int left_end = left_width;
	if (left_end > right_begin)
		left_end = right_begin;
	engine_draw_set(left_set, left_begin, left_end);

	int center_begin = (canvas_width - center_width) / 2;
	if (center_begin < left_end)
		center_begin = left_end;
	int center_end = right_begin;
	engine_draw_set(center_set, center_begin, center_end);
}

/*!
 * Paints the canvas with the default background color.
 */
static void engine_clean_canvas()
{
	cairo_set_source_rgba(cairo_context,
			 default_background[0],
			 default_background[1],
			 default_background[2], 1);
	cairo_paint(cairo_context);
}

/*!
 * Parses the first character from a string, and increments it.
 * @param input the string to parse and increment.
 * @param length the length of the string. Will be decremented.
 * @return the parsed character.
 */
static char engine_parse_position(char **input, size_t *length)
{
	if (*length < 1)
		return '\0';

	char position = (*input)[0];

	*length -= 1;
	*input += 1;

	return position;
}

/*!
 * Parses the first size_teger from a string, and increments it.
 * @param input the string to parse and increment.
 * @param length the length of the string. Will be decremented.
 * @return the parsed size_teger.
 */
static size_t engine_parse_index(char **input, size_t *length)
{
	size_t i = 0;
	while (i < *length && isdigit((*input)[i]))
		i += 1;

	(*input)[i] = '\0';
	size_t index = atoi(*input);

	*length -= i + 1;
	*input += i + 1;

	return index;
}

/*!
 * size_terprets a char to a layout_set.
 * @param position the character.
 * @return the layout_set size_terpreted, or NULL.
 */
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

void engine_update(char *input, size_t length)
{
	if (length < 3)
		return;

	char position = engine_parse_position(&input, &length);
	if (position == '\0')
		return;

	struct layout_set *set = engine_find_position(position);
	if (set == NULL)
		return;

	size_t index = engine_parse_index(&input, &length);

	layout_set_text_update(set, index, input);

	engine_refresh();
}

void engine_input_wait()
{
	size_t length = BUFSIZ;
	char buffer[length];

	if (fgets(buffer, length, stdin) == NULL)
		return;
	buffer[strlen(buffer) - 1] = '\0';

	engine_update(buffer, length);
}

void engine_refresh()
{
	engine_clean_canvas();
	engine_draw_sets();
}

void engine_terminate()
{
	cairo_destroy(cairo_context);

	layout_set_destroy(left_set);
	layout_set_destroy(right_set);
	layout_set_destroy(center_set);
}

/* vim: set ts=4 sw=4 noet: */
