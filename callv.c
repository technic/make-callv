/* GNU make callv function - a mix of call and value
 * Copyright (C) 2015  Alex Maystrenko <alexeytech@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <gnumake.h>

int plugin_is_GPL_compatible;

int max(int a, int b) {
	if(a > b)
		return a;
	else
		return b;
}

char *
callv(const char *nm, int argc, char **argv)
{
	char *cmd = malloc(strlen(argv[0]) + 20);
	sprintf(cmd, "$(value %s)", argv[0]);
	char *code = gmk_expand(cmd);
	free(cmd);
	
	unsigned int len = strlen(code);
	unsigned int *lens = malloc(sizeof(int) * argc);
	for(int i = 0; i < argc; i++) {
		lens[i] = strlen(argv[i]);
	}
	
	unsigned int newlen = len * 2;
	char *str = malloc(newlen);
	int pos = 0;
	int newpos = 0;
	
	for(pos = 0; pos <= len; pos++) {
		int n = 0;
		char *src;
		if(code[pos] == '$' && code[pos+1] != '$') {
			int i = code[pos+1] - '0';
			if(i >= 0 && i < argc) {
				n = lens[i];
				src = argv[i];
				pos++;
			} else if(i >= argc && i < 10) {
				pos++;
			} else {
				n = 1;
				src = code + pos;
			}
		} else if(code[pos] == '$' && code[pos+1] == '$') {
			n = 2;
			src = code + pos;
			pos++;
		} else {
			n = 1;
			src = code + pos;
		}
		
		if (n > 0) {
			if (newpos + n > newlen) {
				newlen += max(100, n);
				realloc(str, newlen);
			}
			strncpy(str+newpos, src, n);
			newpos += n;
		}
	}
	
	char *res = gmk_alloc(newlen);
	strcpy(res, str);
	
	free(lens);
	free(str);
	return res;
}

int
callv_gmk_setup ()
{
	/* Register the function with make name "callv".  */
	gmk_add_function ("callv", callv, 1, 10, GMK_FUNC_DEFAULT);
	return 1;
}

