// SPDX-License-Identifier: MIT

#include "array.h"
#include "timer.h"
#include <stdio.h>

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

void test_array_operations( uint32_t n )
{
    array_storage arr, arr2;
    uint64_t start, end;
    game_item curitem;
    uint32_t randitem;
    uint32_t failcount;
    
    printf( "  array: create empty\n" );
    array_init( &arr );
    start = timer_nsec();
    for ( uint32_t i = 0; i < n; ++i )
    {
        make_random_item( &(curitem), n - i );
        array_add_one( &arr, &curitem );
    }
    end = timer_nsec();
    printf( "  array: fill from empty: %lu\n", end - start );
    
    printf( "  array: create with size\n" );
    array_init_with_size( &arr, n );
    start = timer_nsec();
    for ( uint32_t i = 0; i < n; ++i )
    {
        make_random_item( &(curitem), n - i );
        array_assign_index( &arr, i, &curitem );
    }
    end = timer_nsec();
    printf( "  array: fill sized: %lu\n", end - start );
    array_destroy( &arr2 );

    printf( "  array: create copy\n" );
    start = timer_nsec();
    array_init_with_list( &arr2, arr._buf, n );
    end = timer_nsec();
    printf( "  array: fill with existing list: %lu\n", end - start );
    array_destroy( &arr2 );

    start = timer_nsec();
    failcount = 0;
    for ( uint32_t i = 0; i < n; ++i )
    {
        game_item *exist = array_find_item( &arr, i + 1 );
        if ( ! exist )
            ++failcount;
    }
    end = timer_nsec();
    if ( failcount > 0 )
    {
        printf( "ERROR: unable to find %u items\n", failcount );
    }
    printf( "  array: time to find everything: %lu (ave %lu)\n", end - start, ((end - start) + n - 1 ) / n );

    start = timer_nsec();
    array_destroy( &arr );
    end = timer_nsec();
    printf( "  array: time to destroy: %lu\n", end - start );
}

