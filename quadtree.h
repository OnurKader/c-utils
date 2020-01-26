#ifndef QUADTREE_H
#define QUADTREE_H

#include "vec.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define center centre

#ifndef BUCKET_SIZE
#	define BUCKET_SIZE 4
#endif

float dist(float a, float b, float c, float d)
{
	return sqrtf((a - c) * (a - c) + (b - d) * (b - d));
}

typedef struct Point
{
	float x, y;
} Point;

typedef struct Rect
{
	Point origin;
	Point center;
	float w, h;
} Rect;

typedef vec_t(Point) vec_p_t;

bool pointInRect(const Point* const p, const Rect* const r)
{
	return ((p->x >= r->origin.x) && (p->x < r->origin.x + r->w) &&
			(p->y >= r->origin.y) && (p->y < r->origin.y + r->h));
}

bool intersects(const Rect* const r1, const Rect* const r2)
{
	if(r1->origin.x + r1->w < r2->origin.x)	   // r1 is left of r2
		return false;
	if(r1->origin.x > r2->origin.x + r2->w)	   // r1 is right of r2
		return false;

	if(r1->origin.y + r1->h < r2->origin.y)	   // r1 is above r2
		return false;
	if(r1->origin.y > r2->origin.y + r2->h)	   // r1 is below r2
		return false;

	return true;
}

Point makePoint(float x, float y) { return (Point){x, y}; }

Rect makeRect(const float x, const float y, const float w, const float h)
{
	Rect temp;
	temp.origin.x = x;
	temp.origin.y = y;
	temp.w = w;
	temp.h = h;
	temp.center.x = x + w / 2.f;
	temp.center.y = y + h / 2.f;
	return temp;
}

#define getCentreOfRect getCenterOfRect

Point getCenterOfRect(Rect* const rect)
{
	rect->center = makePoint(rect->origin.x + rect->w / 2.f,
							 rect->origin.y + rect->h / 2.f);
	return rect->center;
}

typedef struct QuadTree
{
	vec_p_t points;
	size_t total_points;

	Rect boundary;

	struct QuadTree* north_west;
	struct QuadTree* north_east;
	struct QuadTree* south_west;
	struct QuadTree* south_east;
} QuadTree;

bool qt_init(QuadTree** qt, const Rect rect)
{
	QuadTree* temp = (QuadTree*)calloc(1U, sizeof(struct QuadTree));
	if(temp)
	{
		temp->boundary = rect;
		vec_init(&temp->points);
		vec_reserve(&temp->points, BUCKET_SIZE);
		*qt = temp;
		return true;
	}
	return false;
}

bool qt_destroy(QuadTree* const qt)
{
	// TODO Iterate over the children until one of them is NULL and free at each
	// step, just recurse through it.
	vec_deinit(&qt->points);
	if(!qt->north_west)
	{
		free(qt);
		return true;
	}
	qt_destroy(qt->north_west);
	qt_destroy(qt->north_east);
	qt_destroy(qt->south_west);
	qt_destroy(qt->south_east);
	free(qt);
	return true;
}

// TODO subdivide, move the items in the points vector to the correct children
void qt_subdivide(QuadTree* const qt)
{
	QuadTree* a = (qt);
	a = a;
}

bool qt_insert(QuadTree* const qt, const Point p)
{
	if(!pointInRect(&p, &qt->boundary))
		return false;

	if(qt->points.length < BUCKET_SIZE && !qt->north_west)
	{
		vec_push(&qt->points, p);
		return true;
	}

	if(!qt->north_west)
		qt_subdivide(qt);

	if(qt_insert(qt->north_west, p))
		return true;
	if(qt_insert(qt->north_east, p))
		return true;
	if(qt_insert(qt->south_west, p))
		return true;
	if(qt_insert(qt->south_east, p))
		return true;

	return false;
}

void qt_getPointsInRect(QuadTree* const qt,
						const Rect* const rect,
						vec_p_t* vec)
{
	if(!intersects(&qt->boundary, rect))
		return;

	for(uint8_t i = 0U; i < qt->points.length; ++i)
		if(pointInRect(&qt->points.data[i], rect))
			vec_push(vec, qt->points.data[i]);

	if(!qt->north_west)
		return;

	qt_getPointsInRect(qt->north_west, rect, vec);
	qt_getPointsInRect(qt->north_east, rect, vec);
	qt_getPointsInRect(qt->south_west, rect, vec);
	qt_getPointsInRect(qt->south_east, rect, vec);

	return;
}

#endif
