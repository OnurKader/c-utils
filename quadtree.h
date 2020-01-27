#ifndef QUADTREE_H
#define QUADTREE_H

#include "vec.h"

#include <SDL2/SDL_shape.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifndef BUCKET_SIZE
#	define BUCKET_SIZE 4
#endif

typedef SDL_FPoint Point;

typedef SDL_FRect Rect;

typedef vec_t(Point) vec_p_t;
typedef vec_t(Point) vec_point_t;

bool pointInRect(const Point* const p, const Rect* const r)
{
	return ((p->x >= r->x) && (p->x < r->x + r->w) && (p->y >= r->y) &&
			(p->y < r->y + r->h));
}

bool intersects(const Rect* const r1, const Rect* const r2)
{
	if(r1->x + r1->w < r2->x)	 // r1 is left of r2
		return false;
	if(r1->x > r2->x + r2->w)	 // r1 is right of r2
		return false;

	if(r1->y + r1->h < r2->y)	 // r1 is above r2
		return false;
	if(r1->y > r2->y + r2->h)	 // r1 is below r2
		return false;

	return true;
}

Point makePoint(float x, float y) { return (Point){x, y}; }

Rect makeRect(const float x, const float y, const float w, const float h)
{
	Rect temp;
	temp.x = x;
	temp.y = y;
	temp.w = w;
	temp.h = h;
	return temp;
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
	*qt = NULL;
	return false;
}

bool qt_destroy(QuadTree* qt)
{
	vec_deinit(&qt->points);
	if(!qt->north_west)
	{
		free(qt);
		qt = NULL;
		return true;
	}
	qt_destroy(qt->north_west);
	qt_destroy(qt->north_east);
	qt_destroy(qt->south_west);
	qt_destroy(qt->south_east);
	return true;
}

bool qt_insert(QuadTree* const, const Point);

void qt_subdivide(QuadTree* const qt)
{
	if(qt->north_west)
		return;

	Rect rect;
	rect = makeRect(
		qt->boundary.x, qt->boundary.y, qt->boundary.w / 2.f, qt->boundary.h / 2.f);

	qt_init(&qt->north_west, rect);
	rect.x = qt->boundary.x + qt->boundary.w / 2.f;
	qt_init(&qt->north_east, rect);
	rect.x = qt->boundary.x;
	rect.y = qt->boundary.y + qt->boundary.h / 2.f;
	qt_init(&qt->south_west, rect);
	rect.x = qt->boundary.x + qt->boundary.w / 2.f;
	rect.y = qt->boundary.y + qt->boundary.h / 2.f;
	qt_init(&qt->south_east, rect);

	// After initializing the children 4 nodes, put the data in qt->points to
	// the correct nodes.
	for(uint8_t i = 0U; i < BUCKET_SIZE; ++i)
	{
		if(qt_insert(qt->north_west, qt->points.data[i]))
		{
			vec_splice(&qt->points, i, 1);
		}
		else if(qt_insert(qt->north_east, qt->points.data[i]))
		{
			vec_splice(&qt->points, i, 1);
		}
		else if(qt_insert(qt->south_west, qt->points.data[i]))
		{
			vec_splice(&qt->points, i, 1);
		}
		else if(qt_insert(qt->south_east, qt->points.data[i]))
		{
			vec_splice(&qt->points, i, 1);
		}
	}
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

void qt_getPointsInRect(QuadTree* const qt, const Rect* const rect, vec_p_t* vec)
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

