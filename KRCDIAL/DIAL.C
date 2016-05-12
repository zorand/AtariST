#define T0OBJ 0
#define T1OBJ 19
#define T2OBJ 24
#define FREEBB 0
#define FREEIMG 0
#define FREESTR 57

char *rs_strings[] = {
"                  ",
"",
"",
"                  ",
"",
"",
"                  ",
"",
"",
"                  ",
"",
"",
"                  ",
"",
"",
"                  ",
"",
"",
"                  ",
"",
"",
"  KRC  ",
"",
"",
"About",
"Dial",
"Cancel",
"Setup",
"Add",
"(C)1993. Zoran Dimitrijevic",
"",
"",
" KRC Dialer ",
"",
"",
"KRC Research Ltd.",
"",
"",
"Use it at your own risk!",
"",
"",
"@@@@@@@@@@@@@@@@@@",
" Name: __________________",
"X",
"@@@@@@@@@@@@@@@@",
" Number: ________________",
"X",
"Save Settings",
"@@@",
" Redial after: ___",
"9",
"s",
"  KRC  ",
"",
"",
"Delete",
" OK "};

long rs_frstr[] = {
0};

BITBLK rs_bitblk[] = {
0};

long rs_frimg[] = {
0};

ICONBLK rs_iconblk[] = {
0};

TEDINFO rs_tedinfo[] = {
(char *)0L, (char *)1L, (char *)2L, 3, 6, 0, 0x1180, 0x0, 0, 19,1,
(char *)3L, (char *)4L, (char *)5L, 3, 6, 0, 0x1180, 0x0, 0, 19,1,
(char *)6L, (char *)7L, (char *)8L, 3, 6, 0, 0x1180, 0x0, 0, 19,1,
(char *)9L, (char *)10L, (char *)11L, 3, 6, 0, 0x1180, 0x0, 0, 19,1,
(char *)12L, (char *)13L, (char *)14L, 3, 6, 0, 0x1180, 0x0, 0, 19,1,
(char *)15L, (char *)16L, (char *)17L, 3, 6, 0, 0x1180, 0x0, 0, 19,1,
(char *)18L, (char *)19L, (char *)20L, 3, 6, 0, 0x1180, 0x0, 0, 19,1,
(char *)21L, (char *)22L, (char *)23L, 5, 6, 2, 0x1180, 0x0, -1, 8,1,
(char *)29L, (char *)30L, (char *)31L, 3, 6, 2, 0x1180, 0x0, -1, 28,1,
(char *)32L, (char *)33L, (char *)34L, 3, 6, 2, 0x1180, 0x0, -1, 13,1,
(char *)35L, (char *)36L, (char *)37L, 3, 6, 0, 0x1180, 0x0, -1, 18,1,
(char *)38L, (char *)39L, (char *)40L, 3, 6, 0, 0x1180, 0x0, -1, 25,1,
(char *)41L, (char *)42L, (char *)43L, 3, 6, 2, 0x1180, 0x0, -1, 19,26,
(char *)44L, (char *)45L, (char *)46L, 3, 6, 0, 0x1180, 0x0, -1, 17,26,
(char *)48L, (char *)49L, (char *)50L, 3, 6, 0, 0x1180, 0x0, -1, 4,19,
(char *)52L, (char *)53L, (char *)54L, 5, 6, 0, 0x1180, 0x0, -1, 8,1};

