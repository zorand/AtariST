/* Include files */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aes.h>
#include <vdi.h>
#include <tos.h>
#include <scancode.h>
#include "dial.h"

/* Defines */

#define TRUE 1
#define FALSE 0
#define WI_KIND   (MOVER|CLOSER|NAME)
#define NIL (-1)
#define NO_WINDOW (-1)

/* Globals */

char *filename="dial.cnf";
char *mark="KRC Dialer configuration file\n";
char *dialseq="ATDP";

struct dlist{
   char name[19];
   char number[17];
   char redial[4];
   int  selected;
   struct dlist *pred, *succ;};

struct diallist{
   struct dlist *first, *last; } dialerlist;

struct dlist *viewlist[7], *lasthit, *dialingbbs;
int dialingflag=FALSE;
int dialingtime=-1;

/* Gem stuff */

OBJECT *dial, *info, *settings, *dialog;
int   editstring, editcur,first;
int   time=1000;
int   multiredial=10;

int   gl_apid; /* Acc number */
int   acc=0;

int   gl_hchar;
int   gl_wchar;
int   gl_wbox;
int   gl_hbox; /* system sizes */

int   menu_id ; /* my menu id */

int   phys_handle;/* physical workstation handle */
int   handle;     /* virtual workstation handle */
int   wi_handle;  /* window handle */
int   top_window; /* handle of topped window */

int   xdesk,ydesk,hdesk,wdesk;
int   xold,yold,hold,wold;
int   xwork,ywork,hwork,wwork;
int   xwind,ywind,hwind,wwind;    /* desktop and work areas */

int   msgbuff[8]; /* event message buffer */
int   keycode;    /* keycode returned by event-keyboard */
int   mx,my,mb;   /* mouse x and y pos. */
int   ret;        /* dummy return variable */

int   hidden;     /* current state of cursor */

int   fulled;     /* current state of window */

int   contrl[12];
int   intin[128];
int   ptsin[128];
int   intout[128];
int   ptsout[128];   /* storage wasted for idiotic bindings */

int   work_in[11];  /* Input to GSX parameter array */
int   work_out[57]; /* Output from GSX parameter array */
int   pxyarray[10]; /* input point array */

/* Redeclarations */

void draw_proc(int,int,int,int);

/* Functions */

struct dlist *add(struct dlist *p){
   struct dlist *point;

   if( (point=malloc(sizeof(struct dlist)))==NULL ) return NULL;
   point->pred=p;
   if(p==NULL) dialerlist.first=point;
   if(p!=NULL){
      point->succ=p->succ;
      p->succ=point;
   } else point->succ=NULL;
   if(point->succ==NULL) dialerlist.last=point;
   strcpy(point->name,"");
   strcpy(point->number,"");
   strcpy(point->redial,"");
   point->selected=FALSE;
   return point;
}

delete(struct dlist *p){
   if(p->pred!=NULL) (p->pred)->succ=p->succ;
   else{
      dialerlist.first=p->succ;
      if(p->succ!=NULL) (p->succ)->pred=NULL;
   }
   if(p->succ!=NULL) (p->succ)->pred=p->pred;
   else{
      dialerlist.last=p->pred;
      if(p->pred!=NULL) (p->pred)->succ=NULL;
   }
   free(p);
}

init_dlist(struct dlist *first){
   int i, flag=TRUE;

   viewlist[0]=first;
   for(i=1;i<7;i++){
      if(viewlist[i-1]==NULL) flag=FALSE;
      if(flag) viewlist[i]=viewlist[i-1]->succ;
      else viewlist[i]=NULL;
   }
}

draw_dlist(int flag){
   int i;

   for(i=0; i<7; i++){
      if(viewlist[i]!=NULL){
         strcpy(dial[BBS1+i].ob_spec.tedinfo->te_ptext,viewlist[i]->name);
         if(viewlist[i]->selected==TRUE) dial[BBS1+i].ob_state=SELECTED;
         else dial[BBS1+i].ob_state=NORMAL;
      } else{
         strcpy(dial[BBS1+i].ob_spec.tedinfo->te_ptext,"");
         dial[BBS1+i].ob_state=NORMAL;
      }
      if(flag) objc_draw(dial,BBS1+i,1,0,0,0,0);
   }
}

