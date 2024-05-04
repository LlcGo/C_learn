#include <stdio.h>
#include <stdlib.h>

#define SIZE 10;

typedef char ElemType;

typedef struct
{
  Elemtype * base;
  ElemType * top; 
  int stackSize; 
}Stack;

void init(Stack * s);
int push(Stack * s, int e);
int clear(Stack * s);
int DestoryStack(Stack * s);
int currentTotal(Stack * s);

void init(Stack * s)
{
    s->base = (ElemType *)malloc(sizeof(ElemType));
    s->top = s->base;
    s->stackSize = SIZE;
}


int main(void)
{ 
  ElemType c;
  Stack * s;
  init(s);
  
  printf("请输入参数")
  scanf("%c",&c);
  while(c != '#')
  {
    push(&s,&c);
    scanf("%c",&c);
  }   
  getchar(); //清除缓冲区\n
  int len = currentToal(&s);
  int i;
  int total;
  for(i = 0; i < len; i++)
  {
     ElemType tmp;
     tmp =  pop(&s);
     total += (tmp -48) * pow(2,i);    
  }
  printf("转换为十进制为%d",total);
  return 0;
}

// 入栈
int push(Stack * s, ElemType e)
{
  // 俩个数组指针相减，获得此类型个数
  // 如果栈满，追加空间
  if(s->top - s->base >= stackSize)
  {
    // 每次扩容 stackSize个 使用malloc进行拷贝的
    s->base = (ElemType *)realloc(s->base,(s->stackSize + SIZE) * sizeof(ElemType)); 
    // top 往上移动 stackSize的空间
    s->top = s->base + s->stackSize;
    // 设置现在的容量
    s->stackSize = s->stackSize + SIZE;
  }
  *(s->top) =  e;
  //上移一位
  s->top++;
  return 0;   
}

// 出栈
ElemType pop(Stack * s)
{
   if(s->base == s->top)
       return -1; 
   // 指针下移一位后才能获得数据
   ElempTyep i =  *--(s->top); 
   return i; 
}

// 清空栈
int clear(Stack *s)
{
  s->top = s->base;
  return 0;
}

// 销毁一个栈
int DestoryStack(Stack * s)
{
   int i;
   int len = s->stackSize;
   // 从下往上销毁 不然清不完
   for(i = 0; i < len; i++)
   {
       free(s->base);
       s-base++;
   }
   s->base = s->top = NULL;
   s->stackSize = 0;
}

// 计算当前容量
int currentTotal(Stack * s)
{
  // 指针可以相减 但是相+基本没有意义
  return (s->top - s->base);
}
