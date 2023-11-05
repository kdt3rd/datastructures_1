// SPDX-License-Identifier: MIT

#ifndef LIST_RUNNER_SORTED_ARRAY_STRUCTURE
#define LIST_RUNNER_SORTED_ARRAY_STRUCTURE 1

#pragma once

#include "item.h"
#include "array.h"

typedef int (*sorted_array_compare_func)( const game_item *a, const game_item *b );

// this is basically the same as the array we've already studied
// but this time we'll keep the list sorted as we add items to it
typedef struct _sorted_array_storage_v1
{
    array_storage _arr;

    sorted_array_compare_func compare_func;
} sorted_array_storage;

void sorted_array_init( sorted_array_storage *a, int scale, sorted_array_compare_func cf );
void sorted_array_init_with_size( sorted_array_storage *a, int scale, sorted_array_compare_func cf, size_t num );
void sorted_array_init_with_list(
    sorted_array_storage *a,
    int scale,
    sorted_array_compare_func cf,
    game_item *f,
    uint32_t num );

size_t sorted_array_get_storage_bytes( sorted_array_storage *a );
void sorted_array_destroy( sorted_array_storage *a );

void sorted_array_assign_index( sorted_array_storage *a, size_t idx, game_item *i );
// HOWEVER: note we've had to add an api for when we are filling in a list otherwise
// we'd be sorting uninitialized values constantly when we've been initialized with a size
// but not yet given values...
void sorted_array_finish_assign( sorted_array_storage *a );

void sorted_array_delete_index( sorted_array_storage *a, size_t idx );

game_item *sorted_array_get_index( sorted_array_storage *a, size_t idx );

void sorted_array_add_one( sorted_array_storage *a, game_item *i );
void sorted_array_add_list( sorted_array_storage *a, game_item *i, uint32_t num );

game_item *sorted_array_find_item( sorted_array_storage *a, uint32_t id );
size_t sorted_array_find_items_along(
    sorted_array_storage *a,
    sorted_array_storage *result,
    const ray *r );

// main entry point
void test_sorted_array_operations( uint32_t n );

#endif // LIST_RUNNER_SORTED_ARRAY_STRUCTURE
