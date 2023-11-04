// SPDX-License-Identifier: MIT

#include "sorted_array.h"

void sorted_array_init( sorted_array_storage *a, sorted_array_compare_func cf )
{
}

void sorted_array_init_with_list(
    sorted_array_storage *a,
    sorted_array_compare_func cf,
    game_item *f,
    uint32_t num )
{
}

void sorted_array_destroy( sorted_array_storage *a )
{
}

void sorted_array_assign_index( sorted_array_storage *a, size_t idx, game_item *i )
{
}

game_item *sorted_array_get_index( sorted_array_storage *a, size_t idx )
{
    return NULL;
}

void sorted_array_add_one( sorted_array_storage *a, game_item *i )
{
}

void sorted_array_add_list( sorted_array_storage *a, game_item *i, uint32_t num )
{
}

game_item *sorted_array_find_item( sorted_array_storage *a, uint32_t id )
{
    return NULL;
}

size_t sorted_array_find_items_along(
    sorted_array_storage *a,
    sorted_array_storage *result,
    const ray *r )
{
    return 0;
}

