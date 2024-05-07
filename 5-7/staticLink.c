#include <stdio.h>
#include <stdlib.h>
#define SpaceSize 100

typedef int ElementType;
typedef int PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

struct Node
{
  ElementType value;
  Position Next;
};


strcut Node CursorSpace[SpaceSize];

static Position CursorAlloc(void)
{
   Position p;
   p = CursorSpace[0].next;
   CursorSpace[0] = CursorSpace[p].next;
   return p;
}
