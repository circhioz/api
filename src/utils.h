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

#ifndef API_UTILS_H
#define API_UTILS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdlib.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void *malloc_or_die(size_t);
void *calloc_or_die(size_t, size_t);
char *my_strdup(char *);
char *my_getline(void);

#endif //API_UTILS_H
