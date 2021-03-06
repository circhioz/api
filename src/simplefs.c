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
#include <string.h>

#include "simplefs.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/**
 * Create and return a new string with node full path
 */
char *fs_get_path(node_t *node, size_t len) {
    char *path;
    /* Root? Alloc path array */
    if (node->parent != NULL) {
        path = fs_get_path(node->parent, len + strlen(node->name) + 1);
        strcat(path, "/");
        strcat(path, node->name);
    } else {
        path = calloc_or_die(len + 1, sizeof(char));
        if (len == 0)
            strcat(path, "/");
    }
    return path;
}

/**
 * Get node type, Dir or File
 */
uint8_t fs_get_type(node_t *node) {
    return node->type;
}

/**
 * Get file content
 */
char *fs_get_file_content(node_t *node) {
    if (node->type != File) {
        /* This isn't a file */
        return NULL;
    }
    return node->payload.content;
}

/**
 * Assign new content to a file
 * Return true if succeeded, false if failed
 */
bool fs_set_file_content(node_t *node, char *new_content) {
    if (fs_get_type(node) != File) {
        /* This isn't a file */
        return false;
    }
    /* Free the old content */
    free(node->payload.content);
    /* Duplicate the new content */
    node->payload.content = my_strdup(new_content);
    return true;
}

/**
 * Get a node by name from a specific directory, return NULL if not found
 */
node_t *fs_find_in_dir(node_t *parent, char *key) {
    /* Get node from dir hashtable */
    return hashtable_get(parent->payload.dirhash, key);
}

/**
 * Create new empty file or dir in a specific directory
 * Return true if succeeded, false if failed
 */
bool fs_create(node_t *parent, char *key, uint8_t type) {
    if (hashtable_get_size(parent->payload.dirhash) >= MAX_NODES /* Dir is full */
        || strlen(key) > MAX_NAMELENGHT /* Name is too long */
        || parent->depth >= MAX_DEPTH) /* Parent node is at max depth */
        return false;
    /* Create a new empty resource */
    node_t *child = malloc_or_die(sizeof(node_t));
    child->name = my_strdup(key);
    if (hashtable_set(parent->payload.dirhash, child->name, child)) {
        child->depth = parent->depth + (uint16_t)1;
        child->parent = parent;
        child->type = type;
        if (type == Dir) {
            // Empty DirHash
            child->payload.dirhash = hashtable_create();
        } else {
            // Empty content
            child->payload.content = calloc_or_die(1, sizeof(char));
        }
        return true;
    }
    free(child->name);
    free(child);
    return false;
}

/**
 * Delete a resource (also recursively)
 */
bool fs_delete(node_t *node, bool recursive) {
    /* If dir is not empty, delete every child */
    if (node->type == Dir) {
        if(hashtable_get_size(node->payload.dirhash) > 0) {
            /* Recursion disabled? Dir is not empty! */
            if (!recursive) return false;
            /* Iterate through the table */
            do {
                size_t state = 0;
                node_t *child = hashtable_iterate(node->payload.dirhash, &state);
                while (child) {
                    fs_delete(child, true);
                    child = hashtable_iterate(node->payload.dirhash, &state);
                }
            } while (hashtable_get_size(node->payload.dirhash) > 0);
        }
        hashtable_destroy(node->payload.dirhash);
    } else {
        free(node->payload.content);
    }
    hashtable_remove(node->parent->payload.dirhash, node->name);
    free(node->name);
    free(node);
    return true;
}

/**
 * Create a new root directory
 */
node_t *fs_new_root(void) {
    node_t *root;
    root = malloc_or_die(sizeof(node_t));
    root->name = calloc_or_die(1, sizeof(char));
    root->depth = 0;
    root->parent = NULL;
    root->type = Dir;
    root->payload.dirhash = hashtable_create();
    return root;
}

/**
 * Destroy the root directory
 */
void fs_destroy_root(node_t *root) {
    hashtable_destroy(root->payload.dirhash);
    free(root->name);
    free(root);
}

/**
 * Find resources recursively given a starting directory
 */
node_t **fs_find_r(node_t *node, char *name, size_t *num, node_t **array) {
    size_t state = 0; // Iterator state
    node_t *child = hashtable_iterate(node->payload.dirhash, &state);
    while (child) {
        if (strcmp(child->name, name) == 0) {
            /* We found a node with the requested name */
            *num = *num + 1;
            array = (array == NULL) ? malloc_or_die(sizeof(node_t *))
                                    : realloc_or_die(array, (*num) * sizeof(node_t *));
            array[*num - 1] = child;
        }
        /* Check subdirs */
        if (child->type == Dir) {
            array = fs_find_r(child, name, num, array);
        }
        child = hashtable_iterate(node->payload.dirhash, &state);
    }
    return array;
}
