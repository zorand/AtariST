/*
 *   Primena racunara u Numerickoj matematici
 *   ----------------------------------------
 *
 *   Oblast: Interpolacija
 *   Tema:   Interpolacija funkcija dve nezavisne promenljive
 *
 *   Zoran Dimitrijevic, IV 4
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 *   Globalne promenljive:
 */

double x0,h;         /* x[i]=x0+i*h */
double y0,k;         /* y[i]=y0+i*k */
int N;               /* Broj tacaka */
double *x, *y;       /* x[i], y[i]  */
double **z;          /* Vrednosti funkcije */
double **kon_raz;    /* Tabela konacnih razlika */
double **p;          /* Interpolacioni polinom */
double *px, *py;     /* Polinomi oblika px=a0+a1*x+a2*x^2+... */
int *komb;           /* Pomocni niz za trazenje koeficijenata */


/* Memory allocation */

memalloc(int N){
   int i;

   /* Formiranje z[N][N]: */
   z=calloc(N,sizeof(void *));
   for(i=0;i<N;i++) z[i]=calloc(N,sizeof(double));

   /* Formiranje x[N] i y[N]: */
   x=calloc(N,sizeof(double));
   y=calloc(N,sizeof(double));
   x[0]=x0;
   for(i=1;i<N;i++) x[i]=x[i-1]+h;
   y[0]=y0;
   for(i=1;i<N;i++) y[i]=y[i-1]+k;

   /* Formiranje polinoma: */
   p=calloc(N,sizeof(void *));
   for(i=0;i<N;i++) p[i]=calloc(N,sizeof(double));
   px=calloc(N,sizeof(double));
   py=calloc(N,sizeof(double));

   /* Formiranje tabele konacnih razlika: */
   kon_raz=calloc(N,sizeof(void *));
   for(i=0;i<N;i++) kon_raz[i]=calloc(N,sizeof(double));

   /* Formiranje pomocnog niza: */
   komb=calloc(N,sizeof(int));
}

/* Oslobadjanje alocirane memorije */

memfree(){
   int i;

   for(i=0;i<N;i++){
      free(z[i]);
      free(kon_raz[i]);
      free(p[i]);
   }
   free(x);
   free(y);
   free(z);
   free(kon_raz);
   free(p);
   free(px);
   free(py);
   free(komb);
}

/* Unos podataka */

unos_pod(char *filename){
   FILE *input;
   int i,j;

   if(filename!=NULL && *filename!='-') input=fopen(filename,"r");
   else input=stdin;
   if(input==stdin) printf("\n Unesi x0 i h (x[i]=x0+i*h):");
   fscanf(input, "%lf %lf", &x0, &h);
   if(input==stdin) printf("\n Unesi y0 i k (y[i]=y0+i*k):");
   fscanf(input, "%lf %lf", &y0, &k);
   if(input==stdin) printf("\n Unesi broj tacaka:");
   fscanf(input, "%d", &N);
   if(N<2 || h==0 || k==0){
      fclose(input);
      fputs(stderr, "Greska u unosu!");
      exit(-1);
   }
   memalloc(N);
   if(input==stdin) printf("\n Unesi tacke z(x[i],y[i]):\n");
   for(i=0;i<N;i++)
      for(j=0;j<N;j++) fscanf(input, "%lf", &z[i][j]);
   fclose(input);
}

/* Formiranje tabele konacnih razlika */

kon_razlike(){
   int i,j,k;

   for(i=0;i<N;i++)
      for(j=0;j<N;j++) kon_raz[i][j]=z[i][j];
   for(i=0;i<N;i++)
      for(k=0;k<N-1;k++)
         for(j=N-1;j>k;j--) kon_raz[i][j]-=kon_raz[i][j-1];
   for(j=0;j<N;j++)
      for(k=0;k<N-1;k++)
         for(i=N-1;i>k;i--) kon_raz[i][j]-=kon_raz[i-1][j];
}

int fakt(int n){
   int i,f=1;

   for(i=1;i<=n;i++) f*=i;
   return f;
}

kombinacije(int n, int k, int m, int p, double *c, double *x){
   int i,j,l;
   double koef;

   for(i=p;i<=n-k+m;i++){
      komb[m]=i;
      if(m==k-1){
         koef=1;
         for(l=0;l<k;l++) koef*=x[komb[l]];
         *c+=koef;
      } else kombinacije(n,k,m+1,i+1,c,x);
   }
}

/* Trazenje koeficijenata i formiranje polinoma */

koef(){
   int i,j,m,n,znak;
   double c=1,c1;

   for(i=0;i<N;i++)
      for(j=0;j<N-i;j++) p[i][j]=0;
   p[0][0]=z[0][0];

   for(i=1;i<=(N-1)*(N-1);i++){
      c/=i;
      for(j=0;j<=i;j++){
         if(i-j<N && j<N){
            c1=kon_raz[j][i-j]*fakt(i)/fakt(j)/fakt(i-j)/pow(h,i-j)/pow(k,j);
            px[i-j]=1; znak=-1;
            for(m=1;m<=i-j;m++){
               px[i-j-m]=0;
               kombinacije(i-j,m,0,0,&px[i-j-m],x);
               px[i-j-m]*=znak;
               znak*=-1;
            }
            py[j]=1; znak=-1;
            for(m=1;m<=j;m++){
               py[j-m]=0;
               kombinacije(j,m,0,0,&py[j-m],y);
               py[j-m]*=znak;
               znak*=-1;
            }
            for(m=0;m<=j;m++)
               for(n=0;n<=i-j;n++) p[m][n]+=c*c1*px[n]*py[m];
         }
      }
   }
}

/* Ispis polinoma */

ispis(FILE *output){
   int i,j;

   fprintf(output,"\nZadate vrednosti:\n\n");
   fprintf(output,"N=%d\n",N);
   fprintf(output,"x[i]=%.2lf+%.2lf*i\n",x0,h);
   fprintf(output,"y[i]=%.2lf+%.2lf*i\n",y0,k);
   fprintf(output,"z[i][j]:\n");
   for(i=0;i<N;i++){
      for(j=0;j<N;j++) fprintf(output,"%8.2lf,",z[i][j]);
      fprintf(output,"\n");
   }
   fprintf(output,"\nTabela konacnih razlika:\n");
   for(i=0;i<N;i++){
      for(j=0;j<N;j++) fprintf(output,"%8.2lf,",kon_raz[i][j]);
      fprintf(output,"\n");
   }
   fprintf(output,"\nP(x,y)=");
   for(i=0;i<N;i++){
      for(j=0;j<N;j++) fprintf(output,"%+.2lf*x^%d*y^%d",p[i][j],j,i);
      fprintf(output,"\n       ");
   }
   fprintf(output,"\n");
}

main(int argc, char *argv[])
{
   FILE *output;
   int i;

   unos_pod(argv[1]);
   kon_razlike();
   koef();
   if(argc==3) output=fopen(argv[2],"a");
   else output=stdout;   
   ispis(output);
   if(output!=stdout) fclose(output);
   memfree();
}

