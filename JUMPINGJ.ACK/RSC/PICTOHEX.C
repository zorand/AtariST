#include <stdio.h>

main(int argc, char *argv[])
{
   FILE *source, *target;
   int buffer;
   int i,j;

   if ((argv[2]==0) || (argv[1]==0)) exit(-1);
   source=fopen(argv[1],"rb");
   target=fopen(argv[2],"a");
   if((source==NULL) || (target==NULL)) exit(-1);
   for(i=0;i<17;i++) fread(&buffer,2,1,source);
   for(j=0;j<158;j++){
      for(i=0;i<40;i++){
         fread(&buffer,2,1,source);
         if(i<15) fprintf(target,"%d,",buffer);
      }
      fprintf(target,"\n");
   }
   return 0;
}
