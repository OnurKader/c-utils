#include "doub_link_list.h"
#include "quadtree.h"

#include <stdio.h>
#include <time.h>

int main(void)
{
	/* List *list; */
	/* if(!list_init(&list)) */
	/* { */
	/* 	list_push(list, 1L); */
	/* 	list_push(list, 74L); */
	/* 	list_push(list, -1L); */
	/* 	list_push(list, 2309L); */
	/* 	list_push(list, 405L); */
	/* 	list_push(list, 0L); */
	/* 	list_push(list, -2309L); */

	/* 	list_insert(list, 69L, 3UL); */
	/* 	list_insert(list, 420L, 6UL); */
	/* 	list_insert(list, -69420L, 2UL); */
	/* 	list_insert(list, 69420L, 0UL); */

	/* 	list_print(list); */

	/* 	printf("popping: %ld, %ld, %ld, %ld\n", */
	/* 		   list_pop(list), */
	/* 		   list_pop(list), */
	/* 		   list_pop(list), */
	/* 		   list_pop(list)); */

	/* 	list_destroy(list); */
	/* } */

	srand(time(NULL));

	QuadTree* qt = NULL;
	const Rect quad_rect = makeRect(-50.f, -50.f, 100.f, 100.f);

	Point random_points[51];
	for(uint8_t i = 0U; i < 51; ++i)
	{
		random_points[i] = makePoint((rand() % 100000 / 100000.f * 100.f) - 50.f,
									 (rand() % 100000 / 100000.f * 100.f) - 50.f);
		if(i % 3 == 2)
			printf("(%+06.2f, %+06.2f)\n", random_points[i].x, random_points[i].y);
		else
			printf("(%+06.2f, %+06.2f)\t", random_points[i].x, random_points[i].y);
	}

	if(qt_init(&qt, quad_rect))
	{
		qt_insert(qt, random_points[0]);
		qt_insert(qt, random_points[1]);
		qt_insert(qt, random_points[2]);

		Rect query = makeRect(0.f, 0.f, 25.f, 25.f);
		Point points[128U];
		size_t num_of_points = 0ULL;
		qt_getPointsInRect(qt, &query, points, &num_of_points);

		qt_destroy(qt);
	}

	return 0;
}