scroll(int direction){
   int i;

   if(direction==DOWN){
      if(viewlist[6]!=NULL)
         if(viewlist[6]->succ!=NULL){
            viewlist[0]=viewlist[0]->succ;
            init_dlist(viewlist[0]);
            draw_dlist(TRUE);
          }
   } else if(direction==UP){
      if(viewlist[0]!=NULL)
         if(viewlist[0]->pred!=NULL){
            viewlist[0]=viewlist[0]->pred;
            init_dlist(viewlist[0]);
            draw_dlist(TRUE);
         }
   }
}

load_conf(){
   FILE *conf;
   char buf[30];
   struct dlist *p;

   conf=fopen(filename,"rb");
   if(conf==NULL){
      dialerlist.first=NULL;
      dialerlist.last=NULL;
      return;
   }
   fread(buf,strlen(mark),1,conf);
   if(strcmp(buf,mark)){
      dialerlist.first=NULL;
      dialerlist.last=NULL;
      return;
   }
   p=NULL;
   while(!feof(conf)){
      p=add(p);
      if(p==NULL) break;
      if(fread(p->name,1,19,conf)<17){
         delete(p);
         break;
      }
      fread(p->number,17,1,conf);
      fread(p->redial,4,1,conf);
      p->selected=FALSE;
   }
   fclose(conf);
}

save_conf(){
   FILE *conf;
   struct dlist *p;

   conf=fopen(filename,"wb");
   if(conf==NULL) return -1;
   fwrite(mark, strlen(mark),1,conf);
   p=dialerlist.first;
   if(p!=NULL) do{
      fwrite(p->name,19,1,conf);
      fwrite(p->number,17,1,conf);
      fwrite(p->redial,4,1,conf);
      p=p->succ;
   } while(p!=NULL);
   fclose(conf);
   return 0;
}

/* GEM */

shutdown(int code)
{
    wind_update(FALSE);
    cleanup();
    appl_exit();  /* Shut down the application */ 

    exit(code);
}

cleanup()
{
    struct dlist *p;
    p=dialerlist.first;
    while(p!=NULL)
       if(p->succ!=NULL){
           p=p->succ;
           delete(p->pred);
       } else{
           delete(p);
           p=NULL;
       }
    wind_close(wi_handle);
    wind_delete(wi_handle);
    rsrc_free();  /* Free memory used by resource. */
}

int read_modem(buf, count)
register char *buf;
register int count;
{
   register int n;

   n = 0;
   if(Bconstat(1)){
      n++;
      *buf++ = Bconin(1);
      while((n < count)  && Bconstat(1)){
         *buf++ = Bconin(1);
         if(*(buf-1)>27) n++;
         else buf--;
      }
      *buf='\0';
      return n;
   } else{
        *buf='\0';
        return 0;
   }
}

void write_modem(buf,len)
register char *buf;
register int len;
{
   register int i;
   for(i=0;i<len;i++)
      Bconout(1,*buf++);
}

void flushinput()
{
   while(Bconstat(1))
       Bconin(1);
}

void send2modem(char *str)
{
   Bconout(1,13);
   write_modem(str, strlen(str));
   Bconout(1,13);
}

hide_mouse()
{
   if(! hidden){
      graf_mouse(M_OFF,0x0L);
      hidden=TRUE;
   }
}

show_mouse()
{
   if(hidden){
      graf_mouse(M_ON,0x0L);
      hidden=FALSE;
   }
}

/* open virtual workstation */

open_vwork()
{
int i;
   for(i=0;i<10;work_in[i++]=1);
   work_in[10]=2;
   handle=phys_handle;
   v_opnvwk(work_in,&handle,work_out);
}

init_resources()
{
   if (!rsrc_load("dial.rsc")) {
      form_alert(1, "[0][Cannot find dial.rsc file][Bye]");
      exit(2);
   }
}

/* open window */

open_window()
{
   wi_handle=wind_create(WI_KIND,xwind,ywind,wwind,hwind);
   wind_set(wi_handle, WF_NAME," KRC Dialer ",0,0);
   wind_open(wi_handle,xwind,ywind,wwind,hwind);
   wind_get(wi_handle,WF_WORKXYWH,&xwork,&ywork,&wwork,&hwork);
}


