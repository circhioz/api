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
#include "strlist.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * Head insert string into the list
 */
strlist_t *strlist_head_insert(strlist_t *head, char *string) {
    strlist_t *old_head;
    old_head = head;
    head = malloc_or_die(sizeof(strlist_t));
    head->next = old_head;
    head->str = string;
    return head;
}

/**
 * Destroy list after use
 */
void strlist_destroy(strlist_t *head) {
    strlist_t *tmp;
    while (head != NULL) {
        tmp = head->next;
        free(head->str);
        free(head);
        head = tmp;
    }
}

/**
 * Sort list by lexicographical order (bubble sort)
 */
strlist_t *strlist_sort(strlist_t *head) {
    if (!head) {
        return NULL;
    }
    strlist_t *tmp_ptr = head;
    strlist_t *tmp_nxt = head->next;

    char *tmp;
    while (tmp_nxt != NULL) {
        while (tmp_nxt != tmp_ptr) {
            if (strcmp(tmp_ptr->str, tmp_nxt->str) > 0) {
                tmp = tmp_ptr->str;
                tmp_ptr->str = tmp_nxt->str;
                tmp_nxt->str = tmp;
            }
            tmp_ptr = tmp_ptr->next;
        }
        tmp_ptr = head;
        tmp_nxt = tmp_nxt->next;
    }
    return tmp_ptr;
}
