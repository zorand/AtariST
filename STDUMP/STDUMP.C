#include <stdio.h>
#include <graphics.h>
#include <tos.h>
#include <aes.h>

#define dump() winddump(-1,-1,0,0,ROTATE)
#define PRNCOLOR -1
#define NORMAL 0
#define EXTEND 1
#define SHRINK 2
#define SHRINKED 3
#define SMALL 4
#define ROTATE 5
#define SHRINKROTATE 6
#define SHRINKEDROTATE 7
#define SMALLROTATE 8

winddump(int x,int y,int w,int h, int mode){
   int maxx, maxy;
   int i,j,k,buf;
   unsigned int print;
   int prnx, color, graphmode=1;
   int n,s,t;
   int nmb=1,step=0;

   if(x<0 || y<0){
      x=0; y=0;
      maxx=getmaxx()+1;
      maxy=getmaxy()+1;
   } else{
      maxx=x+w;
      maxy=y+h;
   }
   switch(mode){
      case NORMAL: n=8; s=1; prnx=maxx-x;
      break;
      case EXTEND: n=8; s=1; prnx=(maxx-x); graphmode=4;
      break;
      case SHRINK: n=16; s=2; prnx=maxx-x; nmb=2;
      break;
      case SMALL:
      case SHRINKED: n=24; s=3; prnx=maxx-x; nmb=3;
      break;
      case ROTATE:
         n=8; s=1; prnx=maxy-y;
         buf=maxx; maxx=maxy; maxy=buf;
         buf=x; x=y; y=buf;
         buf=x; x=maxx; maxx=buf;
         graphmode=0;
      break;
      case SHRINKROTATE:
         n=16; s=2; nmb=2; prnx=maxy-y;
         buf=maxx; maxx=maxy; maxy=buf;
         buf=x; x=y; y=buf;
         buf=x; x=maxx; maxx=buf;
      break;
      case SHRINKEDROTATE:
      case SMALLROTATE:
         n=24; s=3; nmb=3; prnx=maxy-y;
         buf=maxx; maxx=maxy; maxy=buf;
         buf=x; x=y; y=buf;
         buf=x; x=maxx; maxx=buf;
      break;
   }
   switch(mode){
      case SMALL:
      case SMALLROTATE:
         graphmode=3;
      break;
   }
   if(y>maxy){
      n*=-1;
      s*=-1;
   }
   do{
      for(k=0;k<nmb;k++){
         fputc(27,stdprn);
         if(k==0){
            fputc('3',stdprn);
            fputc(24-step,stdprn);
            fputc('\n',stdprn);
            step=0;
         } else{
            step++;
            if(n>0) y++;
            else y--;
            fputc('3',stdprn);
            fputc(1,stdprn);
            fputc('\n',stdprn);
         }
         fputc(27,stdprn);
         fputc('*',stdprn);
         fputc(graphmode,stdprn);
         fputc(prnx % 256,stdprn);
         fputc(prnx / 256,stdprn);

         for(i=(x<maxx?x:x-1); x<maxx ? i<maxx : i>=maxx;){
            print=0;
            for(j=y; n>0 ? j<y+n : j>y+n; j+=s){
               print<<=1;
               if(mode<ROTATE) color=getpixel(i,j);
               else color=getpixel(j,i);
               if(color & PRNCOLOR && (n>0?j<maxy:j>=maxy)) print|=1;
            }
            fputc(print,stdprn);
            if(x<maxx) i++; else i--;
         }
      }
      y+=(n>0 ? n-step : n+step);
   } while(n>0 ? y<=maxy : y>=maxy);
   fputc(27,stdprn);
   fputc('2',stdprn);
   fputc('\n',stdprn);
   fputc('\n',stdprn);
}

unos(char *filename){
   FILE *file;
   char c, *vmem;
   int i;

   vmem=Physbase();
   file=fopen(filename, "rb");
   if(file==NULL){
      graf_mouse(M_ON, NULL);
      exit(-1);
   }
   for(i=0;i<34;i++) fread(&c,1,1,file);
   fread(vmem, 2, 16000, file);
}   

main(int argc, char *argv[])
{
int gd,gm;
int mode;
   graf_mouse(M_OFF, NULL);
   gd=STGEM;
   gm=STHIGH;
   initgraph(&gd,&gm,"");
   unos(argv[1]);
   if(argv[2]!=NULL)
      switch(*argv[2]){
         case 0: mode=SHRINK;
         break;
         default:
            if(*argv[2]>='0' && *argv[2]<='8') mode=*argv[2]-'0';
            else mode=SHRINK;
         break;
      }
   winddump(-1,-1,0,0,mode);
   graf_mouse(M_ON, NULL);
   return 0;
}

