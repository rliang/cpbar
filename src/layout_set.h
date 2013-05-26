#ifndef LAYOUT_SET_H
#define LAYOUT_SET_H

#include <pango/pango.h>

struct layout_set;

/*!
 * Initializes a new layout_set linked list, each node containing a PangoLayout.
 * @param pango_context the PangoContext for creating the PangoLayout's.
 * @return the newly allocated layout_set, or NULL.
 */
struct layout_set *layout_set_new(PangoContext *pango_context);

/*!
 * Frees recursively a layout_set and all children nodes.
 * @param set the root layout_set to destroy.
 */
void layout_set_destroy(struct layout_set *set);

/*!
 * Returns the PangoLayout in the node.
 * @param the layout_set node.
 * @return the PangoLayout.
 */
PangoLayout *layout_set_get_layout(struct layout_set *set);

/*!
 * Updates the Pango markup of the indexed node from a root set.
 * @param set the root layout_set.
 * @param index the index of the node.
 * @param text the string containing the Pango markup.
 */
void layout_set_text_update(struct layout_set *set,
		size_t index, const char *text);

/*!
 * Iterates over the children nodes, adding up the total width.
 * @param set the root layout_set to iterate from.
 * @return the total width in pixels.
 */
unsigned int layout_set_get_pixel_width(struct layout_set *set);

/*!
 * Returns the next child node.
 * @param set the node to get the child from.
 * @return the child layout_set, or NULL.
 */
struct layout_set *layout_set_get_next(struct layout_set *set);

#endif /* LAYOUT_SET_H */
