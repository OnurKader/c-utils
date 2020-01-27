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

	Point random_points[52];
	for(uint8_t i = 0U; i < 52; ++i)
	{
		random_points[i] = makePoint((rand() % 10000 / 10000.f * 100.f) - 50.f,
									 (rand() % 10000 / 10000.f * 100.f) - 50.f);
		if(i % 4 == 3)
			printf("(%+06.2f, %+06.2f)\n", random_points[i].x, random_points[i].y);
		else
			printf("(%+06.2f, %+06.2f)\t", random_points[i].x, random_points[i].y);
	}

	printf("\n");

	if(qt_init(&qt, quad_rect))
	{
		/* qt_insert(qt, random_points[0]); */
		/* qt_insert(qt, random_points[1]); */
		/* qt_insert(qt, random_points[2]); */
		/* qt_insert(qt, random_points[3]); */
		for(uint8_t i = 0U; i < 52; ++i)
		{
			qt_insert(qt, random_points[i]);
		}

		Rect query = makeRect(0.f, 0.f, 25.f, 25.f);
		vec_p_t points;
		vec_init(&points);

		qt_getPointsInRect(qt, &query, &points);
		int i;
		Point* point;
		vec_foreach_ptr(&qt->points, point, i)
		{
			printf("(%+06.2f, %+06.2f)\t", point->x, point->y);
		}

		vec_deinit(&points);

		qt_destroy(qt);
	}

	return 0;
}
