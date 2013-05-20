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
 * Parse the first integer in a string and output the rest to a buffer.
 * @param input the buffer to parse from.
 * @param id valid pointer to the output integer.
 * @param string valid pointer to the buffer to the output string.
 * @param length length of both input and string buffers.
 */
bool parse_input(char *input, int *id, char *string, int length);

/*!
 * Parses a color in format #rrggbb from a string
 and outputs it as a triplet of doubles.
 * @param string the string to parse from.
 * @param output the array of doubles to output to.
 */
void parse_color(const char *string, double output[3]);

#endif /* UTILS_H */
