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
 * Implements the MurmurHash3 hash function.
 */
uint64_t hashtable_hash(const char *key) {
    size_t len = strlen(key);
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;
    uint64_t h = 1023724138 ^ (len * m);
    const uint64_t * data = (const uint64_t *)key;
    const uint64_t * end = data + (len / 8);
    while (data != end)
    {
        uint64_t k = *data++;
        k *= m;
        k ^= k >> r;
        k *= m;
        h ^= k;
        h *= m;
    }
    const unsigned char * data2 = (const unsigned char*)data;
    switch (len & 7) {
        case 7: h ^= ((uint64_t) data2[6]) << 48;
        case 6: h ^= ((uint64_t) data2[5]) << 40;
        case 5: h ^= ((uint64_t) data2[4]) << 32;
        case 4: h ^= ((uint64_t) data2[3]) << 24;
        case 3: h ^= ((uint64_t) data2[2]) << 16;
        case 2: h ^= ((uint64_t) data2[1]) << 8;
        case 1: h ^= ((uint64_t) data2[0]);
            h *= m;
    };
    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return h;
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
bool hashtable_set(hashtable_t *t, char *key, void *value) {
    int index = hashtable_find_slot(t, key);
    if (t->body[index].key != NULL) {
        /* Entry exists; fail. */
        return false;
    } else {
        /* Create a new  entry */
        if ((float) (t->size + 1) / t->capacity > 0.8) {
            /* Resize the hash table */
            hashtable_resize(t, t->capacity * (uint16_t)2);
            index = hashtable_find_slot(t, key);
        }
        t->size = t->size + (uint16_t)1;
        t->body[index].key = key;
        t->body[index].value = value;
        return true;
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
            if ((next > idx && (next_base <= idx || next_base > next))
                || (next < idx && (next_base <= idx && next_base > next))) {
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
