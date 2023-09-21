/*
The Maze Game

Maze game programmed on stm32f031 using TFT display

. Player must collect all three coins and open gate to reach treasure,
. Player opens gate by pulling lever using fifth additional button
. If Player hits any of the maze walls they are spawned back to the original
  position with all of the coins collected still in the inventory

Authors: Haroun Kassouri C21508813 and Andrei Voiniciuc C21342953
Date: 20/11/2022

Youtube link: https://www.youtube.com/watch?v=5pN85SJk0Z0

*/

#include <stm32f031x6.h>
#include "display.h"
#include "musical_notes.h"
#define SIZE 4
void initClock(void);
void initSysTick(void);
void initTimer(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
static volatile uint32_t milliseconds;
void playNote(uint32_t Freq, uint32_t duration);
void power_saving_delay(uint32_t dly);
int isInside(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void pluscoin(void);
void minuslife(void);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void DrawMaze(void);
void mazeinteraction(void);

int isInside(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x+w;
	y2 = y+h;
	int rvalue = 0;
	if ( (px >= x) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

//Pixel images
static const uint16_t coin[]=
{
296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,65286,65286,65286,65286,65286,65286,296,296,296,296,296,65286,65286,24327,24327,24327,24327,65286,65286,296,296,296,65286,65286,24327,24327,24327,24327,24327,24327,65286,65286,296,296,65286,24327,24327,24327,24327,24327,32517,24327,24327,65286,296,296,65286,24327,24327,32517,24327,24327,32517,24327,24327,65286,296,296,65286,24327,32517,32517,32517,24327,32517,24327,24327,65286,296,296,65286,24327,24327,32517,24327,24327,32517,24327,24327,65286,296,296,65286,65286,24327,24327,24327,24327,24327,24327,65286,65286,296,296,296,65286,65286,24327,24327,24327,24327,65286,65286,296,296,296,296,296,65286,65286,65286,65286,65286,65286,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,
};
static const uint16_t jeff1[]=
{
296,296,26649,26649,26649,296,296,296,296,26649,26649,26649,26649,26649,296,296,26649,26649,26649,64364,51701,64364,296,296,26649,26649,64364,64364,64364,64364,64332,296,26649,26649,64364,64364,26649,26649,296,296,26649,26649,64364,26649,64364,296,296,296,296,26649,64752,64752,64752,296,296,296,296,64752,64752,49575,64752,64752,64752,64752,296,64752,64752,49575,49575,49575,49575,64752,296,64752,64752,64752,64752,64752,64752,64752,296,296,64752,49575,64752,296,296,296,296,296,64752,49575,64752,296,296,296,296,296,64752,64752,64752,296,296,296,296,296,57598,57598,57598,57598,296,296,
};
static const uint16_t jeff2[]=
{
296,296,296,26649,26649,26649,296,296,296,296,26649,26649,26649,26649,26649,296,296,26649,26649,26649,64364,51701,64364,296,296,26649,26649,64364,64364,64364,64364,64332,296,26649,26649,64364,64364,26649,26649,296,296,26649,26649,64364,26649,64364,296,296,296,296,26649,64752,64752,64752,296,296,296,64752,64752,49575,64752,64752,64752,64752,296,64752,64752,49575,49575,49575,49575,64752,296,64752,64752,64752,64752,64752,64752,64752,296,296,64752,64752,49575,64752,296,296,296,64752,64752,64752,49575,64752,296,296,57598,64752,64752,64752,64752,64752,296,296,296,57598,57598,57598,57598,57598,57598,296,
};
static const uint16_t jeff3[]=
{
296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,26649,26649,296,296,296,296,296,42769,42769,25352,42769,296,296,64752,64752,42769,25352,42769,42769,64752,64752,49575,42769,25352,42769,42769,25352,42769,49575,64752,26649,26649,42769,25352,26649,26649,64752,296,296,26649,25352,26649,26649,296,296,296,296,296,26649,26649,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,296,
};
static const uint16_t lever[]=
{
51266,51266,51266,51266,51266,51266,51266,51266,296,296,51266,19555,19555,19555,19555,19555,19555,51266,296,296,51266,19555,0,0,59905,59905,59905,59905,7936,7936,51266,19555,0,0,59905,59905,59905,59905,7936,7936,51266,19555,19555,19555,19555,19555,19555,51266,296,296,51266,51266,51266,51266,51266,51266,51266,51266,296,296,
};
static const uint16_t lever2[]=
{
51266,51266,51266,51266,51266,51266,51266,51266,296,296,51266,19555,19555,19555,19555,19555,19555,51266,296,296,51266,19555,0,7936,7936,0,19555,51266,296,296,51266,19555,0,7936,7936,0,19555,51266,296,296,51266,19555,19555,19555,19555,19555,19555,51266,296,296,51266,51266,51266,51266,51266,51266,51266,51266,296,296,
};
static const uint16_t win[]=
{
296,296,296,296,296,296,48901,65287,65287,48901,296,296,296,296,296,296,48901,296,296,296,296,296,296,48901,48901,296,296,296,296,296,296,48901,48901,65287,296,296,296,296,48901,65287,65287,48901,296,296,296,296,65287,48901,48901,65287,65287,48901,48901,65287,65287,65287,65287,65287,65287,48901,48901,65287,65287,48901,48901,65287,65287,65287,65287,65287,65287,24322,24322,65287,65287,65287,65287,65287,65287,48901,48901,48901,65287,65287,65287,65287,24322,7936,7936,24322,65287,65287,65287,65287,48901,48901,296,48901,65287,65287,65287,24322,7936,7936,7936,7936,24322,65287,65287,65287,48901,296,296,48901,65287,65287,65287,24322,7936,7936,7936,7936,24322,65287,65287,65287,48901,296,296,48901,48901,65287,65287,24322,7936,7936,7936,7936,24322,65287,65287,48901,48901,296,296,296,48901,65287,65287,65287,24322,7936,7936,24322,65287,65287,65287,48901,296,296,296,296,48901,48901,65287,65287,65287,24322,24322,65287,65287,65287,48901,48901,296,296,296,296,48901,48901,48901,48901,65287,65287,65287,65287,48901,48901,48901,48901,296,296,
};
static const uint16_t coinhud[]=
{
16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,65286,65286,65286,65286,65286,65286,16648,16648,16648,16648,16648,65286,65286,24327,24327,24327,24327,65286,65286,16648,16648,16648,65286,65286,24327,24327,24327,24327,24327,24327,65286,65286,16648,16648,65286,24327,24327,24327,24327,24327,32517,24327,24327,65286,16648,16648,65286,24327,24327,32517,24327,24327,32517,24327,24327,65286,16648,16648,65286,24327,32517,32517,32517,24327,32517,24327,24327,65286,16648,16648,65286,24327,24327,32517,24327,24327,32517,24327,24327,65286,16648,16648,65286,65286,24327,24327,24327,24327,24327,24327,65286,65286,16648,16648,16648,65286,65286,24327,24327,24327,24327,65286,65286,16648,16648,16648,16648,16648,65286,65286,65286,65286,65286,65286,16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,16648,
};

int main()
{
	int hinverted = 0;
	int vinverted = 0;
	int toggle = 0;
	int levertoggle = 0;
	int hmoved = 0;
	int vmoved = 0;
	uint16_t x = 10; // deco's starting x and y coords
	uint16_t y = 140;
	uint16_t oldx = x;
	uint16_t oldy = y;
	int coin1 = 0; // "if coin is collected" variable
	int coin2 = 0;
	int coin3 = 0;
	uint16_t coincount = 0;
	
	initClock();
	initSysTick();
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOB,0,0); //enables input for extra button (Port B bit 0)
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
	enablePullUp(GPIOB,0);//enables pullup for extra button (Port B bit 0)
	
	//Title screen, Introduction to game	
	fillRectangle(0,0,128,160,RGBToWord(0,0,0));
	printTextX2("The Maze", 15,20,RGBToWord(255,255,0),RGBToWord(0,0,0));
	printTextX2("Game", 40,40,RGBToWord(255,255,0),RGBToWord(0,0,0));
	printText("Developed by:", 20,70,RGBToWord(255,255,255),RGBToWord(0,0,0));
	printText("Andrei Voiniciuc", 10,85,RGBToWord(255,0,255),RGBToWord(0,0,0));
	printText("C21342953", 35,95,RGBToWord(255,0,255),RGBToWord(0,0,0));
	printText("&",63,105,RGBToWord(255,0,255),RGBToWord(0,0,0));
	printText("Haroun Kassouri", 13,115,RGBToWord(255,0,255),RGBToWord(0,0,0));
	printText("C21508813", 35,125,RGBToWord(255,0,255),RGBToWord(0,0,0));
	
	delay(3500);//Appear for three and a half seconds
	
	//Objective screen, (Tells user how to win game)
	fillRectangle(0,0,128,160,RGBToWord(0,0,0));
	putImage(20,20,8,14,jeff1,hinverted,0);
	putImage(100,20,8,14,jeff1,hinverted,0);
	printTextX2("Goal:", 40,20,RGBToWord(255,255,0),RGBToWord(0,0,0));
	printText("COLLECT ALL 3", 20,50,RGBToWord(255,0,255),RGBToWord(0,0,0));
	printText("COINS TO OPEN", 15,65,RGBToWord(255,0,255),RGBToWord(0,0,0));
	printText("THE GATE BY", 20,80,RGBToWord(255,0,255),RGBToWord(0,0,0));
	printText("PULLING LEVER", 15,95,RGBToWord(255,0,255),RGBToWord(0,0,0));
	printText("AND COLLECT",20,110,RGBToWord(255,0,255),RGBToWord(0,0,0));
	printText("THE TREASURE!", 20,125,RGBToWord(255,0,255),RGBToWord(0,0,0));
	
	delay(5000);//Appear for five seconds
	
	
	//HUD Display at the top of screen
	fillRectangle(0,0,128,160,RGBToWord(10,0,45)); // dark purple background
	fillRectangle(0,0,128,13,RGBToWord(10,10,10)); // dark gray HUD
	fillRectangle(32,0,2,13,RGBToWord(0,0,0));
	putImage(1,1,12,12,coinhud,0,0); // coin counter
	
	DrawMaze();//This function will draw the maze structure, All the maze walls
	
	putImage(36,123,12,12,coin,0,0); // coin 1
	putImage(62,26,12,12,coin,0,0); // coin 2
	putImage(80,80,12,12,coin,0,0); // coin 3
	
	putImage(83,24,10,6,lever2,0,0); // lever 
	putImage(111,26,16,12,win,0,0); // crown (win)
	
	fillRectangle(102,18,2,20,RGBToWord(40,40,40)); // The Gate

	
	initTimer();
	while(1)
	{
		hmoved = vmoved = 0;
		hinverted = vinverted = 0;
		
		if ((GPIOB->IDR & (1 << 4))==0) // Right button
		{					
			if (x < 110)
			{
				x = x + 1;
				hmoved = 1;
				hinverted=0;
			}
		}
		if ((GPIOB->IDR & (1 << 5))==0) // Left button
		{			
			
			if (x > 10)
			{
				x = x - 1;
				hmoved = 1;
				hinverted=1;
			}			
		}
		if ( (GPIOA->IDR & (1 << 11)) == 0) // Up button
		{
			if (y < 140)
			{
				y = y + 1;			
				vmoved = 1;
				vinverted = 0;
			}
		}
		if ( (GPIOA->IDR & (1 << 8)) == 0) // Down button
		{			
			if (y > 16)
			{
				y = y - 1;
				vmoved = 1;
				vinverted = 1;
			}
		}
		
		// Coin Counter in HUD, (Will display the number of coins user collected)
		printText("=", 15, 3, RGBToWord(0xff,0xff,0), RGBToWord(10,10,10));
		if(coincount == 0)
		{
			printText("0", 23, 3, RGBToWord(0xff,0xff,0), RGBToWord(10,10,10));
		}
		else if(coincount == 1)
		{
			printText("1", 23, 3, RGBToWord(0xff,0xff,0), RGBToWord(10,10,10));
		}
		else if(coincount == 2)
		{
			printText("2", 23, 3, RGBToWord(0xff,0xff,0), RGBToWord(10,10,10));
		}
		else
		{
			printText("3", 23, 3, RGBToWord(0xff,0xff,0), RGBToWord(10,10,10));
		}
	
		// Lever interaction using 5th button 
		if (isInside(83,24,15,15,x,y) || isInside(83,24,15,15,x+8,y) || isInside(83,24,15,15,x,y+14) || isInside(83,24,15,15,x+8,y+14))// If character image is within the area close to the lever 15x15 pixels near, not touching the lever
		{
			if (isInside(83,24,5,5,x,y) || isInside(83,24,5,5,x+8,y) || isInside(83,24,5,5,x,y+7) || isInside(83,24,5,5,x+8,y+7)) // If character image is exactly inside lever image, new lever image is not pasted above character image
			{
				if ( (GPIOB->IDR & (1 << 0)) == 0)
				{
					playNote(A4, 175);
					playNote(A4, 175);
					playNote(A4, 175);
					levertoggle = levertoggle ^ 1;
				}
			}
			else // if character image is beside the lever
			{
				if ( (GPIOB->IDR & (1 << 0)) == 0)//Fifth button connected to port B bit 0
				{
					if (levertoggle) 
					{
						putImage(83,24,10,6,lever,0,0); // Lever toggled image
					}
					else
					{
						putImage(83,24,10,6,lever2,0,0); // Lever untoggled image 
					}
					playNote(A4, 175);
					playNote(A4, 175);
					playNote(A4, 175);
					levertoggle = levertoggle ^ 1;
				}
			}
		}
		else // used to replace the empty pixels with the current image of the lever once character leaves area.
		{
			if (levertoggle) 
			{
				putImage(83,24,10,6,lever,0,0);
			}
			else
			{
				putImage(83,24,10,6,lever2,0,0);
			}
		}
		
		
		if ((vmoved) || (hmoved)) // If any sort of movement is detected, stuff happens inside here
		{
			fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45)); // "Eating the pixels", it just replaces deco's previous position with background colour
			oldx = x;
			oldy = y;					
			if (hmoved)
			{
				if (toggle) // the alternating images giving illusion of deco walking
					putImage(x,y,8,14,jeff1,hinverted,0);
				else
					putImage(x,y,8,14,jeff2,hinverted,0);
				
				toggle = toggle ^ 1; // constantly alternates as long as he's moving horizontally
			}
			else // if deco's vertically moved, show the hat only
			{
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			// Coins interaction
			if (coin1 == 0) // once coin 1 is collected it disappears
			{	
				if (isInside(36,123,12,12,x,y) || isInside(36,123,12,12,x+8,y) || isInside(36,123,12,12,x,y+14) || isInside(36,123,12,12,x+8,y+14)) // when this statement is inside this "movement" if statement, it will only trigger when movement is detected. if placed outside then it will constantly trigger as long as deco's in the "coin hitbox"
				{
					fillRectangle(36,123,12,12,RGBToWord(10,0,45));
					pluscoin();
					coincount++;
					coin1++;
				}
			}
			if (coin2 == 0) // once coin 2 is collected it disappears
			{	
				if (isInside(62,26,12,12,x,y) || isInside(62,26,12,12,x+8,y) || isInside(62,26,12,12,x,y+14) || isInside(62,26,12,12,x+8,y+14)) // when this statement is inside this "movement" if statement, it will only trigger when movement is detected. if placed outside then it will constantly trigger as long as deco's in the "coin hitbox"
				{
					fillRectangle(62,26,12,12,RGBToWord(10,0,45));
					pluscoin();
					coincount++;
					coin2++;
				}
			}
			if (coin3 == 0) // once coin 3 is collected it disappears
			{	
				if (isInside(80,80,12,12,x,y) || isInside(80,80,12,12,x+8,y) || isInside(80,80,12,12,x,y+14) || isInside(80,80,12,12,x+8,y+14)) // when this statement is inside this "movement" if statement, it will only trigger when movement is detected. if placed outside then it will constantly trigger as long as deco's in the "coin hitbox"
				{
					fillRectangle(80,80,12,12,RGBToWord(10,0,45));
					pluscoin();
					coincount++;
					coin3++;
				}
			}
			// Gate interaction
			if (isInside(102,18,2,20,x,y) || isInside(102,18,2,20,x+8,y) || isInside(102,18,2,20,x,y+14) || isInside(102,18,2,20,x+8,y+14))
			{
				if (coincount == 3) // if all three coins have been collected
				{
					if (levertoggle == 1) // if user has used fifth button to toggle lever
					{
						fillRectangle(102,18,2,20,RGBToWord(10,0,45));
						fillRectangle(102,18,20,2,RGBToWord(40,40,40));
					}
					else // if user hasn't used fifth button to toggle lever
					{
						printText("Flick lever!", 36, 3, RGBToWord(0xff,0xff,0), RGBToWord(10,10,10));
						playNote(D4, 175);
						playNote(D4, 175);
						playNote(D4, 175);
						fillRectangle(36,0,92,12,RGBToWord(10,10,10));
						fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
						x = 83;
						y = 22;
						putImage(x,y,8,14,jeff3,0,vinverted);
					}
				}
				else // else (if user hasn't collected all three)
				{
					printText("Get all coins", 36, 3, RGBToWord(0xff,0xff,0), RGBToWord(10,10,10));
					playNote(G4, 175);
					playNote(G4, 175);
					playNote(G4, 175);
					fillRectangle(36,0,92,12,RGBToWord(10,10,10));
					fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
					x = 83;
					y = 22;
					putImage(x,y,8,14,jeff3,0,vinverted);
					
				}
			}
			// Maze walls interaction
			if (isInside(21,35,3,102,x,y) || isInside(21,35,3,102,x+8,y) || isInside(21,35,3,102,x,y+14) || isInside(21,35,3,102,x+8,y+14))
			{
				minuslife(); //
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted); //places character image back to original position
			}
			if (isInside(21,35,25,3,x,y) || isInside(21,35,25,3,x+8,y) || isInside(21,35,25,3,x,y+14) || isInside(21,35,25,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(21,134,10,3,x,y) || isInside(21,134,10,3,x+8,y) || isInside(21,134,10,3,x,y+14) || isInside(21,134,10,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(31,118,3,19,x,y) || isInside(31,118,3,19,x+8,y) || isInside(31,118,3,19,x,y+14) || isInside(31,118,3,19,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(31,118,19,3,x,y) || isInside(31,118,19,3,x+8,y) || isInside(31,118,19,3,x,y+14) || isInside(31,118,19,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(49,118,3,19,x,y) || isInside(49,118,3,19,x+8,y) || isInside(49,118,3,19,x,y+14) || isInside(49,118,3,19,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(49,134,10,3,x,y) || isInside(49,134,10,3,x+8,y) || isInside(49,134,10,3,x,y+14) || isInside(49,134,10,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(5,15,3,143,x,y) || isInside(5,15,3,143,x+8,y) || isInside(5,15,3,143,x,y+14) || isInside(5,15,3,143,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(5,15,53,3,x,y) || isInside(5,15,53,3,x+8,y) || isInside(5,15,53,3,x,y+14) || isInside(5,15,53,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(58,15,3,38,x,y) || isInside(58,15,3,38,x+8,y) || isInside(58,15,3,38,x,y+14) || isInside(58,15,3,38,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(21,155,102,3,x,y) || isInside(21,155,102,3,x+8,y) || isInside(21,155,102,3,x,y+14) || isInside(21,155,102,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(43,35,3,15,x,y) || isInside(43,35,3,15,x+8,y) || isInside(43,35,3,15,x,y+14) || isInside(43,35,3,15,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(43,50,18,3,x,y) || isInside(43,50,18,3,x+8,y) || isInside(43,50,18,3,x,y+14) || isInside(43,50,18,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(120,76,3,79,x,y) || isInside(120,76,3,79,x+8,y) || isInside(120,76,3,79,x,y+14) || isInside(120,76,3,79,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(59,109,3,28,x,y) || isInside(59,109,3,28,x+8,y) || isInside(59,109,3,28,x,y+14) || isInside(59,109,3,28,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(74,91,3,46,x,y) || isInside(74,91,3,46,x+8,y) || isInside(74,91,3,46,x,y+14) || isInside(74,91,3,46,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			if (isInside(74,134,30,3,x,y) || isInside(74,134,30,3,x+8,y) || isInside(74,134,30,3,x,y+14) || isInside(74,134,30,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(24,109,38,3,x,y) || isInside(24,109,38,3,x+8,y) || isInside(24,109,38,3,x,y+14) || isInside(24,109,38,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(39,88,38,3,x,y) || isInside(39,88,38,3,x+8,y) || isInside(39,88,38,3,x,y+14) || isInside(39,88,38,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(39,73,58,3,x,y) || isInside(39,73,58,3,x+8,y) || isInside(39,73,58,3,x,y+14) || isInside(39,73,58,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(39,73,3,15,x,y) || isInside(39,73,3,15,x+8,y) || isInside(39,73,3,15,x,y+14) || isInside(39,73,3,15,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(76,15,3,38,x,y) || isInside(76,15,3,38,x+8,y) || isInside(76,15,3,38,x,y+14) || isInside(76,15,3,38,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(94,38,3,37,x,y) || isInside(94,38,3,37,x+8,y) || isInside(94,38,3,37,x,y+14) || isInside(94,38,3,37,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(78,15,28,3,x,y) || isInside(78,15,28,3,x+8,y) || isInside(78,15,28,3,x,y+14) || isInside(78,15,28,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(94,38,12,3,x,y) || isInside(94,38,12,3,x+8,y) || isInside(94,38,12,3,x,y+14) || isInside(94,38,12,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(105,97,3,40,x,y) || isInside(105,97,3,40,x+8,y) || isInside(105,97,3,40,x,y+14) || isInside(105,97,3,40,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(75,97,30,3,x,y) || isInside(75,97,30,3,x+8,y) || isInside(75,97,30,3,x,y+14) || isInside(75,97,30,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(75,73,48,3,x,y) || isInside(75,73,48,3,x+8,y) || isInside(75,73,48,3,x,y+14) || isInside(75,73,48,3,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
      if (isInside(74,73,3,15,x,y) || isInside(74,73,3,15,x+8,y) || isInside(74,73,3,15,x,y+14) || isInside(74,73,3,15,x+8,y+14))
			{
				minuslife();
				fillRectangle(oldx,oldy,8,14,RGBToWord(10,0,45));
				x = 10;
				y = 140;
				putImage(x,y,8,14,jeff3,0,vinverted);
			}
			// Winning interaction, If user touches crown i.e. if user wins the game
			if (isInside(111,26,16,12,x,y) || isInside(111,26,16,12,x+8,y) || isInside(111,26,16,12,x,y+14) || isInside(111,26,16,12,x+8,y+14))
			{
				fillRectangle(0,0,128,160,RGBToWord(0,0,0));
				printTextX2("You Won!", 17,10,RGBToWord(255,0,255),RGBToWord(0,0,0));
				printTextX2("The Maze", 15,30,RGBToWord(255,255,0),RGBToWord(0,0,0));
				printTextX2("Game", 40,50,RGBToWord(255,255,0),RGBToWord(0,0,0));
				printText("Developed by:", 20,70,RGBToWord(255,255,255),RGBToWord(0,0,0));
				printText("Andrei Voiniciuc", 10,85,RGBToWord(255,0,255),RGBToWord(0,0,0));
				printText("C21342953", 35,95,RGBToWord(255,0,255),RGBToWord(0,0,0));
				printText("&",63,105,RGBToWord(255,0,255),RGBToWord(0,0,0));
				printText("Haroun Kassouri", 13,115,RGBToWord(255,0,255),RGBToWord(0,0,0));
				printText("C21508813", 35,125,RGBToWord(255,0,255),RGBToWord(0,0,0));
				playNote(C4, 200);
				playNote(D4, 200);
				playNote(E4, 200);
				playNote(F4, 200);
				playNote(G4, 200);
				playNote(F4, 200);
				playNote(E4, 200);
				playNote(D4, 200);
				playNote(C4, 200);
				break; //breaks out of while loop to stop program (so program can hit return 0)
			}
		}

		
		// keep a line of space between this delay and anything before or it just won't run the statement before it, weird af program
		delay(40); // Sets character image speed
	}
	return 0;
}
void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
  // Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
        // inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
void playNote(uint32_t Freq, uint32_t duration)
{
		TIM14->CR1 = 0; // Set Timer 14 to default values
	TIM14->CCMR1 = (1 << 6) + (1 << 5);
	TIM14->CCER |= (1 << 0);
	TIM14->PSC = 48000000UL/65536UL; // yields maximum frequency of 21kHz when ARR = 2;
	TIM14->ARR = (48000000UL/(uint32_t)(TIM14->PSC))/((uint32_t)Freq);
	TIM14->CCR1 = TIM14->ARR/2;	
	TIM14->CNT = 0;
	TIM14->CR1 |= (1 << 0);
	power_saving_delay(duration);
	TIM14->CR1 &= ~(1u << 0); // clear LSB of Control Register 1 for Timer 14.
}
void power_saving_delay(uint32_t dly)
{
	uint32_t end_time = milliseconds + dly;
	while(milliseconds < end_time)
	{
		__asm(" WFI "); // sleep
	}
}
void initTimer()
{
	// Power up the timer module
	RCC->APB1ENR |= (1 << 8);
	pinMode(GPIOB,1,2); // Assign a non-GPIO (alternate) function to GPIOB bit 1
	GPIOB->AFR[0] &= ~(0x0fu << 4); // Assign alternate function 0 to GPIOB 1 (Timer 14 channel 1)
	TIM14->CR1 = 0; // Set Timer 14 to default values
	TIM14->CCMR1 = (1 << 6) + (1 << 5);
	TIM14->CCER |= (1 << 0);
	TIM14->PSC = 48000000UL/65536UL; // yields maximum frequency of 21kHz when ARR = 2;
	TIM14->ARR = (48000000UL/(uint32_t)(TIM14->PSC))/((uint32_t)C4);
	TIM14->CCR1 = TIM14->ARR/2;	
	TIM14->CNT = 0;
}

// Function which will print message to player and play notes(When player collects coin i.e. interacts with coin)
void pluscoin()
{
	printText("+1 Coin!", 70, 3, RGBToWord(0xff,0xff,0), RGBToWord(10,10,10));
	playNote(C4, 175);
	playNote(E4, 175);
	playNote(G4, 175);
	fillRectangle(70,0,58,12,RGBToWord(10,10,10));
}

// Function which will print message to player and play notes
void minuslife()
{
	printText("Try Again!", 60, 3, RGBToWord(0xff,0xff,0), RGBToWord(10,10,10)); // message printed to player
	playNote(G4, 175);
	playNote(E4, 175);
	playNote(C4, 175);
	fillRectangle(60,0,68,12,RGBToWord(10,10,10)); //Makes message "Try again!" disappear
}

// Function to draw the maze structure i.e. the maze walls
void DrawMaze(void)
{
	//The Maze structure
	fillRectangle(5,15,3,143,RGBToWord(255,255,0)); // x, y, width, height, colour
	fillRectangle(5,15,53,3,RGBToWord(0,255,0));
	fillRectangle(58,15,3,38,RGBToWord(255,0,255));
	fillRectangle(21,155,102,3,RGBToWord(255,0,255));
	fillRectangle(43,35,3,15,RGBToWord(0,255,255));
	fillRectangle(43,50,18,3,RGBToWord(255,0,0)); 
	fillRectangle(120,76,3,79,RGBToWord(255,0,0));
	fillRectangle(59,109,3,28,RGBToWord(255,0,0));
	fillRectangle(74,91,3,46,RGBToWord(0,255,0));
	fillRectangle(74,134,30,3,RGBToWord(0,255,255));
	fillRectangle(21,35,3,102,RGBToWord(255,0,0));
	fillRectangle(21,35,25,3,RGBToWord(0,255,0));
	fillRectangle(21,134,10,3,RGBToWord(255,0,255));
	fillRectangle(31,118,3,19,RGBToWord(255,0,255));
	fillRectangle(31,118,19,3,RGBToWord(0,255,255));
	fillRectangle(49,118,3,19,RGBToWord(255,255,0));
	fillRectangle(49,134,10,3,RGBToWord(255,255,0));	
	fillRectangle(24,109,38,3,RGBToWord(0,255,0));
	fillRectangle(39,88,38,3,RGBToWord(255,0,255));
	fillRectangle(39,73,58,3,RGBToWord(255,0,0)); 
	fillRectangle(39,73,3,15,RGBToWord(0,255,255)); 
	fillRectangle(76,15,3,38,RGBToWord(255,0,255));
	fillRectangle(94,38,3,37,RGBToWord(255,0,0));
	fillRectangle(78,15,28,3,RGBToWord(255,0,0));
	fillRectangle(94,38,12,3,RGBToWord(0,255,0));	
  fillRectangle(105,97,3,40,RGBToWord(0,255,255));
	fillRectangle(75,97,30,3,RGBToWord(255,0,255));
	fillRectangle(75,73,48,3,RGBToWord(255,255,0));
	fillRectangle(74,73,3,15,RGBToWord(0,255,255));
	fillRectangle(61,50,16,3,RGBToWord(255,255,255));
}