/* find and redraw all clipping rectangles */

do_redraw(xc,yc,wc,hc)
int xc,yc,wc,hc;
{
GRECT t1,t2;

   hide_mouse();
   wind_update(TRUE);
   t2.g_x=xc;
   t2.g_y=yc;
   t2.g_w=wc;
   t2.g_h=hc;
   wind_get(wi_handle,WF_FIRSTXYWH,&t1.g_x,&t1.g_y,&t1.g_w,&t1.g_h);
   while (t1.g_w && t1.g_h) {
     draw_proc(t1.g_x,t1.g_y,t1.g_w,t1.g_h);
     wind_get(wi_handle,WF_NEXTXYWH,&t1.g_x,&t1.g_y,&t1.g_w,&t1.g_h);
   }
   wind_update(FALSE);
   show_mouse();
}

void draw_proc(int x, int y, int w, int h)
{
   int cur;
   wind_get(wi_handle, WF_WORKXYWH, &xwork, &ywork, &wwork, &hwork);
   dialog->ob_x=xwork;
   dialog->ob_y=ywork;

   if(dialog==dial) draw_dlist(0);
   objc_draw(dialog,ROOT,MAX_DEPTH,x,y,w,h);
   if(dialog==settings){
      if(first){
         objc_edit(dialog,editstring,0,&editcur,ED_INIT);
         objc_edit(dialog,editstring,0,&editcur,ED_START);
         first=FALSE;
      }else{
         objc_edit(dialog,editstring,0,&editcur,ED_END);
         objc_edit(dialog,editstring,0,&editcur,ED_START);
      }
   }
}

modem_dial(struct dlist *bbs){
   static struct dlist *last;

   if(bbs==NULL){
      form_alert(1,"[0][MultiDialing :)][ OK ]");
   } else{
      Bconout(1,13);
      write_modem(dialseq,strlen(dialseq));
      write_modem(bbs->number,strlen(bbs->number));
      Bconout(1,13);
   }
}

do_dial(int obj_hit,int br){
    if(obj_hit>=BBS1 && obj_hit<=BBS7)
       if(viewlist[obj_hit-BBS1]!=NULL){
          lasthit=viewlist[obj_hit-BBS1];
          if(br==2){
             dialingbbs=lasthit;
             dialingflag=TRUE;
             modem_dial(dialingbbs);
          } else if(viewlist[obj_hit-BBS1]->selected){
             viewlist[obj_hit-BBS1]->selected=FALSE;
             dial[obj_hit].ob_state=NORMAL;
          } else{
             viewlist[obj_hit-BBS1]->selected=TRUE;
             dial[obj_hit].ob_state=SELECTED;
          }
          objc_draw(dialog,obj_hit,1,0,0,0,0);
       }
    switch(obj_hit){
       case UP:
       case DOWN:
          scroll(obj_hit);
       break;
       case ABOUT:
          dialog=info;
          do_redraw(xwind,ywind,wwind,hwind);
       break;
       case DIALER:
          dialingbbs=NULL;
          dialingflag=TRUE;
          modem_dial(dialingbbs);
       break;
       case ADD:
          dialog=settings;
          lasthit=add(dialerlist.last);
          first=TRUE;
          editcur=0;
          editstring=NAME;
          strcpy(settings[NAME].ob_spec.tedinfo->te_ptext,lasthit->name);
          strcpy(settings[NUMBER].ob_spec.tedinfo->te_ptext,lasthit->number);
          strcpy(settings[REDIAL].ob_spec.tedinfo->te_ptext,lasthit->redial);
          do_redraw(xwind,ywind,wwind,hwind);
       break;
       case SETUP:
          if(lasthit!=NULL){
             dialog=settings;
             first=TRUE;
             editcur=0;
             editstring=NAME;
             strcpy(settings[NAME].ob_spec.tedinfo->te_ptext,lasthit->name);
             strcpy(settings[NUMBER].ob_spec.tedinfo->te_ptext,lasthit->number);
             strcpy(settings[REDIAL].ob_spec.tedinfo->te_ptext,lasthit->redial);
             do_redraw(xwind,ywind,wwind,hwind);
          }
       break;
       case CANCEL:
          if(!acc) shutdown(0);
       break;
    }
}

