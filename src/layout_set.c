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

#include "layout_set.h"

#include <stdlib.h>

struct layout_set {
	PangoLayout *layout;
	struct layout_set *list_next;
};

struct layout_set *layout_set_new(PangoContext *pango_context)
{
	if (pango_context == NULL)
		return NULL;

	struct layout_set *new = malloc(sizeof(struct layout_set));
	if (new == NULL)
		return NULL;

	new->layout= pango_layout_new(pango_context);
	new->list_next = NULL;

	return new;
}

void layout_set_destroy(struct layout_set *set)
{
	while (set != NULL) {
		struct layout_set *next = set->list_next;

		g_object_unref(set->layout);
		free(set);

		set = next;
	}
}

PangoLayout *layout_set_get_layout(struct layout_set *set)
{
	return set->layout;
}

/*!
 * Ensures that the node contains a child.
 * @param set the node.
 * @returns the child node.
 */
static struct layout_set *layout_set_ensure_next(struct layout_set *set)
{
	if (set->list_next == NULL) {
		PangoContext *context = pango_layout_get_context(set->layout);
		set->list_next = layout_set_new(context);
	}
	return set->list_next;
}

/*!
 * Iterates over the layout_set list until the specified index.
 * @param set the root node to iterate from.
 * @index the amount of iterations.
 * @return the last node iterated.
 * @see layout_set_ensure_next
 */
static struct layout_set *layout_set_iterate_to(struct layout_set *set,
		size_t index)
{
	for (size_t i = 0; i < index; ++i) {
		layout_set_ensure_next(set);
		set = set->list_next;
	}

	return set;
}

/*!
 * Re-allocates a layout_set node's PangoLayout.
 * @param set the node to replace the PangoLayout.
 */
static void layout_set_reset_layout(struct layout_set *set)
{
	PangoLayout *old = set->layout;
	PangoContext *context = pango_layout_get_context(set->layout);

	set->layout= pango_layout_new(context);
	pango_layout_set_ellipsize(set->layout, PANGO_ELLIPSIZE_END);

	g_object_unref(old);
}

void layout_set_text_update(struct layout_set *set,
		size_t index, const char *text)
{
	set = layout_set_iterate_to(set, index);
	layout_set_reset_layout(set);
	pango_layout_set_markup(set->layout, text, -1);
}

unsigned int layout_set_get_pixel_width(struct layout_set *set)
{
	unsigned int total = 0;
	for ( ; set != NULL; set = set->list_next) {
		int width = 0;
		pango_layout_get_pixel_size(set->layout, &width, NULL);
		total += width;
	}
	return total;
}

struct layout_set *layout_set_get_next(struct layout_set *set)
{
	return set->list_next;
}

/* vim: set ts=4 sw=4 noet: */
