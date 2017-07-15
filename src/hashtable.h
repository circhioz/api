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

#ifndef API_HASHTABLE_H
#define API_HASHTABLE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

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
bool hashtable_set(hashtable_t *, char *, void *);
void hashtable_resize(hashtable_t *, uint16_t);
void hashtable_remove(hashtable_t *, char *);
void *hashtable_iterate(hashtable_t *, size_t *);
void hashtable_destroy(hashtable_t *);

#endif //API_HASHTABLE_H
