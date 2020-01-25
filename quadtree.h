#ifndef QUADTREE_H
#define QUADTREE_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define center centre

#ifndef BUCKET_SIZE
#	define BUCKET_SIZE 4U
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

bool pointInRect(const Point* const p, const Rect* const r)
{
	return ((p->x >= r->origin.x) && (p->x < r->origin.x + r->w) && (p->y >= r->origin.y) &&
			(p->y < r->origin.y + r->h));
}

Point makePoint(float x, float y) { return (Point){x, y}; }

Rect makeRect(float x, float y, float w, float h)
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
	rect->center =
		makePoint(rect->origin.x + rect->w / 2.f, rect->origin.y + rect->h / 2.f);
	return rect->center;
}

typedef struct QuadTree
{
	Point points[BUCKET_SIZE];

	struct QuadTree* north_west;
	struct QuadTree* north_east;
	struct QuadTree* south_west;
	struct QuadTree* south_east;
} QuadTree;

bool qt_init(QuadTree** qt);

bool qt_destroy(QuadTree* qt);

#endif

