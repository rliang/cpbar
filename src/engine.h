/* See LICENSE file for copyright and license details. */

#ifndef ENGINE_H
#define ENGINE_H

#include <pango/pangocairo.h>

void engine_init_canvas(cairo_surface_t *surface, int width, int height,
		const char *foreground, const char *background);
void engine_init_sets(int sizes[3], const char *default_font);
void engine_update(char *string, int id);
void engine_terminate();

#endif /* ENGINE_H */
