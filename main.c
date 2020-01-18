#include "doub_link_list.h"

#include <stdio.h>

int main(void)
{
	List *list;
	if(!list_init(&list))
	{
		list_push(list, 1L);
		list_push(list, 74L);
		list_push(list, -1L);
		list_push(list, 2309L);
		/* list_push(list, 405L); */
		/* list_push(list, 0L); */
		/* list_push(list, -2309L); */

		/* list_insert(list, 69L, 3UL); */
		/* list_insert(list, 420L, 6UL); */
		/* list_insert(list, -69420L, 2UL); */
		/* list_insert(list, 69420L, 0UL); */

		list_print(list);

		printf("popping: %ld, %ld, %ld, %ld\n",
			   list_pop(list),
			   list_pop(list),
			   list_pop(list),
			   list_pop(list));

		list_destroy(list);
	}
	return 0;
}

