// Copyright (c) 2023 Kimball Thurston
// SPDX-License-Identifier: MIT

#ifndef LIST_RUNNER_GAME_ITEM
#define LIST_RUNNER_GAME_ITEM 1

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "geometry.h"

////////////////////////////////////////

typedef struct _game_item_v1
{
    uint32_t id;
    aabb bounds;
} game_item;

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

static inline void make_random_item( game_item *gi, uint32_t id )
{
    gi->id = id;
    make_random_box( &(gi->bounds) );
}

#endif // LIST_RUNNER_GAME_ITEM
