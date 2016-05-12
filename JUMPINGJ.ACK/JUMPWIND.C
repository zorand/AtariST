/* Include files */

#include <aes.h>
#include <vdi.h>
#include <tos.h>
#include <scancode.h>
#include "jumping.h"

/* Functions */

hide_mouse()
{
	if(!hidden){
		graf_mouse(M_OFF,NULL);
		hidden=TRUE;
	}
}

show_mouse()
{
	if(hidden){
		graf_mouse(M_ON,NULL);
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
	maxx=work_out[0];
	maxy=work_out[1];
}

shutdown(int code)
{
	wind_close(wi_handle);
	wind_delete(wi_handle);
	v_clsvwk(handle);
	appl_exit();  /* Shut down the application */ 

	exit(code);   /* The end */
}

open_window()
{
	wi_handle=wind_create(WI_KIND,xwind,ywind,wwind,hwind);
	wind_set(wi_handle, WF_NAME," Jumping Jack by KRC Games, Jan 1993. ",0,0);
	wind_open(wi_handle,xwind,ywind,wwind,hwind);
	wind_get(wi_handle,WF_WORKXYWH,&xwork,&ywork,&wwork,&hwork);
}

int Rand(int n){
	register float a;
	register int number;

	a=(float)Random()/0x1000000*n;
	return number=a;
}

long pow(int n){
	int i;
	long num=1;
	for(i=1;i<n;i++) num*=10;
	return num;
} 

char *int2str(int number, char *str, int lenght){
	long i;
	int j=0;

	for(i=pow(lenght);i>0;i/=10){
		str[j]=number/i;
		number-=i*str[j];
		str[j++]+=48;
	}
	str[j++]=' ';
	str[j]=0;
	return str;
}

printlives(){
	v_gtext(handle,xwork+60,ywork+ytext,int2str(lives, str, 1));
}

printscore(){
	v_gtext(handle,xwork+wwork-60,ywork+ytext,int2str(score, str, 6));
}

printhighscore(){
	v_gtext(handle,xwork+275,ywork+ytext,int2str(hightscore, str, 6));
}

/* find and redraw all clipping rectangles */

do_redraw(int xc,int yc,int wc,int hc)
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

draw_help(int x,int y){
	int pxyarray[8];
	MFDB help, dest;

	pxyarray[0]=0; pxyarray[1]=0; pxyarray[2]=239; pxyarray[3]=155;
	pxyarray[4]=xwork+x; pxyarray[5]=ywork+y;
	pxyarray[6]=xwork+x+239; pxyarray[7]=ywork+y+155;
	help.fd_addr=helpic[0];
	help.fd_w=240;
	help.fd_h=156;
	help.fd_wdwidth=15;
	help.fd_stand=0;
	help.fd_nplanes=1;
	help.fd_r1=0;
	help.fd_r2=0;
	help.fd_r3=0;
	dest=jackMFDB;
	dest.fd_addr=NULL;
	hide_mouse();
	vro_cpyfm(handle,S_ONLY,pxyarray,&help,&dest);
	show_mouse();
}

draw_jack(int x,int y,int *sprite){

	pxy[4]=xwork+x; pxy[5]=ywork+y;
	pxy[6]=xwork+x+31; pxy[7]=ywork+y+31;
	jackMFDB.fd_addr=sprite;	
	vro_cpyfm(handle,S_ONLY,pxy,&jackMFDB,&destMFDB);
}

gameover(){
	if(!overflag){
		overflag=TRUE;
		vst_effects(handle, OUTLINED);
		vst_point(handle,20,&ret,&ret,&ret,&ret);
		v_gtext(handle,xwork+wwork/2-75,ywork+hwork/2+4, "GAME OVER");
		vst_effects(handle, 0);
		vst_point(handle,10,&ret,&ret,&ret,&ret);
		show_mouse();
	}else{
		lives=4;
		level=1;
		if(score>hightscore) hightscore=score;
		score=0;
		overflag=FALSE;
		setup(1);
	}
}

dropdead(){
	if(--lives==0){
		jack.what=SITF1;
		jack.sprite=sit_for[0];
		draw_jack(jack.x,jack.y,sit_for[0]);
		gameover();
	}
	printlives();
}

fall_down(struct place *smb){
	int line[4];

	if (!fallcont){
		if ((smb->what>=WALK1L) && (smb->what<=WALK5L)) smb->x-=5;
		if ((smb->what>=WALK1R) && (smb->what<=WALK5R)) smb->x+=5;
		draw_jack(smb->x, smb->y, stand);
		fallcont++;
		return 0;
	}
	if (fallcont<6){
		fallcont++;
		vsl_color(handle,0);
		vsl_width(handle,7);
		smb->y+=7;
		draw_jack(smb->x, smb->y, fall[fallcont%2]);
		line[0]=xwork+smb->x; line[2]=xwork+smb->x+31;
		line[1]=line[3]=ywork+smb->y-4;
		v_pline(handle,2,line);
	} else {
		fallcont=0;
		fallflag=FALSE;
		smb->what=STAND;
		smb->sprite=stand;
		knockedflag=TRUE;
		vsl_color(handle,1);
		vsl_width(handle,2);
		line[0]=xwork+smb->x; line[2]=xwork+smb->x+32;
		line[1]=line[3]=ywork+smb->y-3;
		v_pline(handle,2,line);
		if(smb->y==293) dropdead();
	}
}

add_hole(){
	if(holenumber<maxhole){
		hole[holenumber].x=Rand(wwork);
		hole[holenumber].y=10+Rand(8)*35;
		hole[holenumber].direction=Rand(2);
		holenumber++;
	}
}

knocked_down(struct place *smb){
	if(knockedcont++<33){
		if(smb->what==STAND) draw_jack(smb->x, smb->y, sit_for[knockedcont%2]);
		else if((smb->what>=WALK1R) && (smb->what<=WALK5R))
			draw_jack(smb->x, smb->y, sit_right[knockedcont%2]);
		else if((smb->what>=WALK1L) && (smb->what<=WALK5L))
			draw_jack(smb->x, smb->y, sit_left[knockedcont%2]); 
	}
	else{
		knockedcont=0;
		knockedflag=FALSE;
		if((smb->what>=WALK1R) && (smb->what<=WALK5R)) smb->x+=10;
		else if((smb->what>=WALK1L) && (smb->what<=WALK5L)) smb->x-=10;
		smb->what=STAND;
		smb->sprite=stand;
		draw_jack(smb->x, smb->y, stand);
	}
}

go_left(struct place *smb){
	static int flag=1;
	if ((smb->what>=WALK1R) && (smb->what<=WALK5R)) smb->x+=10+STEP;
	if (smb->what==STAND) smb->x+=5+STEP;
	if (smb->x>=-24) smb->x-=STEP;
	if (smb->x<-24) smb->x=wwork;
	if ((smb->what>=WALK1L) && (smb->what<=WALK5L)){
		if (smb->what==WALK1L) flag=1;
		else if (smb->what==WALK5L) flag=-1;
		smb->what+=flag;
		smb->sprite=walk_left[smb->what-WALK1L];
	}
	else{
		smb->what=WALK3L;
		smb->sprite=walk_left[2];
		flag=1;
	}
}

go_right(struct place *smb){
	static int flag=1;
	if ((smb->what>=WALK1L) && (smb->what<=WALK5L)) smb->x-=10+STEP;
	if (smb->what==STAND) smb->x-=5+STEP;
	if (smb->x<=wwork-8) smb->x+=STEP;
	if (smb->x>wwork-8) smb->x=-32;
	if ((smb->what>=WALK1R) && (smb->what<=WALK5R)){
		if (smb->what==WALK1R) flag=1;
		else if (smb->what==WALK5R) flag=-1;
		smb->what+=flag;
		smb->sprite=walk_right[smb->what-WALK1R];
	}
	else{
		smb->what=WALK3R;
		smb->sprite=walk_right[2];
		flag=1;
	}
}

go_monster(struct place *smb){
	smb->sprite=monstersprite[smb->what][monsterstepnum];
	if(smb->x>wwork-5){
		smb->x=-32;
		if(smb->y==293-35) smb->y=13;
		else smb->y+=35;
	}else smb->x+=MONSTEP;
	draw_jack(smb->x,smb->y,smb->sprite);
}

go_jump(struct place *smb){
	int line[4];

	if (!jumpcont){
		add_hole();
		if ((smb->what>=WALK1L) && (smb->what<=WALK5L)) smb->x-=5;
		if ((smb->what>=WALK1R) && (smb->what<=WALK5R)) smb->x+=5;
		draw_jack(smb->x, smb->y, stand);
		jumpcont++;
		smb->what=STAND;
		smb->sprite=stand;
		return 0;
	}
	if (jumpcont==1){
		draw_jack(smb->x, smb->y, jump[0]);
		jumpcont++;
		return 0;
	}
	if (jumpcont==2){
		jumpcont++;
		return 0;
	}
	vsl_color(handle,0);
	vsl_width(handle,7);
	if (jumpcont++<8){
		smb->y-=7;
		draw_jack(smb->x, smb->y, jump[jumpcont%2+1]);
		line[0]=xwork+smb->x; line[2]=xwork+smb->x+31;
		line[1]=line[3]=ywork+smb->y+35;
		v_pline(handle,2,line);
	}else {
		jumpcont=0;
		jumpflag=FALSE;
		draw_jack(smb->x, smb->y, stand);
		vsl_color(handle,1);
		vsl_width(handle,2);
		line[0]=xwork+smb->x; line[2]=xwork+smb->x+32;
		line[1]=line[3]=ywork+smb->y+32;
		v_pline(handle,2,line);
	}
}

go_hit(struct place *smb){
	hitflag=FALSE;
	knockedflag=TRUE;
}

draw_hole(struct hole_place *smt){
	int line[8];

	vsl_color(handle,0);
	vsl_width(handle,2);
		if(smt->x>wwork-holelenght){
			line[0]=xwork; line[2]=xwork+holelenght+smt->x-wwork;
			if (smt->y==290) line[1]=line[3]=ywork+10;
			else line[1]=line[3]=ywork+smt->y+35;	
			v_pline(handle,2,line);
		}
		line[0]=xwork+smt->x; line[2]=xwork+smt->x+holelenght;
		line[1]=line[3]=ywork+smt->y;
		v_pline(handle,2,line);
}

go_hole(struct hole_place *smt){
	int line[4];

	vsl_color(handle,1);
	vsl_width(handle,2);
	if(smt->direction==1){
		line[0]=xwork+smt->x;line[2]=xwork+smt->x+HOLESTEP;
		line[1]=line[3]=ywork+smt->y;
		v_pline(handle,2,line);
		smt->x+=HOLESTEP;
		 if(smt->x>wwork){
			smt->x=0;
			if (smt->y==290) smt->y=10;
			else smt->y+=35;
		}
	} else{
		if(smt->x>wwork-holelenght){
			line[0]=xwork+holelenght-wwork+smt->x; line[2]=line[0]-HOLESTEP;
			if(smt->y==290) line[1]=line[3]=ywork+10;
			else line[1]=line[3]=ywork+smt->y+35;
			v_pline(handle,2,line);
		}
		line[0]=xwork+smt->x+holelenght;line[2]=xwork+smt->x+holelenght-HOLESTEP;
		line[1]=line[3]=ywork+smt->y;
		v_pline(handle,2,line);
		smt->x-=HOLESTEP;
		 if(smt->x<0){
			smt->x+=wwork;
			if (smt->y==10) smt->y=290;
			else smt->y-=35;
		}
	}
	draw_hole(smt); 
}

checkhole(struct place *smb, struct hole_place *smt){

	if((smb->what>=WALK1L) && (smb->what<=WALK5L))
		if((smb->x>smt->x)&&(smb->x<smt->x+holelenght-20)&&(smb->y+32==smt->y)) return TRUE;
	if ((smb->what>=WALK1R) && (smb->what<=WALK5R))
		if((smb->x+12>smt->x)&&(smb->x+12<smt->x+holelenght-20)&&(smb->y+32==smt->y)) return TRUE;
	if (smb->what==STAND)
		if((smb->x+6>smt->x)&&(smb->x+26<smt->x+holelenght)&&(smb->y+32==smt->y)) return TRUE;

	if (smt->x>wwork-holelenght){
		if((smb->what>=WALK1L) && (smb->what<=WALK5L))
			if((smb->x>smt->x-wwork)&&(smb->x<smt->x-wwork+holelenght-20)
			&&(smb->y+32==((smt->y==290)?10:smt->y+35))) return TRUE;
		if ((smb->what>=WALK1R) && (smb->what<=WALK5R))
			if((smb->x+12>smt->x-wwork)&&(smb->x+12<smt->x-wwork+holelenght-20)
			&&(smb->y+32==((smt->y==290)?10:smt->y+35))) return TRUE;
		if (smb->what==STAND)
			if((smb->x+6>smt->x-wwork)&&(smb->x+26<smt->x-wwork+holelenght)
			&&(smb->y+32==((smt->y==290)?10:smt->y+35))) return TRUE;
	}
	return FALSE;
}

checkfall(struct place *smb){
	int i;
	for(i=0;i<holenumber;i++)
		if(checkhole(smb, &hole[i].x)){
			fallflag=TRUE;
			return i;
		}
}

checkmonster(){
	int i;
	for(i=0;i<monsternumber;i++){
		if((jack.what>=WALK1L) && (jack.what<=WALK5L))
			if((jack.x>monster[i].x+10)&&(jack.x<monster[i].x+33)&&(jack.y==monster[i].y)){
				knockedflag=TRUE;
				if(jumpflag){
					if(jumpcont>3) go_jump(&jack.x);
					jumpcont=0;
					jumpflag=FALSE;
				}
				return 0;
			}
		if ((jack.what>=WALK1R) && (jack.what<=WALK5R))
			if((jack.x+32>monster[i].x+10)&&(jack.x+10<monster[i].x+32)&&(jack.y==monster[i].y)){
				knockedflag=TRUE;
				if(jumpflag){
					if(jumpcont>3) go_jump(&jack.x);
					jumpcont=0;
					jumpflag=FALSE;
				}
				return 0;
			}
		if (jack.what==STAND)
			if((jack.x+26>monster[i].x+10)&&(jack.x+6<monster[i].x+32)&&(jack.y==monster[i].y)){
				knockedflag=TRUE;
				if(jumpflag){
					if(jumpcont>3) go_jump(&jack.x);
					jumpcont=0;
					jumpflag=FALSE;
				}
				return 0;
			}
	}
}

checkjump(struct place *smb){
	int i;
	smb->y-=35;
	for(i=0;i<holenumber;i++)
		if(checkhole(smb, &hole[i].x)){
			jumpflag=TRUE;
			smb->y+=35;
			return i;
		}
	smb->y+=35;
	hitflag=TRUE;
	return -1;
}

void draw_proc(int x, int y, int w, int h)
{
	int pxyarray[4], line[4], i;

	pxyarray[0]=x; pxyarray[1]=y;
	pxyarray[2]=x+w-1; pxyarray[3]=y+h-1;
	vs_clip(handle,1,pxyarray);
	vsf_color(handle,0);

	wind_get(wi_handle, WF_WORKXYWH, &xwork, &ywork, &wwork, &hwork);
	pxyarray[0]=xwork; pxyarray[1]=ywork;
	pxyarray[2]=xwork+wwork; pxyarray[3]=ywork+hwork;
	vr_recfl(handle,pxyarray);
	vsl_color(handle,1);
	vsl_width(handle,2);
	for(i=10;i<300;i+=35){
		line[0]=xwork;line[2]=xwork+MAXX;
		line[1]=line[3]=ywork+i;
		v_pline(handle,2,line);
	}
	draw_jack(jack.x, jack.y, jack.sprite);
	vsl_color(handle,0);
	vsl_width(handle,2);
	for(i=0;i<holenumber;i++) draw_hole(&hole[i].x);
	for(i=0;i<monsternumber;i++) draw_jack(monster[i].x,monster[i].y,monstersprite[i]);
	v_gtext(handle, xwork+5, ywork+ytext, "Lives:");
	printlives();
	v_gtext(handle, xwork+100, ywork+ytext, "Level:");
	v_gtext(handle, xwork+157, ywork+ytext, int2str(level, str, 1));
	v_gtext(handle, xwork+wwork-110, ywork+ytext, "Score:");
	printscore();
	v_gtext(handle,xwork+190,ywork+ytext, "Highscore:");
	printhighscore();
	if(overflag){
		overflag=FALSE;
		gameover();
	}
	pxyarray[0]=xwork; pxyarray[1]=ywork;
	pxyarray[2]=xwork+wwork-1; pxyarray[3]=ywork+hwork-1;
	vs_clip(handle,1,pxyarray);
}

levelend(struct place *smb){
	if(smb->y==-22) return TRUE;
	else return FALSE;
}

setup(int newlevel){
	int i;
	float a;
	long r;

	level=newlevel;
	if(level>MAXLEVEL) level=1;
	jack.x=MAXX/2;
	jack.y=293;
	jack.what=STAND;
	jack.sprite=stand;
	jumpflag=FALSE; jumpcont=0;
	fallflag=FALSE; fallcont=0;
	knockedflag=FALSE; knockedcont=0;
	hitflag=FALSE; hitcont=0;
	holenumber=level;
	if((monsternumber=level-1)>MAXMONSTER) monsternumber=MAXMONSTER;
	if((maxhole=level+4)>MAXHOLE) maxhole=MAXHOLE;
	HOLESTEP=4+level;
	MONSTEP=1+level;
	holelenght=50;
	for(i=0;i<holenumber;i++){
		hole[i].x=Rand(wwork);
		hole[i].y=10+Rand(8)*35;
		hole[i].direction=Rand(2);
	}
	for(i=0;i<monsternumber;i++){
		monster[i].x=-32+Rand(wwork);
		monster[i].y=13+i*70;
		monster[i].what=i;
		monster[i].sprite=monstersprite[i][0];
	}
	hide_mouse();
	draw_proc(xwork,ywork,wwork,hwork);
	mxold=mx;
	walk=0;
  /*show_mouse();*/
}

main()
{
	gl_apid=appl_init();
	phys_handle=graf_handle(&gl_wchar,&gl_hchar,&gl_wbox,&gl_hbox);

	hidden=FALSE;
	graf_mouse(ARROW,NULL);

/* Set window size */
	xwind=30; ywind=20; wwind=MAXX; hwind=MAXY;
	hide_mouse();
	open_vwork();
	open_window();
	show_mouse();

	vst_font(handle,1);
	vst_color(handle,1);
	vst_effects(handle, 0);
	vst_point(handle,10,&ret,&ret,&ret,&ret);
	ytext=hwork-5;
	jackMFDB.fd_w=32;
	jackMFDB.fd_h=32;
	jackMFDB.fd_wdwidth=2;
	jackMFDB.fd_stand=0;
	jackMFDB.fd_nplanes=1;
	jackMFDB.fd_r1=0;
	jackMFDB.fd_r2=0;
	jackMFDB.fd_r3=0;
	jack.x=-32;
	jack.y=-22;

	pxy[0]=0; pxy[1]=0; 	 /* pxyarray for draw_jack function */
	pxy[2]=31; pxy[3]=31;
	destMFDB=jackMFDB;
	destMFDB.fd_addr=NULL;

	multi();
	return 0;
}

/* dispatches all jumping tasks */

multi()
{
	int event, br, TheEnd=FALSE, control=20, i, Start=TRUE;
	int my_event, time, pxyarray[4];
	int mbold=0, keycold=0; /* key_code_old :) */
	unsigned char c;

	my_event=MU_MESAG | MU_BUTTON | MU_KEYBD | MU_TIMER;
	time=33;
	while (!TheEnd) {
		event = evnt_multi(my_event,
				0x02,0x01,0x01,
				0,0,0,0,0,
				0,0,0,0,0,
				msgbuff,time,0,&mx,&my,&mb,&ret,&keycode,&br);

		wind_update(TRUE);
		wind_get(wi_handle,WF_TOP,&top_window,&ret,&ret,&ret);

		if (event & MU_MESAG && !hidden) switch (msgbuff[0]){
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

			case WM_CLOSED:
				if(msgbuff[3] == wi_handle){
					wind_close(wi_handle);
					wind_delete(wi_handle);
					v_clsvwk(handle);
					wi_handle = NO_WINDOW;
					TheEnd=TRUE;
				}
			break;

			case WM_MOVED:
				if(msgbuff[3] == wi_handle){
					wind_set(wi_handle,WF_CURRXYWH,msgbuff[4],msgbuff[5],msgbuff[6],msgbuff[7]);
					wind_get(wi_handle,WF_WORKXYWH,&xwork,&ywork,&wwork,&hwork);
					pxyarray[0]=xwork; pxyarray[1]=ywork;
					pxyarray[2]=xwork+wwork-1; pxyarray[3]=ywork+hwork-1;
					vs_clip(handle,1,pxyarray);
				}
			break;
		} /* switch (msgbuff[0]) */

		if(top_window==wi_handle){
			my_event=MU_MESAG | MU_BUTTON | MU_KEYBD | MU_TIMER;
			time=33;
	
			if(event & MU_KEYBD){
				if(keycode==HELP){
					if(helpflag){
						helpflag=FALSE;
						hide_mouse();
						draw_proc(xwork+123,ywork+69,250,160);
					} else{
						draw_help(125,71);
						helpflag=TRUE;
					}
				}
				if(keycode==CNTRL_C){
					if(hidden) show_mouse();
					else hide_mouse();
				}
				if(keycode==UNDO)
				if(overflag) gameover();
				else{
					if(hidden) show_mouse();
					else hide_mouse();
				}
			}

			if(helpflag){
				if(mb==2 || mb==3){
					helpflag=FALSE;
					hide_mouse();
					draw_proc(xwork+123,ywork+69,250,160);
				}
			} else{
				if(control<20) control++;
				else if(mb==3){
					if(overflag) gameover();
					else{
						if(hidden) show_mouse();
						else hide_mouse();
						control=0;
					}
				}

				if (levelend(&jack.x)) setup(++level);
				if(Start){
					draw_help(125,71);
					helpflag=TRUE;
					Start=FALSE;
				}
				if(event & MU_KEYBD) keycold=keycode;
				if(mb) mbold=mb;

				if (hidden && event & MU_TIMER){
					keycode=keycold; keycold=0;
					if (!mb){ mb=mbold; mbold=0;}
					if(++monsterstepnum>=MAXMONSTER) monsterstepnum=0;
					for(i=0;i<monsternumber;i++) go_monster(&monster[i].x);
					for(i=0;i<holenumber;i++) go_hole(&hole[i].x);
					fallhole=checkfall(&jack.x);
					checkmonster();
					if (fallflag){
						jumpflag=FALSE;
						jumpcont=0;
						fall_down(&jack.x);
					}
					else if (knockedflag) knocked_down(&jack.x);
					else if (jumpflag) go_jump(&jack.x);
					else{
						if (mb==2){
							jumphole=checkjump(&jack.x);
							if(jumpflag){
								score+=10;
								printscore();
								go_jump(&jack.x);
							}
							else{
								go_hit(&jack.x);
								if(jack.y==293) dropdead();
							}
							walk=0;
						}
						if (keycode){
							switch(c=keycode){
								case 'o': case 'O': case 'n': case 'N': case '7':
									go_left(&jack.x);
									walk=0;
								break;
								case 'p': case 'P': case 'm': case 'M': case '9':
									go_right(&jack.x);
									walk=0;
								break;
								case ' ': case '8':
									jumphole=checkjump(&jack.x);
									if(jumpflag){
										score+=10;
										printscore();
										go_jump(&jack.x);
									}
									else{
										go_hit(&jack.x);
										if(jack.y==293) dropdead();
									}
									walk=0;
								break;
							}
							switch(keycode){
								case CUR_LEFT:
									go_left(&jack.x);
									walk=0;
								break;
								case CUR_RIGHT:
									go_right(&jack.x);
									walk=0;
								break;
								case ENTER: case RETURN: case CUR_UP:
									jumphole=checkjump(&jack.x);
									if(jumpflag){
										score+=10;
										printscore();
										go_jump(&jack.x);
									}
									else{
										go_hit(&jack.x);
										if(jack.y==293) dropdead();
									}
									walk=0;
								break;
							}
						}
						if (walk==1 && mx>=mxold-5 && mx<mxold) walk=0;
						if (walk==2 && mx<=mxold+5 && mx>mxold) walk=0;
						if (mx>mxold+5 || mx==maxx || walk==1){ walk=1; go_right(&jack.x);}
						if (mx<mxold-5 || mx==0 || walk==2){ walk=2; go_left(&jack.x);}
						draw_jack(jack.x, jack.y, jack.sprite);
					}
				}
				mxold=mx;
			}  /* if(helpflag) */
		}  /* if (top_window==wi_handle) */ 
		else{
			my_event=MU_MESAG | MU_BUTTON;
			time=0;
			show_mouse();
		}
		wind_update(FALSE);
	}  /* while (TheEnd) */
}
