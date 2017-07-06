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

/**
 * @file main.c
 * @author Francesco Circhetta
 * @date 22 June 2017
 * @brief SimpleFS: progetto di Algoritmi e Principi dell'Informatica
 * @see http://home.deib.polimi.it/barenghi/lib/exe/fetch.php?media=teaching:api:consegna.pdf
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "simplefs.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/**
 * Find resource by its path string
 */
node_t *enter_path(node_t *node, char *path, char **new_name) {
    node_t *tmp = NULL;
    char *cur_token = strtok(path, " /\n\r");
    char *next_token = strtok(NULL, "/\n\r");
    if (cur_token == NULL)
        return NULL; /* Empty path */
    /* Try to enter the path token by token */
    while (cur_token) {
        /* Enter only if current node is a dir */
        if (fs_get_type(node) == Dir) {
            if ((tmp = fs_find_in_dir(node, cur_token))) {
                /* Resource found, get next token */
                node = tmp;
                cur_token = next_token;
                next_token = strtok(NULL, "/\n\r");
            } else {
                /* Resource not found */
                if (new_name != NULL && next_token == NULL) {
                    *new_name = cur_token;
                    break;
                }
                return NULL;
            }
        } else {
            return NULL;
        }
    }
    return node;
}

/**
 * create <path>
 * create_dir <path>
 * Create a new empty file/directory
 */
int do_create(node_t *node, uint8_t type) {
    char *name = NULL;
    node = enter_path(node, NULL, &name);
    if (name != NULL)
        return fs_create(node, name, type);

    /* File exists or invalid path */
    return -1;
}

/**
 * read <path>
 * Read file content
 */
char *do_read(node_t *node) {
    node = enter_path(node, NULL, NULL);
    if (node == NULL)
        return NULL;

    return fs_get_file_content(node);
}

/**
 * write <path> "<content>"
 * Write the whole file content
 */
int do_write(node_t *node) {
    char *path, *new_content;

    path = strtok(NULL, " \n\r"); /* First token is path */
    new_content = strtok(NULL, "\"\n\r"); /* Second token is content */

    node = enter_path(node, path, NULL);
    if (node == NULL || new_content == NULL)
        return -1;

    if (fs_set_file_content(node, new_content) == 0)
        return (int) strlen(new_content);
    else
        return -1;
}

/**
 * delete <path>
 * delete_r <path>
 * Delete a resource (also recursively)
 */
int do_delete(node_t *node, int recursive) {
    node = enter_path(node, NULL, NULL);
    if (node == NULL)
        return -1;

    if (recursive) {
        fs_delete_r(node);
        return 0;
    } else {
        return fs_delete(node);
    }
}

/**
 * find <name>
 * Find a resource in the entire FS
 */
void do_find(node_t *root) {
    char *token = strtok(NULL, " \n\r");
    size_t nres = 0;
    node_t **res = fs_find_r(root, token, &nres, NULL);
    if(nres > 0) {
        char **paths = malloc_or_die(nres * sizeof(char *));
        for (int i = 0; i < nres; i++) {
            paths[i] = fs_get_path(res[i]);
        }
        free(res);
        qsort(paths, nres, sizeof(char *), compare_str);
        for(int i = 0; i < nres; i++) {
            printf("ok %s\n", paths[i]);
            free(paths[i]);
        }
        free(paths);
    } else {
        printf("no\n");
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int main() {
    char *line = NULL;
    node_t *root = fs_new_root(); /* Root node init */

    /* Command parser */
    line = my_getline();
    while (*line != '\0') {
        char *token;

        token = strtok(line, " \n\r");
        if (!token) {
            /* Empty line, skip */
        } else if (strcmp(token, "create") == 0) {
            if (do_create(root, File) == 0) {
                printf("ok\n");
            } else {
                printf("no\n");
            }
        } else if (strcmp(token, "create_dir") == 0) {
            if (do_create(root, Dir) == 0) {
                printf("ok\n");
            } else {
                printf("no\n");
            }
        } else if (strcmp(token, "read") == 0) {
            char *content;
            content = do_read(root);
            if (content) {
                printf("contenuto %s\n", content);
            } else {
                printf("no\n");
            }
        } else if (strcmp(token, "write") == 0) {
            int written = do_write(root);
            if (written > 0) {
                printf("ok %d\n", written);
            } else {
                printf("no\n");
            }
        } else if (strcmp(token, "delete") == 0) {
            if (do_delete(root, 0) == 0) {
                printf("ok\n");
            } else {
                printf("no\n");
            }
        } else if (strcmp(token, "delete_r") == 0) {
            if (do_delete(root, 1) == 0) {
                printf("ok\n");
            } else {
                printf("no\n");
            }
        } else if (strcmp(token, "find") == 0) {
            do_find(root);
        } else if (strcmp(token, "exit") == 0) {
            break;
        }

        free(line);
        line = my_getline();
    }

    free(line);
    hashtable_destroy(root->payload.dirhash);
    free(root->name);
    free(root);

    return 0;
}
