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

#include "simplefs.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/**
 * Free file node and delete directory entry
 */
void fs__free(node_t *node) {
    /* Free: payload, name, node struct
     * Remove: entry in parent hashtable */
    free(node->payload.content);
    hashtable_remove(node->parent->payload.dirhash, node->name);
    free(node->name);
    free(node);
}

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
 * Return 0 if succeeded, -1 if failed
 */
int fs_set_file_content(node_t *node, char *new_content) {
    if (fs_get_type(node) != File) {
        /* This isn't a file */
        return -1;
    }
    /* Free the old content */
    free(node->payload.content);
    /* Duplicate the new content */
    node->payload.content = my_strdup(new_content);
    return 0;
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
 * Return 0 if succeeded, -1 if failed
 */
int fs_create(node_t *parent, char *key, uint8_t type) {
    if (hashtable_get_size(parent->payload.dirhash) >= MAX_NODES /* Dir is full */
        || strlen(key) > MAX_NAMELENGHT /* Name is too long */
        || parent->depth >= MAX_DEPTH) /* Parent node is at max depth */
        return -1;
    /* Create a new empty resource */
    node_t *child = malloc_or_die(sizeof(node_t));
    child->name = my_strdup(key);
    if (hashtable_set(parent->payload.dirhash, child->name, child) == true) {
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
        return 0;
    }
    free(child->name);
    free(child);
    return -1;
}

/**
 * Delete a resource (also recursively)
 */
int fs_delete(node_t *node, bool recursive) {
    /* If dir is not empty, delete every child */
    if (node->type == Dir
        && hashtable_get_size(node->payload.dirhash) > 0) {
        /* Recursion disabled? Dir is not empty! */
        if (recursive == false) return -1;
        /* Iterate through the table */
        do {
            int state = 0;
            node_t *child = hashtable_iterate(node->payload.dirhash, &state);
            while (child) {
                fs_delete(child, true);
                child = hashtable_iterate(node->payload.dirhash, &state);
            }
        } while(hashtable_get_size(node->payload.dirhash) > 0);

    }
    fs__free(node);
    return 0;
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
    int state = 0; // Iterator state
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
