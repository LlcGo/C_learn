#include <sdtio.h>
#include <stdlib.h>

typedef char ElemType;

typedef struct BiTNode
{
   char data;
   struct BiTNode *lchild,*rchild;
}BiTNode,*BiTree;


void createTree(BiTree *T)
{
  char c;
  scanf("%c",&c);
  if(' ' == c)
  {
     *T = NULL;
  }
  else
  {
     *T = (BiTree)malloc(sizeof(BiTNode));
     *T->data = c;
     createTree(&(*T)->lchild);
     createTree(&(*T)->rchild);
  }
}
