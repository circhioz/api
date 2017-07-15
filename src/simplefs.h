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
bool fs_set_file_content(node_t *, char *);
bool fs_create(node_t *, char *, uint8_t);
bool fs_delete(node_t *, bool);
void fs_destroy_root(node_t *);
node_t **fs_find_r(node_t *, char *, size_t *, node_t **);
node_t *fs_find_in_dir(node_t *, char *);
node_t *fs_new_root(void);

#endif //API_SIMPLEFS_H
