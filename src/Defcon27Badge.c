/*---------------------------------------------------------------------------------

	Gender Hackers DEF CON 27 The Badge The Game
	-BIG thanks to alekmaul and their Animated Sprite Demo that I used as a starting point-


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <string.h>

#include "soundbank.h"

extern char __SOUNDBANK__;

extern char soundbrr,soundbrrend;
brrsamples tadasound;

//---------------------------------------------------------------------
// The "Todd"(isaac) sprite (Hi Edmund <3, remember that time you told me i could mimic one of your sprites for a demo? Please dont sue me, we're friends)
//---------------------------------------------------------------------
extern char gfxpsrite, gfxpsrite_end;
extern char palsprite, palsprite_end;


//---------------------------------------------------------------------
// background
//---------------------------------------------------------------------
extern char patterns, patterns_end;
extern char palette;
extern char map, map_end;

//---------------------------------------------------------------------
// Font
//---------------------------------------------------------------------
extern char snesfont;

#define FRAMES_PER_ANIMATION 3 // 3 sprites per direction

//---------------------------------------------------------------------
// The "Todd" sprite
//---------------------------------------------------------------------
typedef struct
{
	short x, y;
	int gfx_frame;
	int state;
	int anim_frame;
	int flipx;
} Monster;

//---------------------------------------------------------------------
// The state of the sprite (which way it is walking)
//---------------------------------------------------------------------
enum SpriteState {W_DOWN = 0, W_UP = 1, W_RIGHT = 2,  W_LEFT = 2};

//---------------------------------------------------------------------
// Screen dimentions
//---------------------------------------------------------------------
enum {SCREEN_TOP = 1, SCREEN_BOTTOM = 208, SCREEN_LEFT = 1, SCREEN_RIGHT = 240};

char sprTiles[9]={0,2,4, 6,8,10, 12,14,32};  // Remeber that sprites are interleave with 128 pix width,
unsigned char strToasty[32]=   {'T','O','A','S','T','Y','!',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
unsigned char strStart[32]=    {' ',' ',' ',' ',' ',' ',' ',' ','[',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',']',' ',' ',' ',' ',' ',' ',' ',' '};
unsigned char strWhatever[33]= {' ',' ',' ','W','H','A','T','E','V','E','R','.','T','H','E','.','F','U','C','K','.','T','H','I','S','.','I','S',' ',' ',' ',' '};
unsigned char strPresents[32]= {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','P','r','e','s','e','n','t','s',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
unsigned char strGenhack[33]= {' ',' ',' ',' ',' ',' ',' ','-',' ','G','e','n','d','e','r',' ','H','a','c','k','e','r','s',' ','-',' ',' ',' ',' ',' ',' ',' '};


unsigned char lcount=0, pcount=0, konami=1, konamidone=0, lrToasty=0, toastydone=0, lrStart=0, startdone=0, lrWhatever=0, whateverdone=0, lrGenhack=0, genhackdone=0, todSpeed=1, btnDown=0;
unsigned short pad0; bgColor=0;
				// change this to a 1 to instantly solve the puzzle(s), like a cheater
bool bitStop=1; testing=0;

short skip = 0;

//---------------------------------------------------------------------------------
int main(void) {
	
	if (testing) konamidone=1;
	unsigned short pad0,i;
	Monster monster = {100,100};
	u16 scrX=0, scrY=0;
	
	// Initialize sound engine (take some time)
	spcBoot();
	
    // Initialize SNES 
	consoleInit();
	
	// Set give soundbank
	spcSetBank(&__SOUNDBANK__);
	
	// allocate way past the max amount of ram and see what happens
	spcAllocateSoundRegion(84);

	// Load music
	spcLoad(MOD_POLLEN8);
	
	// Load effect
	spcSetSoundEntry(15, 8, 4, &soundbrrend-&soundbrr, &soundbrr, &tadasound);
	
	// Init font
	consoleInitText(0, 1, &snesfont);
	
	// Now Put in 16 color mode and disable all backgrounds
	setMode(BG_MODE1,0); bgSetDisable(1);  bgSetDisable(2);
	setScreenOn();
	
	// Play file from the beginning
	spcPlay(0);
	
	//consoleNocashMessage(strToasty);
	
	while(lcount < 255) {
		lcount++;
		WaitForVBlank();
		if (lcount == 8) {
			pcount++;
			lcount=0;
		}
		if (pcount == 1) {
			consoleDrawText(0,5," -       Gender Hackers       - ");
		} else if (pcount == 2) {
			consoleDrawText(0,5,"  -      Gender Hackers      -  ");
		} else if (pcount == 3) {
			consoleDrawText(0,5,"   -     Gender Hackers     -   ");
		} else if (pcount == 4) {
			consoleDrawText(0,5,"    -    Gender Hackers    -    ");
		} else if (pcount == 5) {
			consoleDrawText(0,5,"     -   Gender Hackers   -     ");
		} else if (pcount == 6) {
			consoleDrawText(0,5,"      -  Gender Hackers  -      ");
		} else if (pcount == 7) {
			consoleDrawText(0,5,"       - Gender Hackers -       ");
		} else if (pcount == 8) {
			consoleDrawText(0,7,"            Presents            ");
		} else if (pcount == 10) { 
			consoleDrawText(0,10,"   WHATEVER.                    ");
		} else if (pcount == 12) { 
			consoleDrawText(0,10,"   WHATEVER.THE.                ");
		} else if (pcount == 14) { 
			consoleDrawText(0,10,"   WHATEVER.THE.FUCK.           ");
		} else if (pcount == 16) { 
			consoleDrawText(0,10,"   WHATEVER.THE.FUCK.THIS.      ");
		} else if (pcount == 18) { 
			consoleDrawText(0,10,"   WHATEVER.THE.FUCK.THIS.IS    ");
		} else if (pcount == 20) { 
			consoleDrawText(0,16,"        [ Press START ]        ");
		} else if (pcount == 25) { 
			consoleDrawText(0,16,"        [             ]        ");
		} else if (pcount == 30) { 
			pcount=20;
		}
		
		if (pcount > 19) {
			pad0 = padsCurrent(0);
			switch (pad0) {
				case KEY_A		: {
					//consoleDrawText(16,16,"A     "); 
					if ((konami == 19) || (konami == 23)) { // check for correct keypress
						konami++;
					} else if ((konami == 20) || (konami == 24)) { // ignore if key is held down
						// do nothing
					} else {  // you dun fucked up
						konami=0;
					}
					break;
				}
				case KEY_B		: {
					//consoleDrawText(16,16,"B     "); 
					if ((konami == 17) || (konami == 21)) { // check for correct keypress
						konami++;
					} else if ((konami == 18) || (konami == 22)) { // ignore if key is held down
						// do nothing
					} else {  // you dun fucked up
						konami=0;
					}
					break;
				}
				case KEY_SELECT	: {
					//consoleDrawText(16,16,"SELECT"); 
					if (konami == 25) { // check for correct keypress
						konami++;
					} else if (konami == 26) { // ignore if key is held down
						// do nothing
					} else {  // you dun fucked up
						konami=0;
					}
					break;
				}
				case KEY_START	: {
					//consoleDrawText(16,16,"START "); 
					if ((konami == 27) || (konami == 29)) { // check for third or forth correct keypress
						konami++;
					} else if ((konami == 28) || (konami == 30)) { // ignore if key is held down
						// do nothing
					}  else if (konami == 31) { // ignore if key is held down
						lcount=255;
					} else if (konami == 25) { // you got greedy
						konami=0;
					} else {  // you dun fucked up
						konami=0;
						lcount=255;
					}
					
					//lcount=255; 
					break;
				}
				case KEY_RIGHT	: {
					//consoleDrawText(16,16,">     ");
					if ((konami == 11) || (konami == 15)) { // check for correct keypress
						konami++;
					} else if ((konami == 12) || (konami == 16)) { // ignore if key is held down
						// do nothing
					} else {  // you dun fucked up
						konami=0;
					}
					break;
				}
				case KEY_LEFT	: {
					//consoleDrawText(16,16,"<     ");
					if ((konami == 9) || (konami == 13)) { // check for correct keypress
						konami++;
					} else if ((konami == 10) || (konami == 14)) { // ignore if key is held down
						// do nothing
					} else {  // you dun fucked up
						konami=0;
					}
					break;
				}
				case KEY_DOWN	: {
					//consoleDrawText(16,16,"v     ");
					if ((konami == 5) || (konami == 7)) { // check for correct keypress
						konami++;
					} else if ((konami == 6) || (konami == 8)) { // ignore if key is held down
						// do nothing
					} else {  // you dun fucked up
						konami=0;
					}
					break;
				}
				case KEY_UP		: {
					//consoleDrawText(16,16,"^     ");
					if ((konami == 1) || (konami == 3)) { // check for correct keypress
						konami++;
					} else if ((konami == 2) || (konami == 4)) { // ignore if key is held down
						// do nothing
					} else {  // you dun fucked up
						konami=0;
					}
					break;
				}
				case KEY_R		: {
					//consoleDrawText(16,16,"R     "); 
					break; 
				}
				case KEY_L		: {
					//consoleDrawText(16,16,"L     "); 
					break; 
				}
				case KEY_X		: {
					//consoleDrawText(16,16,"X     "); 
					break; 
				}
				case KEY_Y		: {
					//consoleDrawText(16,16,"Y     "); 
					break; 
				}
				default			: { 
					if (konami % 2 == 0) { // incriment correct responces only after button has been released
						konami++;
					}
					break;
				}
			}
			if (konami == 31) {
				//spcPlaySound(0);
				consoleDrawText(0,24,strToasty);
				consoleDrawText(0,16,"        [             ]        ");
				//consoleDrawText(0,24,"                      TOASTY!   ");
				// todo: save sram here
				konamidone=1; //im really hoping someone debugs this and just sets this to 1
			} else {
				consoleDrawText(0,24,"                                ");
			}
			
		}
	}
	
	// Copy tiles to VRAM
	bgInitTileSet(1, &patterns, &palette, 0, (&patterns_end - &patterns), 16*2, BG_16COLORS, 0x6000);
	
	// Copy Map to VRAM
	bgInitMapSet(1, &map, (&map_end - &map),SC_64x64, 0x1000);
	
	
	// Init Sprites gfx and palette with default size of 16x16
	oamInitGfxSet(&gfxpsrite, (&gfxpsrite_end-&gfxpsrite), &palsprite, 16*2, 0, 0x4000, OBJ_SIZE16);

	// Define sprites parameters
	oamSet(0,  monster.x, monster.y, 0, 0, 0, 0, 0); 
	oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
	oamSetVisible(0,OBJ_SHOW);
	
	
	// Initialize text console with our font
	
	//consoleDrawText(1,1,"Binding of Todd");
	
		// Now Put in 16 color mode and disable all backgrounds
	bgSetEnable(1);  bgSetEnable(2);
	setScreenOn();
	spcProcess();
	spcPlaySound(0);
	
	// Wait for nothing :P
	while(1) {
		// Refresh pad values in VBL and Get current #0 pad
		pad0 = padsCurrent(0);
		
		if (pad0) {
			// Update sprite with current pad
			if(pad0 & KEY_UP) {
				if(monster.y >= SCREEN_TOP) {
					monster.y-=todSpeed;
				} else {
					monster.y=SCREEN_BOTTOM;
					scrY-=256;
					if (konamidone==1) {
						strToasty[lrToasty]-=1; 
						if (strToasty[lrToasty]==31) strToasty[lrToasty]=127;
						consoleDrawText(0,24,strToasty);
					}
					if (toastydone==1) {
						strStart[lrStart]-=1; 
						if (strStart[lrStart]==31) strStart[lrStart]=127;
						consoleDrawText(0,16,strStart);
					}
					if (startdone==1) {
						strWhatever[lrWhatever]-=1; 
						if (strWhatever[lrWhatever]==31) strWhatever[lrWhatever]=127;
						consoleDrawText(0,10,strWhatever);
					}
					if (whateverdone==1) {
						strGenhack[lrGenhack]-=1; 
						if (strGenhack[lrGenhack]==31) strGenhack[lrGenhack]=127;
						consoleDrawText(0,5,strGenhack);
					}
					bgSetScroll(1, scrX,scrY);
				}
				monster.state = W_UP;
				monster.flipx = 0;
			}
			if(pad0 & KEY_LEFT) {
				if(monster.x >= SCREEN_LEFT) {
					monster.x-=todSpeed;
				} else {
					monster.x=SCREEN_RIGHT;
					scrX+=256;
					if (konamidone==1) lrToasty-=1;
					if (toastydone==1) lrStart-=1;
					if (startdone==1) lrWhatever-=1;
					if (whateverdone==1) lrGenhack-=1;
					bgSetScroll(1, scrX,scrY);
				}
				monster.state = W_LEFT;
				monster.flipx = 1;
			}
			if(pad0 & KEY_RIGHT) {
				if(monster.x <= SCREEN_RIGHT) {
					monster.x+=todSpeed;
				} else {
					monster.x=SCREEN_LEFT;
					scrX-=256;
					if (konamidone==1) lrToasty+=1;
					if (toastydone==1) lrStart+=1;
					if (startdone==1) lrWhatever+=1;
					if (whateverdone==1) lrGenhack+=1;
					bgSetScroll(1, scrX,scrY);
				}
				monster.state = W_LEFT;
				monster.flipx = 0;
			}
			if(pad0 & KEY_DOWN) {
				if(monster.y <= SCREEN_BOTTOM) {
					monster.y+=todSpeed;
				} else {
					monster.y=SCREEN_TOP;
					scrY+=256;
					
					if (konamidone==1) {
						strToasty[lrToasty]+=1; 
						if (strToasty[lrToasty]==128) strToasty[lrToasty]=32; 
						consoleDrawText(0,24,strToasty);
					}
					if (toastydone==1) {
						strStart[lrStart]+=1; 
						if (strStart[lrStart]==128) strStart[lrStart]=32; 
						consoleDrawText(0,16,strStart);
					}
					if (startdone==1) {
						strWhatever[lrWhatever]+=1;
						if (strWhatever[lrWhatever]==128) strWhatever[lrWhatever]=32;  
						consoleDrawText(0,10,strWhatever);
					}
					if (whateverdone==1) {
						strGenhack[lrGenhack]+=1;
						if (strGenhack[lrGenhack]==128) strGenhack[lrGenhack]=32;  
						consoleDrawText(0,5,strGenhack);
					}
					bgSetScroll(1, scrX,scrY);
				}
				monster.state = W_DOWN;
				monster.flipx = 0;
			}
			if(pad0 & KEY_R) {
				if(btnDown==0) todSpeed++;
			}
			if(pad0 & KEY_L) {
				if(btnDown==0) todSpeed--;
			}
			skip++;
			if(skip >= 3) {
				monster.anim_frame++;
				skip = 0;
				if (konamidone==1) {
					consoleDrawText(0,24,strToasty);
					if (testing) strncpy(strToasty, "Dan Forden                     ", 32); //remove, for testing
					if (strcmp(strToasty, "Dan Forden                     ")==0) {
						spcPlaySound(0);
						konamidone = 2;
						toastydone = 1;
						for (i=0; i<=12; i++) {
							bitStop = 1;
							while (bitStop) {
								switch (i) {
									case 0 : {
										strToasty[i]+=1;
										if (strToasty[i]=='Y') bitStop = 0;
										break;
									}
									case 1 : {
										strToasty[i]+=1;
										if (strToasty[i]=='o') bitStop = 0;
										break;
									}
									case 2 : {
										strToasty[i]+=1;
										if (strToasty[i]=='u') bitStop = 0;
										break;
									}
									case 3 : {
										//strToasty[i]-=1;
										if (strToasty[i]==' ') bitStop = 0;
										break;
									}
									case 4 : {
										strToasty[i]-=1;
										if (strToasty[i]=='A') bitStop = 0;
										break;
									}
									case 5 : {
										strToasty[i]+=1;
										if (strToasty[i]=='r') bitStop = 0;
										break;
									}
									case 6 : {
										strToasty[i]-=1;
										if (strToasty[i]=='e') bitStop = 0;
										break;
									}
									case 7 : {
										strToasty[i]-=1;
										if (strToasty[i]==' ') bitStop = 0;
										break;
									}
									case 8 : {
										strToasty[i]-=1;
										if (strToasty[i]=='V') bitStop = 0;
										break;
									}
									case 9 : {
										strToasty[i]-=1;
										if (strToasty[i]=='a') bitStop = 0;
										break;
									}
									case 10 : {
										strToasty[i]+=1;
										if (strToasty[i]=='l') bitStop = 0;
										break;
									}
									case 11 : {
										strToasty[i]+=1;
										if (strToasty[i]=='i') bitStop = 0;
										break;
									}
									case 12 : {
										strToasty[i]+=1;
										if (strToasty[i]=='d') bitStop = 0;
										break;
									}
								}
								consoleDrawText(0,24,strToasty);
								spcProcess();
								WaitForVBlank();
							}
						}
						
						
						for (i=0; i<=29; i++) {
							bitStop = 1;
							while (bitStop) {
								switch (i) {
									case 0 : {
										strStart[i]+=1;
										if (strStart[i]=='G') bitStop = 0;
										break;
									}
									case 1 : {
										strStart[i]+=1;
										if (strStart[i]=='r') bitStop = 0;
										break;
									}
									case 2 : {
										strStart[i]+=1;
										if (strStart[i]=='r') bitStop = 0;
										break;
									}
									case 3 : {
										strStart[i]+=1;
										if (strStart[i]=='&') bitStop = 0;
										break;
									}
									case 4 : {
										strStart[i]+=1;
										if (strStart[i]=='Z') bitStop = 0;
										break;
									}
									case 5 : {
										strStart[i]+=1;
										if (strStart[i]=='x') bitStop = 0;
										break;
									}
									case 6 : {
										strStart[i]+=1;
										if (strStart[i]=='g') bitStop = 0;
										break;
									}
									case 7 : {
										strStart[i]+=1;
										if (strStart[i]=='t') bitStop = 0;
										break;
									}
									case 8 : {
										strStart[i]+=1;
										if (strStart[i]=='y') bitStop = 0;
										break;
									}
									case 9 : {
										strStart[i]+=1;
										if (strStart[i]=='&') bitStop = 0;
										break;
									}
									case 10 : {
										strStart[i]+=1;
										if (strStart[i]=='V') bitStop = 0;
										break;
									}
									case 11 : {
										strStart[i]+=1;
										if (strStart[i]=='k') bitStop = 0;
										break;
									}
									case 12 : {
										strStart[i]+=1;
										if (strStart[i]=='u') bitStop = 0;
										break;
									}
									case 13 : {
										strStart[i]+=1;
										if (strStart[i]=='v') bitStop = 0;
										break;
									}
									case 14 : {
										strStart[i]+=1;
										if (strStart[i]=='r') bitStop = 0;
										break;
									}
									case 15 : {
										strStart[i]+=1;
										if (strStart[i]=='k') bitStop = 0;
										break;
									}
									case 16 : {
										strStart[i]+=1;
										if (strStart[i]=='&') bitStop = 0;
										break;
									}
									case 17 : {
										strStart[i]+=1;
										if (strStart[i]=='G') bitStop = 0;
										break;
									}
									case 18 : {
										strStart[i]+=1;
										if (strStart[i]=='x') bitStop = 0;
										break;
									}
									case 19 : {
										strStart[i]+=1;
										if (strStart[i]=='k') bitStop = 0;
										break;
									}
									case 20 : {
										strStart[i]+=1;
										if (strStart[i]=='&') bitStop = 0;
										break;
									}
									case 21 : {
										strStart[i]+=1;
										if (strStart[i]=='H') bitStop = 0;
										break;
									}
									case 22 : {
										strStart[i]+=1;
										if (strStart[i]=='k') bitStop = 0;
										break;
									}
									case 23 : {
										strStart[i]+=1;
										if (strStart[i]=='g') bitStop = 0;
										break;
									}
									case 24 : {
										strStart[i]+=1;
										if (strStart[i]=='{') bitStop = 0;
										break;
									}
									case 25 : {
										strStart[i]+=1;
										if (strStart[i]=='z') bitStop = 0;
										break;
									}
									case 26 : {
										strStart[i]+=1;
										if (strStart[i]=='o') bitStop = 0;
										break;
									}
									case 27 : {
										strStart[i]+=1;
										if (strStart[i]=='l') bitStop = 0;
										break;
									}
									case 28 : {
										strStart[i]+=1;
										if (strStart[i]=='{') bitStop = 0;
										break;
									}
									case 29 : {
										strStart[i]+=1;
										if (strStart[i]=='r') bitStop = 0;
										break;
									}
								}
								consoleDrawText(0,16,strStart);
								spcProcess();
								WaitForVBlank();
							}
						}
						
					}
					
				}
				
				if (toastydone==1) {
					if (testing) strncpy(strStart, "All Trans People Are Beautiful ", 32); //remove, for testing
					consoleDrawText(0,16,strStart);
					
					if (strcmp(strStart, "All Trans People Are Beautiful ")==0) {
						spcPlaySound(0);
						toastydone = 2;						
						startdone = 1;
						for (i=0; i<=31; i++) {
							bitStop = 1;
							while (bitStop) {
								switch (i) {
									case 0 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='R') bitStop = 0;
										break;
									}
									case 1 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='G') bitStop = 0;
										break;
									}
									case 2 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='9') bitStop = 0;
										break;
									}
									case 3 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='u') bitStop = 0;
										break;
									}
									case 4 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='d') bitStop = 0;
										break;
									}
									case 5 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='C') bitStop = 0;
										break;
									}
									case 6 : {
										strWhatever[i]-=1;
										if (strWhatever[i]=='B') bitStop = 0;
										break;
									}
									case 7 : {
										strWhatever[i]-=1;
										if (strWhatever[i]=='C') bitStop = 0;
										break;
									}
									case 8 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='Z') bitStop = 0;
										break;
									}
									case 9 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='S') bitStop = 0;
										break;
									}
									case 10 : {
										strWhatever[i]-=1;
										if (strWhatever[i]=='B') bitStop = 0;
										break;
									}
									case 11 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='B') bitStop = 0;
										break;
									}
									case 12 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='Z') bitStop = 0;
										break;
									}
									case 13 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='n') bitStop = 0;
										break;
									}
									case 14 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='J') bitStop = 0;
										break;
									}
									case 15 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='h') bitStop = 0;
										break;
									}
									case 16 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='a') bitStop = 0;
										break;
									}
									case 17 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='W') bitStop = 0;
										break;
									}
									case 18 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='Q') bitStop = 0;
										break;
									}
									case 19 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='g') bitStop = 0;
										break;
									}
									case 20 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='V') bitStop = 0;
										break;
									}
									case 21 : {
										strWhatever[i]-=1;
										if (strWhatever[i]=='G') bitStop = 0;
										break;
									}
									case 22 : {
										strWhatever[i]-=1;
										if (strWhatever[i]=='8') bitStop = 0;
										break;
									}
									case 23 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='g') bitStop = 0;
										break;
									}
									case 24 : {
										strWhatever[i]-=1;
										if (strWhatever[i]=='Q') bitStop = 0;
										break;
									}
									case 25 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='m') bitStop = 0;
										break;
									}
									case 26 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='U') bitStop = 0;
										break;
									}
									case 27 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='g') bitStop = 0;
										break;
									}
									case 28 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='W') bitStop = 0;
										break;
									}
									case 29 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='W') bitStop = 0;
										break;
									}
									case 30 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='9') bitStop = 0;
										break;
									}
									case 31 : {
										strWhatever[i]+=1;
										if (strWhatever[i]=='1') bitStop = 0;
										break;
									}
								}
								consoleDrawText(0,10,strWhatever);
								spcProcess();
								WaitForVBlank();
							}
						}
						
					}
					
				}
				
				
				
				if (startdone==1) {
					if (testing) strncpy(strWhatever, "Dont Be Afraid To Be You        ", 32); //remove, for testing
					consoleDrawText(0,10,strWhatever);
					
					if (strcmp(strWhatever, "Dont Be Afraid To Be You        ")==0) {
						spcPlaySound(0);
						startdone = 2;						
						whateverdone = 1;
						for (i=12; i<=19; i++) {
							bitStop = 1;
							while (bitStop) {
								switch (i) {
									case 12 : {
										strPresents[i]-=1;
										if (strPresents[i]==' ') bitStop = 0;
										break;
									}
									case 13 : {
										strPresents[i]-=1;
										if (strPresents[i]==' ') bitStop = 0;
										break;
									}
									case 14 : {
										strPresents[i]-=1;
										if (strPresents[i]==' ') bitStop = 0;
										break;
									}
									case 15 : {
										strPresents[i]-=1;
										if (strPresents[i]==' ') bitStop = 0;
										break;
									}
									case 16 : {
										strPresents[i]-=1;
										if (strPresents[i]==' ') bitStop = 0;
										break;
									}
									case 17 : {
										strPresents[i]-=1;
										if (strPresents[i]==' ') bitStop = 0;
										break;
									}
									case 18 : {
										strPresents[i]-=1;
										if (strPresents[i]==' ') bitStop = 0;
										break;
									}
									case 19 : {
										strPresents[i]-=1;
										if (strPresents[i]==' ') bitStop = 0;
										break;
									}
								}
								consoleDrawText(0,7,strPresents);
								spcProcess();
								WaitForVBlank();
							}
						}
						
						for (i=0; i<=31; i++) {
							bitStop = 1;
							while (bitStop) {
								switch (i) {
									case 0 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='4') bitStop = 0;
										break;
									}
									case 1 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='F') bitStop = 0;
										break;
									}
									case 2 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='8') bitStop = 0;
										break;
									}
									case 3 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='2') bitStop = 0;
										break;
									}
									case 4 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='8') bitStop = 0;
										break;
									}
									case 5 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='B') bitStop = 0;
										break;
									}
									case 6 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='3') bitStop = 0;
										break;
									}
									case 7 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='8') bitStop = 0;
										break;
									}
									case 8 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='C') bitStop = 0;
										break;
									}
									case 9 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='6') bitStop = 0;
										break;
									}
									case 10 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='4') bitStop = 0;
										break;
									}
									case 11 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='C') bitStop = 0;
										break;
									}
									case 12 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='0') bitStop = 0;
										break;
									}
									case 13 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='7') bitStop = 0;
										break;
									}
									case 14 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='C') bitStop = 0;
										break;
									}
									case 15 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='2') bitStop = 0;
										break;
									}
									case 16 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='C') bitStop = 0;
										break;
									}
									case 17 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='0') bitStop = 0;
										break;
									}
									case 18 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='D') bitStop = 0;
										break;
									}
									case 19 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='6') bitStop = 0;
										break;
									}
									case 20 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='0') bitStop = 0;
										break;
									}
									case 21 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='C') bitStop = 0;
										break;
									}
									case 22 : {
										strGenhack[i]-=1;
										if (strGenhack[i]=='F') bitStop = 0;
										break;
									}
									case 23 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='6') bitStop = 0;
										break;
									}
									case 24 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='9') bitStop = 0;
										break;
									}
									case 25 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='5') bitStop = 0;
										break;
									}
									case 26 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='C') bitStop = 0;
										break;
									}
									case 27 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='C') bitStop = 0;
										break;
									}
									case 28 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='4') bitStop = 0;
										break;
									}
									case 29 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='0') bitStop = 0;
										break;
									}
									case 30 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='D') bitStop = 0;
										break;
									}
									case 31 : {
										strGenhack[i]+=1;
										if (strGenhack[i]=='9') bitStop = 0;
										break;
									}
								}
								consoleDrawText(0,5,strGenhack);
								spcProcess();
								WaitForVBlank();
							}
						}
						
					}
					
				}
				
				
				
				if (whateverdone==1) {
					if (testing) strncpy(strGenhack, "Trans Rights Are Human Rights   ", 32); //remove, for testing
					consoleDrawText(0,5,strGenhack);
					
					if (strcmp(strGenhack, "Trans Rights Are Human Rights   ")==0) {
						spcPlaySound(0);
						whateverdone = 2;						
						genhackdone = 1;
						consoleDrawText(0,1,"Congrats on completing the");
						consoleDrawText(0,2,"Defcon27 GenderHackers Challenge");
						consoleDrawText(0,5,"                                ");
						consoleDrawText(0,6,"Take a fucking screenshot       ");
						consoleDrawText(0,10,"                                ");
						consoleDrawText(0,11,"Brag on twitter @GenderHackers  ");
						consoleDrawText(0,16,"                                ");
						consoleDrawText(0,17,"and dont post any spoilers! <3  ");
						if (testing) consoleDrawText(0,24,"Also, I know that you cheated ;)");
						bgSetDisable(1);  bgSetDisable(2);
					}
					
				}
				//consoleDrawText(1,1,"SCR X=%d Y=%d   ",monster.x,monster.y);
				
			}
			
			if(monster.anim_frame >= FRAMES_PER_ANIMATION) monster.anim_frame = 0;
			
			
			btnDown = 1;
		} else { //if no buttons pressed
			btnDown = 0;
		
		}
		
		//consoleNocashMessage(strToasty);
		if (genhackdone) {
			bgColor++;
			setPaletteColor(0x00, bgColor);
		}
		
		// Now, get current sprite in current animation
		monster.gfx_frame = sprTiles[monster.anim_frame + monster.state*FRAMES_PER_ANIMATION ];
		oamSet(0,  monster.x, monster.y, 3, monster.flipx, 0, monster.gfx_frame, 0);
		
		// Wait VBL 'and update sprites too ;-) )
		spcProcess();
		WaitForVBlank();
	}
	return 0;
}
