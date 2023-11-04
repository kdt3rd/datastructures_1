// Copyright (c) 2023 Kimball Thurston
// SPDX-License-Identifier: MIT

#ifndef LIST_RUNNER_GAME_ITEM
#define LIST_RUNNER_GAME_ITEM 1

#pragma once

#include <stdint.h>
#include <stddef.h>

#include "geometry.h"

////////////////////////////////////////

typedef struct _game_item_v1
{
    uint32_t id;
    aabb bounds;
} game_item;

#endif // LIST_RUNNER_GAME_ITEM
