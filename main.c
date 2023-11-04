// SPDX-License-Identifier: MIT

#include "array.h"
#include "sorted_array.h"
#include "map.h"
#include "hash_map.h"
#include "timer.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline void make_box_pair( float *a, float *b )
{
    *a = drand48();
    *b = drand48();
    if ( *a > *b )
    {
        float t = *a;
        *a = *b;
        *b = t;
    }
}

static inline void make_random_box( aabb *b )
{
    make_box_pair( &b->_min.x, &b->_max.x );
    make_box_pair( &b->_min.y, &b->_max.y );
    make_box_pair( &b->_min.z, &b->_max.z );
}

static void test_normal_array( uint32_t n )
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
        curitem.id = i;
        make_random_box( &(curitem.bounds) );
        array_add_one( &arr, &curitem );
    }
    end = timer_nsec();
    printf( "  array: fill from empty: %lu\n", end - start );
    
    printf( "  array: create with size\n" );
    array_init_with_size( &arr, n );
    start = timer_nsec();
    for ( uint32_t i = 0; i < n; ++i )
    {
        curitem.id = n - i;
        make_random_box( &(curitem.bounds) );
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

static void test_sorted_array( size_t n )
{
}

static void test_map( size_t n )
{
}

static void test_hash_map( size_t n )
{
}

int main( int argc, char *argv[] )
{
    uint32_t listcount = 1024*1024;
    if ( argc > 1 )
        listcount = atoi( argv[1] );
    printf( "testing lists with %u items\n", listcount );

    test_normal_array( listcount );
    test_sorted_array( listcount );
    test_map( listcount );
    test_hash_map( listcount );
    
    return 0;
}
