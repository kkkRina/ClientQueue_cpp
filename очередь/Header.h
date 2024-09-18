#pragma once

struct man
{
	char name[150];
	char sex[10];
	int age;
	char job[150];
	int cash;
	char aim[10];
};

struct queue_elem
{
	man employee;
	queue_elem* next = nullptr;
	queue_elem* prev = nullptr;
};

struct queue
{
	queue_elem* first = nullptr;
	queue_elem* last = nullptr;
};

void enqueue(queue& q, man employee);
bool dequeue(queue& q, man& employee);
void clear(queue& q);