OBJECT rs_object[] = {
-1, 1, 18, G_BOX, NONE, NORMAL, 0x11121L, 1,1, 1053,9,
13, 2, 12, G_BOX, NONE, SHADOWED, 0xFF1101L, 1,1, 275,7,
4, 3, 3, G_BOX, NONE, NORMAL, 0xFF1141L, 274,1, 1,5,
2, -1, -1, G_BOX, NONE, NORMAL, 0xFF1101L, 0,0, 1,5,
5, -1, -1, G_BOXCHAR, SELECTABLE, NORMAL, 0x2FF1100L, 274,6, 1,1,
6, -1, -1, G_BOXCHAR, SELECTABLE, NORMAL, 0x1FF1100L, 274,0, 1,1,
7, -1, -1, G_BOXTEXT, SELECTABLE, NORMAL, 0x0L, 0,0, 18,1,
8, -1, -1, G_BOXTEXT, SELECTABLE, NORMAL, 0x1L, 0,1, 18,1,
9, -1, -1, G_BOXTEXT, SELECTABLE, NORMAL, 0x2L, 0,2, 18,1,
10, -1, -1, G_BOXTEXT, SELECTABLE, NORMAL, 0x3L, 0,3, 18,1,
11, -1, -1, G_BOXTEXT, SELECTABLE, NORMAL, 0x4L, 0,4, 18,1,
12, -1, -1, G_BOXTEXT, SELECTABLE, NORMAL, 0x5L, 0,5, 18,1,
1, -1, -1, G_BOXTEXT, SELECTABLE, NORMAL, 0x6L, 0,6, 18,1,
14, -1, -1, G_TEXT, NONE, NORMAL, 0x7L, 0,8, 1031,1,
15, -1, -1, G_BUTTON, SELECTABLE, SHADOWED, 0x18L, 790,3072, 6,1,
16, -1, -1, G_BUTTON, SELECTABLE, SHADOWED, 0x19L, 790,1794, 6,1,
17, -1, -1, G_BUTTON, SELECTABLE, SHADOWED, 0x1AL, 790,1543, 6,1,
18, -1, -1, G_BUTTON, SELECTABLE, SHADOWED, 0x1BL, 790,3331, 6,1,
0, -1, -1, G_BUTTON, 0x21, SHADOWED, 0x1CL, 790,773, 6,1,
-1, 1, 4, G_BOX, NONE, NORMAL, 0x11100L, 1025,3328, 1053,9,
2, -1, -1, G_TEXT, NONE, NORMAL, 0x8L, 1,3, 28,1,
3, -1, -1, G_BOXTEXT, NONE, SHADOWED, 0x9L, 9,1, 12,1,
4, -1, -1, G_TEXT, NONE, NORMAL, 0xAL, 11,4, 17,1,
0, -1, -1, G_TEXT, LASTOB, NORMAL, 0xBL, 3,7, 24,1,
-1, 1, 8, G_BOX, NONE, NORMAL, 0x11121L, 1537,2816, 1053,9,
2, -1, -1, G_FBOXTEXT, EDITABLE, SHADOWED, 0xCL, 1536,3841, 25,1,
3, -1, -1, G_FBOXTEXT, EDITABLE, SHADOWED, 0xDL, 1536,1795, 25,1,
4, -1, -1, G_BUTTON, SELECTABLE, SHADOWED, 0x2FL, 1039,1799, 13,1,
6, 5, 5, G_FBOXTEXT, EDITABLE, SHADOWED, 0xEL, 1536,3844, 20,1,
4, -1, -1, G_STRING, NONE, NORMAL, 0x33L, 1298,0, 1,1,
7, -1, -1, G_TEXT, NONE, NORMAL, 0xFL, 257,3591, 517,1,
8, -1, -1, G_BUTTON, SELECTABLE, NORMAL, 0x37L, 1812,1280, 8,1,
0, -1, -1, G_BUTTON, 0x21, SHADOWED, 0x38L, 777,1799, 1028,1};

long rs_trindex[] = {
0L,
19L,
24L};

struct foobar {
	int 	dummy;
	int 	*image;
	} rs_imdope[] = {
0};

#define NUM_STRINGS 57
#define NUM_FRSTR 0
#define NUM_IMAGES 0
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_TI 16
#define NUM_OBS 33
#define NUM_TREE 3

char pname[] = "DIAL.RSC";
