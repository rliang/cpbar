#ifndef LAYOUT_SET_H
#define LAYOUT_SET_H

#include <pango/pango.h>

struct layout_set;

struct layout_set *layout_set_new(PangoContext *pango_context);
void layout_set_destroy(struct layout_set *set);
PangoLayout *layout_set_get_layout(struct layout_set *set);
void layout_set_text_update(struct layout_set *set, int index, const char *text);
int layout_set_get_pixel_width(struct layout_set *set);
struct layout_set *layout_set_get_next(struct layout_set *set);

#endif /* LAYOUT_SET_H */
