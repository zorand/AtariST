#include <stdio.h>
#include <math.h>
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
   unsigned char print;
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
            Bconout(0,print);
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
   fread(vmem, 1, 32000, file);
}   

double function(double x){
   return asin(x/sqrt(2*x*x+4*x+4));
}

print(double x0, double x1, double y0, double y1){
   int maxx, maxy,x,y,n,m;
   double step; 
   maxx=getmaxx()+1;
   maxy=getmaxy()+1;
   n=0;
   m=maxy-(function(x0)-y0)*maxy/(y1-y0)-1;
   for(x=1;x<maxx;x++){
      y=maxy-(function(x0+(x1-x0)*x/maxx)-y0)*maxy/(y1-y0)-1;
      line(n,m,x,y);
      n=x;
      m=y;
   }
}

putgrid(double x0, double x1, double y0, double y1){
   int maxx, maxy,x,y;
   double step; 
   maxx=getmaxx()+1;
   maxy=getmaxy()+1;
   for(x=0;x<maxx;x++){
      y=maxy-(0-y0)*maxy/(y1-y0)-1;
      putpixel(x,y,1);
   }
   for(y=0;y<maxy;y++){
      x=(0-x0)*maxx/(x1-x0);
      putpixel(x,y,1);
   }
}

main(int argc, char *argv[])
{
int gd,gm;
int mode;
char c;
double x0,x1,y0,y1;
   graf_mouse(M_OFF, NULL);
   gd=STGEM;
   gm=STHIGH;
   initgraph(&gd,&gm,"");

   puts("Unesite Xmin, Xmax, Ymin, Ymax:");
   scanf("%lf %lf %lf %lf",&x0,&x1,&y0,&y1);
   Bconout(2,27);
   Bconout(2,'E');

   putgrid(x0,x1,y0,y1);
   print(x0,x1,y0,y1);

   c=getch();
   if(c=='p' || c=='P'){
      fprintf(stdprn,"\n y=asin(x/sqrt(2*x*x+4*x+4))\n");
      fprintf(stdprn,"%lf < x < %lf, %lf < y < %lf\n",x0,x1,y0,y1);
      mode=ROTATE;
      winddump(-1,-1,0,0,mode);
   }
   graf_mouse(M_ON, NULL);
   return 0;
}

