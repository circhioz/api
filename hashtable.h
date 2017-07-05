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

#ifndef API_HASHTABLE_H
#define API_HASHTABLE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>

/****************************************************************************
* Public Types
****************************************************************************/
/* Hashtable entry */
typedef struct _hashtable_entry {
    char                *key;
    void                *value;
} hashtable_entry_t;

/* Hashtable */
typedef struct _hashtable {
    uint16_t            size;
    uint16_t            capacity;
    hashtable_entry_t   *body;
} hashtable_t;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

uint16_t hashtable_get_size(hashtable_t *);
hashtable_t *hashtable_create(void);
void *hashtable_get(hashtable_t *, char *);
void hashtable_set(hashtable_t *, char *, void *);
void hashtable_resize(hashtable_t *, uint16_t);
void hashtable_remove(hashtable_t *, char *);
void *hashtable_iterate(hashtable_t *, int *);
void hashtable_destroy(hashtable_t *);

#endif //API_HASHTABLE_H
