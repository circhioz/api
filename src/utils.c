/*
 * Copyright 2017 Francesco Circhetta
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "utils.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MIN_CHUNK 64

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
int my_getline(char **line, size_t *len) {
    if(*line == NULL) {
        *len = MIN_CHUNK;
        *line = malloc_or_die(MIN_CHUNK * sizeof(char));
    }
    size_t n_avail = *len;
    char *read_pos = *line;
    for(;;) {
        register int c = fgetc(stdin);
        /* We always want at least one char left in the buffer, since we
         * always NULL-terminate the line buffer.  */
        if (n_avail < 2) {
            *len = (*len > MIN_CHUNK) ? *len * 2 : *len + MIN_CHUNK;
            n_avail = *len + *line - read_pos;
            *line = realloc_or_die(*line, *len);
            read_pos = *len - n_avail + *line;
        }
        if (c == EOF) {
            /* Return partial line, if any.  */
            if (read_pos == *line)
                return -1;
            else
                break;
        }
        *read_pos++ = (char)c;
        n_avail--;
        if (c == '\n')
            /* Return the line.  */
            break;
    }
    /* Done - NUL terminate and return the number of chars read.  */
    *read_pos = '\0';
    return read_pos - (*line);
}

/**
 * Compare two strings using strcmp and return the result
 * Used as compare function for qsort
 */
int compare_str(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}