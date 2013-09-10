/* See LICENSE file for copyright and license details. */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/*!
 * Duplicates a string to newly-allocated memory.
 * @param string the null-terminated string to duplicate.
 * @return a pointer to the new string, which should be freed after use.
 */
char *strdup(const char *string);

/*!
 * Parses a color in format #rrggbb from a string
 and outputs it as a triplet of doubles.
 * @param string the string to parse from.
 * @param output the array of doubles to output to.
 */
void parse_color(const char *string, double output[3]);

#endif /* UTILS_H */

/* vim: set ts=4 sw=4 noet: */
