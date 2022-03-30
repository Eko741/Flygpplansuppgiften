#include "Queue_object.h"

void QueueObject::swapFlights(QueueObject* a, QueueObject* b)
{
	Data* tmp = a->data;
	a->data = b->data;
	b->data = tmp;
}