do_settings(int obj_hit,int br){
    switch(obj_hit){
       case SAVE:
       case OK:
          objc_edit(dialog,editstring,0,&editcur,ED_END);
          strcpy(lasthit->name,settings[NAME].ob_spec.tedinfo->te_ptext);
          strcpy(lasthit->number,settings[NUMBER].ob_spec.tedinfo->te_ptext);
          strcpy(lasthit->redial,settings[REDIAL].ob_spec.tedinfo->te_ptext);
          if(!strcmp(lasthit->name,"") && !strcmp(lasthit->number,"")){
             delete(lasthit);
             init_dlist(dialerlist.first);
          } else if(viewlist[0]==NULL){
             init_dlist(dialerlist.first);
          } else init_dlist(viewlist[0]);
          if(obj_hit==SAVE) save_conf();
          dialog=dial;
          do_redraw(xwind,ywind,wwind,hwind);
       break;
       case DELETE:
          objc_edit(dialog,editstring,0,&editcur,ED_END);
          strcpy(settings[NAME].ob_spec.tedinfo->te_ptext,"");
          strcpy(settings[NUMBER].ob_spec.tedinfo->te_ptext,"");
          strcpy(settings[REDIAL].ob_spec.tedinfo->te_ptext,"");
          first=TRUE;
          do_redraw(xwind,ywind,wwind,hwind);
       break;
       case NAME:
       case NUMBER:
       case REDIAL:
          objc_edit(dialog,editstring,0,&editcur,ED_END);
          editstring=obj_hit;
          objc_edit(dialog,editstring,0,&editcur,ED_INIT);
          objc_edit(dialog,editstring,0,&editcur,ED_START);
       break;
    }
}


/* Accessory Init. Until First Event_Multi */

main()
{
   gl_apid=appl_init();
   phys_handle=graf_handle(&gl_wchar,&gl_hchar,&gl_wbox,&gl_hbox);
   if(acc) menu_id=menu_register(gl_apid,"  KRC Dialer");
   else graf_mouse(ARROW,NULL);

   init_resorce();

/* Get address of dialog definition in memory. */

   rsrc_gaddr(0, DIAL, &dial);
   rsrc_gaddr(0, INFO, &info);
   rsrc_gaddr(0, SETTINGS, &settings);
   dialog=dial;

   form_center(dial, &xwind, &ywind, &wwind, &hwind);
   hwind+=18;

   wi_handle=NO_WINDOW;
   hidden=FALSE;
   open_vwork();
   open_window();
   load_conf();
   init_dlist(dialerlist.first);
   lasthit=dialerlist.first;

   multi();
}

/* dispatches all accessory tasks */

