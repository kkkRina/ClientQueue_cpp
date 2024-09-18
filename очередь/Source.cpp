#include "Header.h"

void enqueue(queue& q, man employee)
{
	auto* new_elem = new queue_elem;
	new_elem->employee = employee;
	new_elem->prev = q.last;
	if (q.last) q.last->next = new_elem;
	q.last = new_elem;
	if (!q.first) q.first = new_elem;
}

bool dequeue(queue& q, man& employee)
{
	if (!q.first) return false;
	employee = q.first->employee;
	auto* old_elem = q.first;
	q.first = q.first->next;
	delete old_elem;
	if (q.first) q.first->prev = nullptr;
	else q.last = nullptr;
	return true;
}

void clear(queue& q)
{
	auto* curr = q.first;
	while (curr)
	{
		auto* old_elem = curr;
		curr = curr->next;
		delete old_elem;
	}
}