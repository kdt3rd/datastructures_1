// SPDX-License-Identifier: MIT

#include "array.h"

void array_init( array_storage *a )
{
}

void array_init_with_size( array_storage *a, size_t num )
{
}

void array_init_with_list( array_storage *a, game_item *f, size_t num )
{
}

void array_destroy( array_storage *a )
{
}

void array_assign_index( array_storage *a, size_t idx, game_item *i )
{
}

game_item *array_get_index( array_storage *a, size_t idx )
{
    return NULL;
}

void array_add_one( array_storage *a, game_item *i )
{
}

void array_add_list( array_storage *a, game_item *i, size_t num )
{
}

game_item *array_find_item( array_storage *a, uint32_t id )
{
    return NULL;
}

size_t array_find_items_along( array_storage *a, array_storage *result, const ray *r )
{
    return 0;
}