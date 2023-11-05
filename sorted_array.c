// SPDX-License-Identifier: MIT

#include "sorted_array.h"
#include "timer.h"
#include <stdio.h>

static int sort_item_by_id( game_item *a, game_item *b )
{
    if ( a->id < b->id )
        return -1;
    if ( a->id == b->id )
        return 0;
    return 1;
}

void sorted_array_init( sorted_array_storage *a, sorted_array_compare_func cf )
{
}

void sorted_array_init_with_size(
    sorted_array_storage *a,
    sorted_array_compare_func cf,
    size_t num )
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

void test_sorted_array_operations( uint32_t n )
{
    sorted_array_storage arr, arr2;
    uint64_t start, end;
    game_item curitem;
    uint32_t randitem;
    uint32_t failcount;
    
    printf( "  sorted_array: create empty\n" );
    sorted_array_init( &arr, &sort_item_by_id );
    start = timer_nsec();
    for ( uint32_t i = 0; i < n; ++i )
    {
        make_random_item( &(curitem), n - i );
        sorted_array_add_one( &arr, &curitem );
    }
    end = timer_nsec();
    printf( "  sorted_array: fill from empty: %lu\n", end - start );
    
    printf( "  sorted_array: create with size\n" );
    sorted_array_init_with_size( &arr, &sort_item_by_id, n );
    start = timer_nsec();
    for ( uint32_t i = 0; i < n; ++i )
    {
        make_random_item( &(curitem), n - i );
        sorted_array_assign_index( &arr, i, &curitem );
    }
    end = timer_nsec();
    printf( "  sorted_array: fill sized: %lu\n", end - start );
    sorted_array_destroy( &arr2 );

    printf( "  sorted_array: create copy\n" );
    start = timer_nsec();
    sorted_array_init_with_list( &arr2, &sort_item_by_id, arr._buf, n );
    end = timer_nsec();
    printf( "  sorted_array: fill with existing list: %lu\n", end - start );
    sorted_array_destroy( &arr2 );

    start = timer_nsec();
    failcount = 0;
    for ( uint32_t i = 0; i < n; ++i )
    {
        game_item *exist = sorted_array_find_item( &arr, i + 1 );
        if ( ! exist )
            ++failcount;
    }
    end = timer_nsec();
    if ( failcount > 0 )
    {
        printf( "ERROR: unable to find %u items\n", failcount );
    }
    printf( "  sorted_array: time to find everything: %lu (ave %lu)\n", end - start, ((end - start) + n - 1 ) / n );

    start = timer_nsec();
    sorted_array_destroy( &arr );
    end = timer_nsec();
    printf( "  sorted_array: time to destroy: %lu\n", end - start );
}

