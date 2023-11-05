// SPDX-License-Identifier: MIT

#ifndef LIST_RUNNER_ARRAY_STRUCTURE
#define LIST_RUNNER_ARRAY_STRUCTURE 1

#pragma once

////////////////////////////////////////

#include <stdint.h>
#include <stddef.h>

#include "item.h"

typedef struct _array_storage_v1
{
    game_item *_buf;
    size_t count;
    size_t capacity;
    int capacity_scale;
} array_storage;

void array_init( array_storage *a, int scale );
void array_init_with_size( array_storage *a, int scale, size_t num );
void array_init_with_list( array_storage *a, int scale, game_item *f, size_t num );

size_t array_get_storage_bytes( array_storage *a );
void array_destroy( array_storage *a );

void array_assign_index( array_storage *a, size_t idx, game_item *i );

void array_delete_index( array_storage *a, size_t idx );

game_item *array_get_index( array_storage *a, size_t idx );

void array_add_one( array_storage *a, game_item *i );
void array_add_list( array_storage *a, game_item *i, size_t num );

game_item *array_find_item( array_storage *a, uint32_t id );
size_t array_find_items_along( array_storage *a, array_storage *result, const ray *r );

// main entry point
void test_array_operations( uint32_t n );

#endif // LIST_RUNNER_ARRAY_STRUCTURE
