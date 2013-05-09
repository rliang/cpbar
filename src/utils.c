#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strdup(const char *arg)
{
	char *buffer = calloc(strlen(arg), sizeof(char));
	strcpy(buffer, arg);
	return buffer;
}

void parse_color(const char *string, double output[3])
{
	char r[2] = { string[1], string[2] };
	char g[2] = { string[3], string[4] };
	char b[2] = { string[5], string[6] };
	unsigned a[3];
	sscanf(r, "%02x", a);
	sscanf(g, "%02x", a+1);
	sscanf(b, "%02x", a+2);
	output[0] = a[0] / 255.0;
	output[1] = a[1] / 255.0;
	output[2] = a[2] / 255.0;
}
