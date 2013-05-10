/* See LICENSE file for copyright and license details. */

#ifndef UTILS_H
#define UTILS_H

char *strdup(const char *string);
bool parse_input(char *input, int *id, char *string, int length);
void parse_color(const char *string, double output[3]);

#endif /* UTILS_H */
