// SPDX-License-Identifier: MIT

#include "array.h"
#include "timer.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

// def foo( a: vec3 ):
//     a.x = 3
//
// void foo( vec3 *a )
// {
//     a->x = 3;
//     (*a).x = 3;
//     *a.x = 3;
// }

// utility to grow the array
static void grow_array( array_storage *a, size_t num )
{
    size_t newcapacity = a->capacity;
    size_t newcount = a->count + num;

    while ( newcount > newcapacity )
    {
        switch ( a->capacity_scale )
        {
            case 2:
                // rather than super slow, we double the capacity every time
                // we run out of space, quickly becomes a memory hog
                // as the list grows (i.e. for 1000 items, has memory for 2000 things)
                // but this also really makes it faster to add items to the list
                // because most of the time it's going through the above path
                // where there is already memory
                newcapacity = newcapacity == 0 ? newcount : newcapacity * 2;
                break;
            case 3:
                // compromise: grow slightly slower, but still give plenty
                // of fast path above
                if ( newcapacity == 0 )
                    newcapacity = newcount;
                else
                {
                    size_t growth = ( newcapacity * 3 ) / 2;
                    if ( growth == newcapacity )
                        newcapacity *= 2;
                    else
                        newcapacity = growth;
                }
                break;
            case 1:
            default:
                // we grow a single at a time, causing O(n^2) growth, for every item
                // it has to visit every other item and duplicate... (SLOW)
                newcapacity = newcapacity + 1;
                break;
        }
    }

    if ( newcapacity > a->capacity )
    {
        game_item *newlist = malloc( sizeof(game_item) * newcapacity );
        if ( newlist == NULL )
        {
            printf( "ERROR: unable to allocate RAM\n" );
            exit( 1 );
        }

        if ( a->_buf )
        {
            memcpy( newlist, a->_buf, sizeof(game_item) * a->count );
            free( a->_buf );
        }
        a->_buf = newlist;
        a->capacity = newcapacity;
    }
}

void array_init( array_storage *a, int scale )
{
    a->_buf = NULL;
    a->count = 0;
    a->capacity = 0;
    a->capacity_scale = scale;
}

void array_init_with_size( array_storage *a, int scale, size_t num )
{
    array_init( a, scale );
    grow_array( a, num );

    // leave the items uninitialized, but indicate they are there...
    a->count = num;
}

void array_init_with_list( array_storage *a, int scale, game_item *f, size_t num )
{
    array_init( a, scale );
    grow_array( a, num );

    // leave the items uninitialized, but indicate they are there...
    for ( size_t i = 0; i < num; ++i )
        a->_buf[i] = f[i];
    //memcpy( a->_buf, f, sizeof(game_item) * num );
    a->count = num;
}

size_t array_get_storage_bytes( array_storage *a )
{
    return a->capacity * sizeof(game_item) + sizeof(array_storage);
}

void array_destroy( array_storage *a )
{
    if ( a->_buf )
        free( a->_buf );
    array_init( a, a->capacity_scale );
}

void array_assign_index( array_storage *a, size_t idx, game_item *i )
{
    if ( idx < a->count )
        a->_buf[idx] = *i;
}

void array_delete_index( array_storage *a, size_t idx )
{
    if ( idx < a->count )
    {
        if ( a->capacity_scale == 1 )
        {
            size_t newcapacity = a->count - 1;
            game_item *newlist = malloc( sizeof(game_item) * newcapacity );
            for ( size_t i = 0; i < idx; ++i )
                newlist[i] = a->_buf[i];
            // skip the one we're deleting by grabbing the item past it
            for ( size_t i = idx; i < newcapacity; ++i )
                newlist[i] = a->_buf[i+1];
            free( a->_buf );
            a->_buf = newlist;
            a->count = newcapacity;
            a->capacity = newcapacity;
        }
        else
        {
            // need to shift all the following entries back one..
            // note that deleting from the beginning of the list is far
            // more expensive than deleting from the end because you have to visit
            // however many items are after the index to delete
            size_t cm1 = a->count - 1;
            for ( size_t i = idx; i < cm1; ++i )
                a->_buf[i] = a->_buf[i+1];
            a->count = a->count - 1;
            // we can leave the capacity alone
        }
    }
}

