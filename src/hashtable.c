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
#include <string.h>

#include "utils.h"
#include "hashtable.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define HT_INITIAL_CAPACITY 32

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/**
 * Compute the hash value for the given string.
 * Implements the djb k=33 hash function.
 */
unsigned long hashtable_hash(char *str) {
    register unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

/**
 * Find an available slot for the given key, using linear probing.
 */
int hashtable_find_slot(hashtable_t *table, char *key) {
    int idx = hashtable_hash(key) % table->capacity;
    while (table->body[idx].key != NULL
           && strcmp(table->body[idx].key, key) != 0) {
        idx = (idx + 1) % table->capacity;
    }
    return idx;
}

/**
 * Allocate a new memory block with the given capacity.
 */
static inline hashtable_entry_t *hashtable_body_allocate(unsigned int capacity) {
    return (hashtable_entry_t *) calloc_or_die(capacity, sizeof(hashtable_entry_t));
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * Create a new, empty hashtable
 */
hashtable_t *hashtable_create(void) {
    hashtable_t *new_ht = malloc_or_die(sizeof(hashtable_t));
    new_ht->size = 0;
    new_ht->capacity = HT_INITIAL_CAPACITY;
    new_ht->body = hashtable_body_allocate(new_ht->capacity);
    return new_ht;
}

/**
 * Return the item associated with the given key, or NULL if not found.
 */
void *hashtable_get(hashtable_t *table, char *key) {
    int idx = hashtable_find_slot(table, key);
    return table->body[idx].key == NULL ? NULL : table->body[idx].value;
}

/**
 * Assign a value to the given key in the table.
 */
void hashtable_set(hashtable_t *t, char *key, void *value) {
    int index = hashtable_find_slot(t, key);
    if (t->body[index].key != NULL) {
        /* Entry exists; update it. */
        t->body[index].value = value;
    } else {
        /* Create a new  entry */
        if ((float) (t->size + 1) / t->capacity > 0.8) {
            /* Resize the hash table */
            hashtable_resize(t, t->capacity * 2);
            index = hashtable_find_slot(t, key);
        }
        t->size = t->size + 1;
        t->body[index].key = key;
        t->body[index].value = value;
    }
}

/**
 * Resize the allocated memory.
 * Warning: clears the table of all entries.
 */
void hashtable_resize(hashtable_t *t, uint16_t capacity) {
    uint16_t old_capacity = t->capacity;
    hashtable_entry_t *old_body = t->body;
    t->body = hashtable_body_allocate(capacity);
    t->size = 0;
    t->capacity = capacity;
    for (int i = 0; i < old_capacity; i++) {
        if (old_body[i].key != NULL) {
            hashtable_set(t, old_body[i].key, old_body[i].value);
        }
    }
    free(old_body);
}

/**
 * Remove a key from the table
 * The algoritm rearranges entries not to disrupt the probing sequence
 */
void hashtable_remove(hashtable_t *t, char *key) {
    int idx = hashtable_find_slot(t, key);
    if (t->body[idx].key != NULL) {
        int next = (idx + 1) % t->capacity;
        while (t->body[next].key != NULL) {
            int next_base = hashtable_hash(t->body[next].key) % t->capacity;
            if (next > idx && (next_base <= idx || next_base > next) ||
                next < idx && (next_base <= idx && next_base > next)) {
                t->body[idx].key = t->body[next].key;
                t->body[idx].value = t->body[next].value;
                idx = next;
            }
            next = (next + 1) % t->capacity;
        }
        t->body[idx].key = NULL;
        t->body[idx].value = NULL;
        t->size--;
    }
}

/**
 * Iterate through table entries (uses only an int as state memory)
 * Return NULL if no other element is present
 */
void *hashtable_iterate(hashtable_t *table, int *state) {
    register int local_state = *state;
    while (local_state < table->capacity) {
        hashtable_entry_t *entry;
        entry = &(table->body[local_state]);
        if (entry->key != NULL) {
            *state = local_state + 1;
            return entry->value;
        }
        local_state++;
    }
    return NULL;
}

/**
 * Destroy the table and deallocate it from memory. This does not deallocate the contained items.
 */
void hashtable_destroy(hashtable_t *t) {
    free(t->body);
    free(t);
}

/**
 * Return the number of used entries
 */
uint16_t hashtable_get_size(hashtable_t *table) {
    return table->size;
}
