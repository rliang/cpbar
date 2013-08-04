/*
 * Copyright (C) 2013 Ricardo Liang <ricardoliang@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "utils.h"

#include <ctype.h>
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
