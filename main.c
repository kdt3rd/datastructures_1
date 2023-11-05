// SPDX-License-Identifier: MIT

#include "array.h"
#include "sorted_array.h"
#include "map.h"
#include "hash_map.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] )
{
    uint32_t listcount = 1024*1024;
    if ( argc > 1 )
        listcount = atoi( argv[1] );
    printf( "testing lists with %u items\n", listcount );

    printf( "\n testing normal array\n" );
    test_array_operations( listcount );
    printf( "\n testing sorted array\n" );
    test_sorted_array_operations( listcount );
    printf( "\n testing map array\n" );
    test_map_operations( listcount );
    printf( "\n testing hash map array\n" );
    test_hash_map_operations( listcount );
    
    return 0;
}
