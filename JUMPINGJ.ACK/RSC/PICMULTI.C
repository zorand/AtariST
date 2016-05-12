#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *multi(char *number, int n){
   char *a, *b;
   int i,j,k,l; /* brojaci */
   int p,c,pom;
   int lenght;

   lenght=strlen(number);
   a=malloc(lenght);
   b=malloc(lenght);
   for(i=0;i<lenght;i++) a[i]='0';
   a[i]=0;

   for(i=1,l=0;n/i;i*=10,l++){
      p=0;
      c=n%(i*10)/i;
      for(j=lenght;--j>=lenght-l;) b[j]='0';
      for(k=lenght-1;j>=0;j--,k--){
         pom=(number[k]-'0')*c+p;
         b[j]=pom%10+'0';
         p=pom/10;
      }
      p=0;
      for(j=lenght-1;j>=0;j--){
         pom=p+a[j]-'0'+b[j]-'0';
         a[j]=pom%10+'0';
         p=pom/10;
      }
   }
   strcpy(number, a);
   free(a); free(b);
   return number;
}

main()
{
   char *number;
   int fakt=70;
   int i;

/*
*  number="00000...0001";
*/
   number=malloc(201);
   for(i=0;i<199;i++) number[i]='0';
   number[199]='1'; number[200]=0;

   for(i=2;i<=fakt;i++) number=multi(number,i);
   fprintf(stdprn, "\n%d!=%s\n", fakt, number);
   return 0;
}
