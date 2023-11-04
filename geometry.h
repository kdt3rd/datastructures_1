// Copyright (c) 2023 Kimball Thurston
// SPDX-License-Identifier: MIT

#ifndef LIST_RUNNER_GEOMETRY
#define LIST_RUNNER_GEOMETRY 1

#pragma once

#include <float.h>
#include <math.h>

////////////////////////////////////////

typedef struct _position_vector_v1
{
    float x;
    float y;
    float z;
} vec3;

////////////////////////////////////////

typedef struct _axis_aligned_bound_box_v1
{
    vec3 _min;
    vec3 _max;
} aabb;

////////////////////////////////////////

typedef struct _ray_v1
{
    vec3 _origin;
    vec3 _dir;
} ray;

static inline float vector_length( vec3 *v )
{
    return sqrtf( v->x * v->x + v->y * v->y + v->z * v->z );
}

static inline void normalize_vector( vec3 *v )
{
    float l = vector_length( v );
    if ( l > 0.f )
    {
        v->x /= l;
        v->y /= l;
        v->z /= l;
    }
}

static inline int ray_box_process_point( float o, float d, float bmin, float bmax, float *t_near, float *t_far )
{
    if ( d == 0.f )
    {
        if ( o < bmin || o > bmax )
        {
            return 0;
        }
    }
    else
    {
        float t1 = ( bmin - o ) / d;
        float t2 = ( bmax - o ) / d;

        if ( t1 > t2 )
        {
            float t = t1;
            t1 = t2;
            t2 = t;
        }

        if ( t1 > *t_near )
        {
            *t_near = t1;
        }
        if ( t2 < *t_far )
        {
            *t_far = t2;
        }
        if ( ( *t_near > *t_far ) || ( *t_far < 0.f ) )
        {
            // no intersection or the box is behind the origin
            return 0;
        }
    }
    return 1;
}

static inline int ray_intersect_box( const aabb *box, const ray *r, float *t_near, float *t_far )
{
    // distance from origin of ray where intersection occurs
    *t_near = -FLT_MAX;
    *t_far = FLT_MAX;

    int rval;

    rval = ray_box_process_point( r->_origin.x, r->_dir.x, box->_min.x, box->_max.x, t_near, t_far );
    if ( rval )
        rval = ray_box_process_point( r->_origin.y, r->_dir.y, box->_min.y, box->_max.y, t_near, t_far );
    if ( rval )
        rval = ray_box_process_point( r->_origin.y, r->_dir.y, box->_min.y, box->_max.y, t_near, t_far );

    return rval;
    // faster but does not handle coordinate aligned ray!
    //float dirinvx = 1.f / r->_dir.x;
    //float dirinvy = 1.f / r->_dir.y;
    //float dirinvz = 1.f / r->_dir.z;
    //
    //float t1 = (box->_min.x - r->_origin.x)*dirinvx;
    //float t2 = (box->_max.x - r->_origin.x)*dirinvx;
    //float t3 = (box->_min.y - r->_origin.y)*dirinvy;
    //float t4 = (box->_max.y - r->_origin.y)*dirinvy;
    //float t5 = (box->_min.z - r->_origin.z)*dirinvz;
    //float t6 = (box->_max.z - r->_origin.z)*dirinvz;
    //
    //float tmin = fmaxf(fmaxf(fminf(t1, t2), fminf(t3, t4)), fminf(t5, t6));
    //float tmax = fminf(fminf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));
    //
    //// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
    //if (tmax < 0.f)
    //    return 0;
    //
    //// if tmin > tmax, ray doesn't intersect AABB
    //if (tmin > tmax)
    //    return 0;
    //
    //return 1;
}

#endif // LIST_RUNNER_GEOMETRY