game_item *array_get_index( array_storage *a, size_t idx )
{
    if ( idx < a->count )
        return a->_buf + idx;
    return NULL;
}

void array_add_one( array_storage *a, game_item *i )
{
    grow_array( a, 1 );

    a->_buf[a->count] = *i;
    a->count = a->count + 1;
}

void array_add_list( array_storage *a, game_item *ilist, size_t num )
{
    grow_array( a, num );

    for ( size_t i = 0; i < num; ++i )
        a->_buf[a->count + i] = ilist[i];
    //memcpy( a->_buf + a->count, i, sizeof(game_item) * num );
    a->count = a->count + num;
}

game_item *array_find_item( array_storage *a, uint32_t id )
{
    for ( size_t i = 0; i < a->count; ++i )
    {
        if ( a->_buf[i].id == id )
            return a->_buf + i;
    }
    return NULL;
}

size_t array_find_items_along( array_storage *a, array_storage *result, const ray *r )
{
    printf( "TODO!\n" );
    return 0;
}

static void test_array_operations_capacity( uint32_t n, int scale )
{
    array_storage arr, arr2;
    uint64_t start, end;
    game_item curitem;
    uint32_t randitem;
    uint32_t failcount;
    
    printf( "  array(scale=%d): create empty\n", scale );
    array_init( &arr, scale );
    start = timer_nsec();
    for ( uint32_t i = 0; i < n; ++i )
    {
        make_random_item( &curitem, n - i );
        array_add_one( &arr, &curitem );
    }
    end = timer_nsec();
    printf( "  array(scale=%d): fill from empty: %lu\n", scale, end - start );
    
    printf( "  array(scale=%d): create with size\n", scale );
    array_init_with_size( &arr2, scale, n );
    start = timer_nsec();
    for ( uint32_t i = 0; i < n; ++i )
    {
        make_random_item( &(curitem), n - i );
        array_assign_index( &arr2, i, &curitem );
    }
    end = timer_nsec();
    printf( "  array(scale=%d): fill sized: %lu\n", scale, end - start );
    array_destroy( &arr2 );

    printf( "  array(scale=%d): create copy\n", scale );
    start = timer_nsec();
    array_init_with_list( &arr2, scale, arr._buf, n );
    end = timer_nsec();
    printf( "  array(scale=%d): fill with existing list: %lu\n", scale, end - start );

    start = timer_nsec();
    array_delete_index( &arr2, n / 4 );
    array_delete_index( &arr2, n / 2 );
    array_delete_index( &arr2, ( n * 3 ) / 4 );
    end = timer_nsec();
    printf( "  array(scale=%d): delete 3 items: %lu\n", scale, end - start );

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
    printf( "  array(scale=%d): time to find everything: %lu (ave %lu)\n", scale, end - start, ((end - start) + n - 1 ) / n );

    printf( "  array(scale=%d): memory cost: \033[1;31m%lu\033[0m\n", scale, array_get_storage_bytes( &arr ) );
    start = timer_nsec();
    array_destroy( &arr );
    end = timer_nsec();
    printf( "  array(scale=%d): time to destroy: %lu\n", scale, end - start );
}

void test_array_operations( uint32_t n )
{
    // scale algo is 1 meaning + 1 growth (slow, big n is bad)
    test_array_operations_capacity( n, 1 );
    // scale algo is 3 meaning *2 growth
    test_array_operations_capacity( n, 2 );
    // scale algo is 3 meaning *3/2 growth
    test_array_operations_capacity( n, 3 );
}

