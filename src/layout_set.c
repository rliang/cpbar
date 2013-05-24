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

static struct layout_set *layout_set_ensure_next(struct layout_set *set)
{
	if (set->list_next == NULL) {
		PangoContext *context = pango_layout_get_context(set->layout);
		set->list_next = layout_set_new(context);
	}
	return set->list_next;
}

static struct layout_set *layout_set_iterate_to(struct layout_set *set, int index)
{
	for (int i = 0; i < index; ++i) {
		layout_set_ensure_next(set);
		set = set->list_next;
	}

	return set;
}

static void layout_set_reset_layout(struct layout_set *set)
{
	PangoLayout *old = set->layout;
	PangoContext *context = pango_layout_get_context(set->layout);

	set->layout= pango_layout_new(context);
	pango_layout_set_ellipsize(set->layout, PANGO_ELLIPSIZE_END);

	g_object_unref(old);
}

void layout_set_text_update(struct layout_set *set,
		int index, const char *text)
{
	set = layout_set_iterate_to(set, index);
	layout_set_reset_layout(set);
	pango_layout_set_markup(set->layout, text, -1);
}

int layout_set_get_pixel_width(struct layout_set *set)
{
	int total = 0;
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
