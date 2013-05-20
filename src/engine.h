/* See LICENSE file for copyright and license details. */

#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include <pango/pangocairo.h>

/*!
 * Initializes the engine's canvas to draw into.
 * @param surface pointer to the Cairo surface to create the canvas from.
 * @param width width of the canvas.
 * @param height height of the canvas.
 * @return true on success, false otherwise.
 */
bool engine_init_canvas(cairo_surface_t *surface, int width, int height);

/*!
 * Initializes the engine's sets, later updated with engine_update(). Must be
 called after engine_init_canvas().
 * @param sizes lengths of left, right and center's sets, respectively.
 * @param default_font string containing the default font to be used.
 * @param foreground string containing the default foreground, in #rrggbb
 format.
 * @param background string containing the default background, in #rrggbb
 format.
 * @see engine_init_canvas()
 */
void engine_init_sets(const int sizes[3], const char *default_font,
		const char *foreground, const char *background);

/*!
 * Updates the text at the given ID. If ID is greater than each of the, in
 order, left, right and center sets' length, it attempts the following one.
 Triggers a redraw on success. Must be called after initialization.
 * @param string null-terminated string containing the desired text to display.
 * @param id the ID to be updated.
 */
void engine_update(char *string, int id);

/*!
 * Frees resources allocated by engine_init_canvas() and engine_init_sets().
 */
void engine_terminate();

#endif /* ENGINE_H */
