// SPDX-License-Identifier: MIT

#include "sorted_array.h"
#include "timer.h"
#include <stdio.h>

static int sort_item_by_id( const game_item *a, const game_item *b )
{
    if ( a->id < b->id )
        return -1;
    if ( a->id == b->id )
        return 0;
    return 1;
}

static void resort_array( sorted_array_storage *a )
{
    int (*qsfunc)(const void *, const void *);
    qsfunc = (int (*)(const void *, const void *)) a->compare_func;
    qsort( a->_arr._buf, a->_arr.count, sizeof(game_item), qsfunc );
}

void sorted_array_init( sorted_array_storage *a, int scale, sorted_array_compare_func cf )
{
    array_init( &(a->_arr), scale );
    a->compare_func = cf;
}

void sorted_array_init_with_size(
    sorted_array_storage *a,
    int scale,
    sorted_array_compare_func cf,
    size_t num )
{
    array_init_with_size( &(a->_arr), scale, num );
    a->compare_func = cf;
}

void sorted_array_init_with_list(
    sorted_array_storage *a,
    int scale,
    sorted_array_compare_func cf,
    game_item *f,
    uint32_t num )
{
    array_init_with_list( &(a->_arr), scale, f, num );
    a->compare_func = cf;
    resort_array( a );
}

size_t sorted_array_get_storage_bytes( sorted_array_storage *a )
{
    // subtract off the array storage so it's not counted twice in re-using code
    return array_get_storage_bytes( &(a->_arr) ) + sizeof(sorted_array_storage) - sizeof(array_storage);
}

void sorted_array_destroy( sorted_array_storage *a )
{
    array_destroy( &(a->_arr) );
}

void sorted_array_assign_index( sorted_array_storage *a, size_t idx, game_item *i )
{
    // when using this, we can't sort uninitialized items, it requires user to
    // understand this and know to call the finish assign function...
    array_assign_index( &(a->_arr), idx, i );
}

void sorted_array_finish_assign( sorted_array_storage *a )
{
    resort_array( a );
}

game_item *sorted_array_get_index( sorted_array_storage *a, size_t idx )
{
    return array_get_index( &(a->_arr), idx );
}

void sorted_array_add_one( sorted_array_storage *a, game_item *i )
{
    array_add_one( &(a->_arr), i );
    // note the extra cost here of sorting again,
    // however because the list is already sorted, should be
    // on the faster side of the sort complexity
    resort_array( a );
}

void sorted_array_add_list( sorted_array_storage *a, game_item *i, uint32_t num )
{
    array_add_list( &(a->_arr), i, num );
    // note the extra cost here of sorting again,
    // however because the list is already sorted, should be
    // on the faster side of the sort complexity
    resort_array( a );
}

game_item *sorted_array_find_item( sorted_array_storage *a, uint32_t id )
{
    // the whole point of keeping the list sorted is we can then
    // have faster searches, so we override the search algorithm here

    // this assumes we are sorting by id...

    // look at binary search on wikipedia for details how this works
    
    size_t leftidx = 0;
    size_t rightidx = a->_arr.count - 1;

    if ( a->_arr.count == 0 )
    {
        return NULL;
    }

    while ( leftidx != rightidx )
    {
        size_t mid = ( leftidx + rightidx + 1 ) / 2;
        if ( a->_arr._buf[mid].id > id )
        {
            rightidx = mid - 1;
        }
        else
        {
            leftidx = mid;
        }
    }

    if ( a->_arr._buf[leftidx].id == id )
        return a->_arr._buf + leftidx;
    return NULL;
}

size_t sorted_array_find_items_along(
    sorted_array_storage *a,
    sorted_array_storage *result,
    const ray *r )
{
    printf( "TODO!\n" );
    return 0;
}

static void test_sorted_array_operations_capacity( uint32_t n, int scale )
{
    sorted_array_storage arr, arr2;
    uint64_t start, end;
    game_item curitem;
    uint32_t randitem;
    uint32_t failcount;
    
    printf( "  sorted_array(scale=%d): create empty\n", scale );
    sorted_array_init( &arr, scale, &sort_item_by_id );
    start = timer_nsec();
    for ( uint32_t i = 0; i < n; ++i )
    {
        make_random_item( &curitem, n - i );
        sorted_array_add_one( &arr, &curitem );
    }
    end = timer_nsec();
    printf( "  sorted_array(scale=%d): fill from empty: %lu\n", scale, end - start );
    
    printf( "  sorted_array(scale=%d): create with size\n", scale );
    sorted_array_init_with_size( &arr2, scale, &sort_item_by_id, n );
    start = timer_nsec();
    for ( uint32_t i = 0; i < n; ++i )
    {
        make_random_item( &(curitem), n - i );
        sorted_array_assign_index( &arr2, i, &curitem );
    }
    end = timer_nsec();
    printf( "  sorted_array(scale=%d): fill sized: %lu\n", scale, end - start );
    sorted_array_destroy( &arr2 );

    printf( "  sorted_array(scale=%d): create copy\n", scale );
    start = timer_nsec();
    sorted_array_init_with_list( &arr2, scale, &sort_item_by_id, arr._arr._buf, n );
    end = timer_nsec();
    printf( "  sorted_array(scale=%d): fill with existing list: %lu\n", scale, end - start );
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
    printf( "  sorted_array(scale=%d): time to find everything: \033[0;32m%lu\033[0m (ave %lu)\n", scale, end - start, ((end - start) + n - 1 ) / n );

    printf( "  sorted_array(scale=%d): memory cost: \033[1;31m%lu\033[0m\n", scale, sorted_array_get_storage_bytes( &arr ) );
    start = timer_nsec();
    sorted_array_destroy( &arr );
    end = timer_nsec();
    printf( "  sorted_array(scale=%d): time to destroy: %lu\n", scale, end - start );
}

void test_sorted_array_operations( uint32_t n )
{
    // scale algo is 1 meaning + 1 growth (slow, big n is bad)
    test_sorted_array_operations_capacity( n, 1 );
    // scale algo is 3 meaning *2 growth
    test_sorted_array_operations_capacity( n, 2 );
    // scale algo is 3 meaning *3/2 growth
    test_sorted_array_operations_capacity( n, 3 );
}

