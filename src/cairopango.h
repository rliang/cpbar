#ifndef CAIROPANGO_H
#define CAIROPANGO_H

#include <pango/pangocairo.h>

void cairopango_init(cairo_surface_t *surface, int width, int height,
		int sizes[3], const char *default_font,
		const char *foreground, const char *background);
void cairopango_update(char *string, int id);
void cairopango_terminate();

#endif /* CAIROPANGO_H */
