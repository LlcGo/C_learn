#include <stdio.h>
#include "queue.h"
#include <assert.h>

#define QUEUE_SIZE 100  //最大存的数量
#define ARRAY_SIZE (QUEUE_SIZE + 1)  //真正的数量

static QUEUE_TYPE queue[ARRAY_SIZE];
static size_t front = 1; 
static size_t rear = 0;

int empty(void)
{
   return (rear + 1) % ARRAY_SIZE == front;
}

int is_full(void)
{
  return (rear + 2) % ARRAY_SIZE == front;
}

void insert(QUEUE_TYPE value)
{
  assert(!is_full());
  rear = (rear + 1) % ARRAY_SIZE;
  queue[rear] = value;
}

void delete(void)
{
 assert(!is_empty());
 front = (front + 1) % ARRAY_SIZE;
}

QUEUE_TYPE first(void)
{ 
  assert(!is_empty());
  return queue[front];
}

