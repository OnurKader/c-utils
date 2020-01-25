#ifndef QUADTREE_H
#define QUADTREE_H

#include <math.h>
#include <stdint.h>
#include <stdio.h>

float dist(float a, float b, float c, float d)
{
	return sqrt((a - c) * (a - c) + (b - d) * (b - d));
}

typedef struct Point
{
	float x, y;
} Point;

#endif