multi()
{
   int event, cont, br, obj_hit;

   while (TRUE){
      event = evnt_multi(MU_MESAG | MU_BUTTON | MU_KEYBD | MU_TIMER,
                         0x02,0x01,0x01,
                         0,0,0,0,0,
                         0,0,0,0,0,
                         msgbuff,time,0,&mx,&my,&mb,&ret,&keycode,&br);
      wind_update(TRUE);
      wind_get(wi_handle,WF_TOP,&top_window,&ret,&ret,&ret);
      if (event & MU_MESAG){
         switch (msgbuff[0]) {
            case WM_REDRAW:
               if (msgbuff[3] == wi_handle)
                  do_redraw(msgbuff[4],msgbuff[5],msgbuff[6],msgbuff[7]);
            break;
            case WM_NEWTOP:
            case WM_TOPPED:
               if (msgbuff[3] == wi_handle){
                  wind_set(wi_handle,WF_TOP,0,0,0,0);
               }
            break;
            case AC_CLOSE:
               if((msgbuff[3] == menu_id)&&(wi_handle != NO_WINDOW)){
                  v_clsvwk(handle);
                  wi_handle = NO_WINDOW;
               }
            break;
            case WM_CLOSED:
               if(msgbuff[3] == wi_handle){
                  if(dialog==settings){
                     objc_edit(dialog,editstring,0,&editcur,ED_END);
                     if(!strcmp(lasthit->name,"") && !strcmp(lasthit->number,""))
                        delete(lasthit);
                     dialog=dial;
                     do_redraw(xwind,ywind,wwind,hwind);
                  } else if(dialog==info){
                     dialog=dial;
                     do_redraw(xwind,ywind,wwind,hwind);
                  } else if(acc){
                     wind_close(wi_handle);
                     wind_delete(wi_handle);
                     v_clsvwk(handle);
                     wi_handle = NO_WINDOW;
                  } else if(dialog==dial){
                     shutdown(0);
                  }
               }
            break;
            case WM_MOVED:
               if(msgbuff[3] == wi_handle){
                  wind_set(wi_handle,WF_CURRXYWH,
                           msgbuff[4],msgbuff[5],msgbuff[6],msgbuff[7]);
                  wind_get(wi_handle,WF_WORKXYWH,&xwork,&ywork,&wwork,&hwork);
                  dialog->ob_x=xwork;
                  dialog->ob_y=ywork;
               }
            break;
            case AC_OPEN:
               if (msgbuff[4] == menu_id){
                  if(wi_handle == NO_WINDOW){
                     open_vwork();
                     open_window();
                  } else  /* if already opened, for user convenience */
                     wind_set(wi_handle,WF_TOP,0,0,0,0);
               }
            break;
         } /* switch (msgbuff[0]) */
      }
      if (event & MU_BUTTON && wi_handle == top_window){
         obj_hit=objc_find(dialog,ROOT,MAX_DEPTH,mx,my);
         if(dialog==info){
            if(obj_hit!=NIL){
               dialog=dial;
               do_redraw(xwind,ywind,wwind,hwind);
            }
         } else if(dialog==dial){
            do_dial(obj_hit,br);
         } else if(dialog==settings)
            do_settings(obj_hit,br);
      }

      if( event & MU_KEYBD && wi_handle==top_window){
         if(dialog==info){
            dialog=dial;
            do_redraw(xwind,ywind,wwind,hwind);
         } else if(dialog==dial){
            switch(keycode){
               case ALT_A:
                  do_dial(ADD,br);
               break;
               case ALT_C:
                  do_dial(CANCEL,br);
               break;
               case ALT_D:
                  do_dial(DIALER,br);
               break;
               case ALT_S:
                  do_dial(SETUP,br);
               break;
               case CUR_UP:
                  do_dial(UP,br);
               break;
               case CUR_DOWN:
                  do_dial(DOWN,br);
               break;
            }
         } else if(dialog==settings){
            switch(keycode){
               case RETURN:
               case ENTER:
               case CUR_DOWN:
                  objc_edit(dialog,editstring,0,&editcur,ED_END);
                  if((keycode==RETURN || keycode==ENTER) && editstring==REDIAL){
                     do_settings(OK,br);
                     break;
                  }
                  switch(editstring){
                     case NAME: editstring=NUMBER;
                     break;
                     case NUMBER: editstring=REDIAL;
                     break;
                     case REDIAL: editstring=NAME;
                     break;
                  }
                  objc_edit(dialog,editstring,0,&editcur,ED_INIT);
                  objc_edit(dialog,editstring,0,&editcur,ED_START);
               break;
               case CUR_UP:
                  objc_edit(dialog,editstring,0,&editcur,ED_END);
                  switch(editstring){
                     case REDIAL:
                        editstring=NUMBER;
                     break;
                     case NAME: editstring=REDIAL;
                     break;
                     case NUMBER: editstring=NAME;
                     break;
                  }
                  objc_edit(dialog,editstring,0,&editcur,ED_INIT);
                  objc_edit(dialog,editstring,0,&editcur,ED_START);
               break;
               default:
                  objc_edit(dialog,editstring,keycode,&editcur,ED_CHAR);
               break;
            }
         }
      }

      if( event & MU_TIMER ){
         if(dialingtime>=0) dialingtime++;
         if(dialingbbs==NULL){
            if(dialingtime>multiredial){
               dialingtime=-1;
               modem_dial(NULL);
            }
         } else{
            if(dialingtime>atoi(dialingbbs->redial)){
               dialingtime=-1;
               modem_dial(dialingbbs);
            }
         }
      }

      wind_update(FALSE);
   }
}
