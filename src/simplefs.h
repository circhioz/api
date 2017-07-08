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

#ifndef API_SIMPLEFS_H
#define API_SIMPLEFS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "hashtable.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MAX_NODES 1024
#define MAX_NAMELENGHT 255
#define MAX_DEPTH 255

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* Node type */
enum {
    Dir,
    File,
};

typedef union {
    hashtable_t         *dirhash;
    char                *content;
} node_data_u;

/* FS tree node */
typedef struct _node {
    char                *name;
    struct _node        *parent;
    node_data_u         payload;
    uint8_t             type;
    uint16_t            depth;
} node_t;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
char *fs_get_path(node_t *, size_t);
char *fs_get_file_content(node_t *);
uint8_t fs_get_type(node_t *);
int fs_set_file_content(node_t *, char *);
int fs_create(node_t *, char *, uint8_t);
int fs_delete(node_t *, bool);
void fs_destroy_root(node_t *);
node_t **fs_find_r(node_t *, char *, size_t *, node_t **);
node_t *fs_find_in_dir(node_t *, char *);
node_t *fs_new_root(void);

#endif //API_SIMPLEFS_H
