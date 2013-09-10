/* See LICENSE file for copyright and license details. */

#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include <pango/pangocairo.h>

/*!
 * Initializes the engine's canvas to draw size_to.
 * @param surface posize_ter to the Cairo surface to create the canvas from.
 * @param width width of the canvas.
 * @param height height of the canvas.
 * @return true on success, false otherwise.
 */
bool engine_init_canvas(cairo_surface_t *surface,
		unsigned int width, unsigned int height);

/*!
 * Initializes the engine's sets, later updated with engine_update(). Must be
 called after engine_init_canvas().
 * @param default_font string containing the default font to be used.
 * @param foreground string containing the default foreground, in #rrggbb
 format.
 * @param background string containing the default background, in #rrggbb
 format.
 * @see engine_init_canvas()
 */
void engine_init_sets(const char *default_font,
		const char *foreground, const char *background);

/*!
 * Waits for STDIN input and updates text as needed.
 */
void engine_input_wait();

/*!
 * Redraws the canvas, without updating text.
 */
void engine_refresh();

/*!
 * Frees resources allocated by engine_init_canvas() and engine_init_sets().
 */
void engine_terminate();

#endif /* ENGINE_H */

/* vim: set ts=4 sw=4 noet: */
