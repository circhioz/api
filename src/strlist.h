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

#ifndef API_STRLIST_H
#define API_STRLIST_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* List of strings */
typedef struct _strlist {
    char                *str;
    struct _strlist     *next;
} strlist_t;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
strlist_t *strlist_head_insert(strlist_t *, char *);
strlist_t *strlist_sort(strlist_t *);
void strlist_destroy(strlist_t *);

#endif //API_STRLIST_H
