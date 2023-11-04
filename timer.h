// SPDX-License-Identifier: MIT

#ifndef LIST_RUNNER_TIMER
#define LIST_RUNNER_TIMER 1

#pragma once

#include <stdint.h>
#include <time.h>

////////////////////////////////////////

static inline uint64_t timer_nsec()
{
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    return ( uint64_t )( ts.tv_sec ) * ( uint64_t )( 1000000000 ) +
           ( uint64_t )( ts.tv_nsec );
}

#endif // LIST_RUNNER_TIMER
