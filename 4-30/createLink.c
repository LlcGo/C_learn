#include <stdio.h>
#include <stdlib.h>

typedef struct Node0{
   int value;
   struct Node0 * next;
}Node0;

typedef struct Node0 * Node;


void createLinked(Node * node ,int value);
void printfNode(Node * node);
void removeL(Node * node);
void removeL2(Node * node);

int main(void)
{
 Node list;
 createLinked(&list,5);
 printfNode(&list); 
 //printf("地址->%p",&list); 
 removeL2(&list);
 printfNode(&list); 
 return 0;
}

void createLinked(Node * node, int n)
{
   int i;
   Node new;
   *node = (Node)malloc(sizeof(Node));
   if(*node == NULL)
   {
     printf("无内存空");
     return;
   }
   (*node)->next = NULL;
   for(i < 0; i < n; i++)
   {
     new = (Node)malloc(sizeof(Node));
     new->value = i;
     new->next = (*node)->next;
     (*node)->next = new;     
   }
}

void printfNode(Node * node)
{
  Node temp = *node;
  while(temp != NULL)
  {
     printf("值->%d\n",temp->value);
     temp =temp->next;
  }
}

void removeL(Node * node)
{
  Node  p;
  while((*node)->next)
  {
   printf("开删\n");
   p = (*node)->next;
   (*node)->next = p->next;
   free(p);
  }
  (*node)->next = NULL; 
}

void removeL2(Node * node)
{
  Node  p,c;
  p = (*node)->next;
  while(p)
  {
   printf("开删\n");
   c = p->next;
   free(c);
   p = c;
  }
  (*node)->next = NULL;
}

int getMidNode(Node  node, int *e)
{
   Node fast,mid;
   mid = fast = node;
   while(fast-> != NULL)
   {
     if(fast->next->next != NULL)
     { 
        fast = fast->next->next;
        mid = mid->next;
     } 
     else
     {
        fast = fast->next;
     }       
   }
  *e = mid->value;
  return 0;
}
