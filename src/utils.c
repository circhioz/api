/* 
 * This file is part of SimpleFS, an API course project
 * Copyright (c) 2017 Francesco Circhetta.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "utils.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * malloc() wrapper: crash on memory allocation failure!
 */
inline void *malloc_or_die(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL)
        exit(-1);
    return ptr;
}

/**
 * calloc() wrapper: crash on memory allocation failure!
 */
inline void *calloc_or_die(size_t num, size_t size) {
    void *ptr = calloc(num, size);
    if (ptr == NULL)
        exit(-1);
    return ptr;
}

/**
 * realloc() wrapper: crash on memory allocation failure!
 */
inline void *realloc_or_die(void *block, size_t size) {
    void *ptr = realloc(block, size);
    if (ptr == NULL)
        exit(-1);
    return ptr;
}

/**
 * strdup() equivalent
 */
char *my_strdup(char *str) {
    char *new = 0;
    size_t len = strlen(str);
    new = calloc_or_die(len + 1, sizeof(char));
    strncpy(new, str, len);
    return new;
}

/**
 * Local getline() implementation (not available in C99)
 */
char *my_getline(void) {
    char *line = malloc_or_die(100 * sizeof(char)), *linep = line;
    size_t lenmax = 100, len = lenmax;
    for (;;) {
        int c = fgetc(stdin);
        if (c == EOF)
            break;
        if (--len == 0) {
            len = lenmax;
            char *linen = realloc_or_die(linep, lenmax *= 2);
            line = linen + (line - linep);
            linep = linen;
        }
        if ((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}
