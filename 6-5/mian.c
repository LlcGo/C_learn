#include <stdio.h>

int main(){

   int sum = 0;
   int i;
   for(i = 0; i < 100; i++)
   {
	   sum+=i;
	   printf("第%d次--->%d\n",i,sum);
   }   
   return 0;	
}
