#include <aes.h>

static char string_0[] = " \x0e" "\x0f" " ";
static char string_1[] = " File ";
static char string_2[] = "  Jumping Jack  ";
static char string_3[] = "----------------------";
static char string_4[] = "  Desk Accessory 1  ";
static char string_5[] = "  Desk Accessory 2  ";
static char string_6[] = "  Desk Accessory 3  ";
static char string_7[] = "  Desk Accessory 4  ";
static char string_8[] = "  Desk Accessory 5  ";
static char string_9[] = "  Desk Accessory 6  ";
static char string_10[] = "  Quit       ";
static char string_11[] = "THIS PROGRAM IS IN THE PUBLIC DOMAIN";
static char string_12[] = "";
static char string_13[] = "";
static char string_14[] = "MY ADDRESS:";
static char string_15[] = "";
static char string_16[] = "";
static char string_17[] = "15000 SABAC, YUGOSLAVIA";
static char string_18[] = "";
static char string_19[] = "";
static char string_20[] = "DUSANA OSTOJICA 10/10";
static char string_21[] = "";
static char string_22[] = "";
static char string_23[] = " USE IT AT YOUR OWN RISK ";
static char string_24[] = "";
static char string_25[] = "";
static char string_26[] = " OK ";


char *rs_frstr[] = {
0
};


BITBLK *rs_frimg[] = {
0
};


static TEDINFO rs_tedinfo[] = {
	{ string_11, string_12, string_13, 5, 6, 0, 0x1180, 0x0, -1, 37,1 },
	{ string_14, string_15, string_16, 5, 6, 0, 0x1180, 0x0, -1, 12,1 },
	{ string_17, string_18, string_19, 5, 6, 0, 0x1180, 0x0, -1, 24,1 },
	{ string_20, string_21, string_22, 5, 6, 0, 0x1180, 0x0, -1, 22,1 },
	{ string_23, string_24, string_25, 3, 6, 0, 0x1180, 0x0, -1, 26,1 },
};


OBJECT rs_object[] = {
/* MENU */

	{ -1, 1, 5, G_IBOX, NONE, NORMAL, (long)(0x0L), 0,0, 80,25 },
	{ 5, 2, 2, G_BOX, NONE, NORMAL, (long)(0x1100L), 0,0, 80,513 },
	{ 1, 3, 4, G_IBOX, NONE, NORMAL, (long)(0x0L), 2,0, 10,1 },
	{ 4, -1, -1, G_TITLE, NONE, NORMAL, (long)(string_0), 0,0, 4,769 },
	{ 2, -1, -1, G_TITLE, NONE, NORMAL, (long)(string_1), 4,0, 6,769 },
	{ 0, 6, 15, G_IBOX, NONE, NORMAL, (long)(0x0L), 0,769, 80,19 },
	{ 15, 7, 14, G_BOX, NONE, NORMAL, (long)(0xFF1100L), 2,0, 22,8 },
	{ 8, -1, -1, G_STRING, NONE, NORMAL, (long)(string_2), 0,0, 22,1 },
	{ 9, -1, -1, G_STRING, NONE, DISABLED, (long)(string_3), 0,1, 22,1 },
	{ 10, -1, -1, G_STRING, NONE, NORMAL, (long)(string_4), 0,2, 22,1 },
	{ 11, -1, -1, G_STRING, NONE, NORMAL, (long)(string_5), 0,3, 22,1 },
	{ 12, -1, -1, G_STRING, NONE, NORMAL, (long)(string_6), 0,4, 22,1 },
	{ 13, -1, -1, G_STRING, NONE, NORMAL, (long)(string_7), 0,5, 22,1 },
	{ 14, -1, -1, G_STRING, NONE, NORMAL, (long)(string_8), 0,6, 22,1 },
	{ 6, -1, -1, G_STRING, NONE, NORMAL, (long)(string_9), 0,7, 22,1 },
	{ 5, 16, 16, G_BOX, NONE, NORMAL, (long)(0xFF1100L), 6,0, 15,1 },
	{ 15, -1, -1, G_STRING, LASTOB, NORMAL, (long)(string_10), 0,0, 15,1 },

/* INFO */

	{ -1, 1, 7, G_BOX, NONE, SHADOWED, (long)(0x11100L), 0,0, 40,15 },
	{ 2, -1, -1, G_TEXT, NONE, NORMAL, (long)(&rs_tedinfo[0]), 1,12, 28,1 },
	{ 6, 3, 5, G_BOX, NONE, NORMAL, (long)(0xFF1100L), 19,9, 20,3 },
	{ 4, -1, -1, G_TEXT, NONE, NORMAL, (long)(&rs_tedinfo[1]), 513,3584, 776,3840 },
	{ 5, -1, -1, G_TEXT, NONE, NORMAL, (long)(&rs_tedinfo[2]), 1,2, 18,1 },
	{ 2, -1, -1, G_TEXT, NONE, NORMAL, (long)(&rs_tedinfo[3]), 513,1793, 17,1 },
	{ 7, -1, -1, G_BOXTEXT, NONE, SHADOWED, (long)(&rs_tedinfo[4]), 2,13, 25,1 },
	{ 0, -1, -1, G_BUTTON, SELECTABLE|DEFAULT|LASTOB, OUTLINED|SHADOWED, (long)(string_26), 31,13, 8,1 },
};


OBJECT *rs_trindex[] = {
	&rs_object[0],
	&rs_object[17],
};


int rs_numstrings = 27;
int rs_nuser = 0;
int rs_numfrstr = 0;
int rs_numimages = 0;
int rs_numbb = 0;
int rs_numfrimg = 0;
int rs_numib = 0;
int rs_numti = 5;
int rs_numobs = 25;
int rs_numtree = 2;

char rs_name[] = "JUMPING.RSC";
