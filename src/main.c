#include <stm32f031x6.h>
#include <stdlib.h>
#include "display.h"
#include "sound.h"
#include "musical_notes.h"
#include "serial.h"

void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);

int UpPressed(void);
int DownPressed(void);
int LeftPressed(void);
int RightPressed(void);

void playMusic(void);

volatile uint32_t milliseconds;

const uint16_t deco1[]=
{
0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7940,7940,7940,7940,0,0,0,
};
const uint16_t deco2[]= 
	{
0,0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,40224,7936,65535,7936,0,0,0,0,0,0,0,40224,40224,7936,65535,7936,0,0,0,0,0,0,65315,40224,40224,7936,65535,40224,0,0,0,0,0,0,0,65315,0,65315,65315,65315,65315,0,0,
	};
const uint16_t deco3[]= 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,65535,65535,4,4,4,4,4,4,65535,65535,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,24327,24327,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t dg1[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};

const uint16_t space_invader[] = 
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9293,0,0,0,0,0,0,9293,0,0,0,0,0,9293,0,0,0,0,9293,0,0,0,0,0,9293,9293,9293,9293,9293,9293,9293,9293,0,0,0,9293,9293,0,9293,9293,9293,9293,0,9293,9293,0,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,0,9293,9293,9293,9293,9293,9293,9293,9293,0,9293,9293,0,9293,0,0,0,0,0,0,9293,0,9293,0,0,0,9293,9293,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t space_ship[] =
{
	0,0,0,0,0,65535,65535,0,0,0,0,0,0,0,0,0,0,65535,65535,0,0,0,0,0,0,0,0,0,65535,65535,65535,65535,0,0,0,0,0,0,0,0,65535,65535,65535,65535,0,0,0,0,0,0,0,0,65535,65535,65535,65535,0,0,0,0,0,0,0,0,65535,65535,65535,65535,0,0,0,0,0,0,0,0,65535,65535,65535,65535,0,0,0,0,0,0,0,65535,65535,65535,65535,65535,65535,0,0,0,0,0,0,65535,65535,65535,65535,65535,65535,0,0,0,0,0,0,65535,0,65535,65535,0,65535,0,0,0,0,0,0,65535,65535,65535,65535,65535,65535,0,0,0,65535,0,0,65535,65535,65535,65535,65535,65535,0,0,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,0,0,0,65535,65535,0,0,0,65535,65535,0,65535,0,0,0,65535,65535,0,0,0,65535,0,
};

const uint16_t temp_proj[] = 
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,9293,9293,9293,9293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

int main()
{
	int toggle = 0;
	int hmoved = 0;
	int vmoved = 0;
	uint16_t x = 64;
	uint16_t y = 140;
	uint16_t oldx = x;
	uint16_t oldy = y;

	uint16_t continuation_condition = 0;

	uint16_t invader_x = rand() % 116;
	uint16_t invader_y = 0;
	uint16_t oldix = invader_x;
	uint16_t shoot = 0;

	uint16_t projectile_x;
	uint16_t projectile_y;
	uint16_t projectile_active = 0;

	uint16_t speed = 1;
	uint16_t lose = 0;
	// uint16_t stage = 0;
	// uint16_t score = 0;

	initClock();
	initSysTick();
	setupIO();
	initSound();
	initSerial();
	
	putImage(x,y,12,16,space_ship,0,0);

	while(1)
	{
		// Code for welcome screen.
		while(continuation_condition != 1){
			printText("Space Invaders",128/2-47, 160/2-30, 255, 0);
			printText("Push down to start", 2, 160/2, 255, 0);

			if (DownPressed() == 1){
				fillRectangle(0,160/2-30,160,40, 0);
				continuation_condition = 1;
			}
		}
		// End of code for welcome screen.

		//Code for a simple theme song
		//playMusic(); //Turn on for Demo
		//End of code for the song

		// Code to make the invader fall down screen.
		fillRectangle(oldix,0,12,16,0);
		oldix = invader_x;
		putImage(invader_x, invader_y, 12,16,space_invader,0,0);
		
		if(invader_y < 146){
			invader_y = invader_y + speed;
		}
		if(invader_y == 146){
			lose++;
		}
		// End of invader movement code.

		//Code for Lose screen.
		while(lose != 0){
			printText("LOSER",128/2-15, 160/2-30, 255, 0);
		}
		//End of code for Lose screen.

		//Code to see if the Alien and the player collides.
		if (isInside(x, y, 12, 16, invader_x, invader_y) || isInside(x, y, 12, 16, invader_x + 12, invader_y) || isInside(x, y, 12, 16, invader_x, invader_y + 16) || isInside(x, y, 12, 16, invader_x + 12, invader_y + 16)){
			lose++;
		}
		//End of player Alien collision code.


		hmoved = vmoved = 0;

		//Serial Communications Code.
		char receivedChar = 0;
		static int charReceived = 0;

		// Check if USART has received any character
		if (USART1->ISR & USART_ISR_RXNE) {
			receivedChar = USART1->RDR; // Read the character
			charReceived = 1; // Set the flag
		}

		// Check button presses and the received character
		if (RightPressed() == 1 || receivedChar == 'd' || receivedChar == 'D') {
			if (x < 115) {
				x = x + 3;
				hmoved = 1;
			}
		} 
		if (LeftPressed() == 1 || receivedChar == 'a' || receivedChar == 'A') {
			if (x > 1) {
				x = x - 3;
				hmoved = 1;
			}
		} 
		if (UpPressed() == 1 || receivedChar == 'w' || receivedChar == 'W') {
			if (y > 16) {
				y = y - 3;
				vmoved = 1;
			}
		} 
		if (DownPressed() == 1 || receivedChar == 's' || receivedChar == 'S') {
			if (y < 140) {
				y = y + 3;
				vmoved = 1;
			}
		}
		if(receivedChar == ' '){
			printText("Shoot",128/2-15, 160/2-30, 255, 0);
			projectile_x = x;
			shoot++;
		}

		// Reset received character
		receivedChar = 0;
		charReceived = 0;
		//End of Serial Communications Code.

		if (shoot != 0 && projectile_active == 0) {
			shoot = 0;
			projectile_y = y - 16;
			projectile_active = 1;
		}

		if (projectile_active != 0) {
			putImage(projectile_x, projectile_y, 12, 16, temp_proj, 0, 0);
			projectile_y--;
			if (projectile_y <= 0) {
				projectile_active = 0;
				fillRectangle(x, 0, 12, 16, 0);
			}
		}

		if ((vmoved) || (hmoved))
		{
			// only redraw if there has been some movement (reduces flicker)
			fillRectangle(oldx,oldy,12,16,0);
			oldx = x;
			oldy = y;					
			if (hmoved)
			{
				if (toggle)
					putImage(x,y,12,16,space_ship,0,0);
				else
					putImage(x,y,12,16,space_ship,0,0);
				
				toggle = toggle ^ 1;
			}
			else
			{
				putImage(x,y,12,16,space_ship,0,0);
			}
		}		
		delay(50);
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
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2; 	
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}

int DownPressed(){
	if ( (GPIOA->IDR & (1 << 11)) == 0){
		return 1;
	}
	else{
		return 0;
	}
}

int UpPressed(){
	if ((GPIOA->IDR & (1 << 8)) == 0){
		return 1;
	}
	else {
		return 0;
	}
}

int LeftPressed(){
	if((GPIOB->IDR & (1 << 5))==0){
		return 1;
	}
	else{
		return 0;
	}
}

int RightPressed(){
	if((GPIOB->IDR & (1 << 4))==0){
		return 1;
	}
	else{
		return 0;
	}
}

void playMusic(){
	playNote(C0);
	delay(10);
	playNote(D0);
	delay(10);
	playNote(E0);
	delay(10);
	playNote(F0);
	delay(10);
	playNote(G0);
	delay(10);
	playNote(A0);
	delay(10);
	playNote(B0);
	delay(10);
	playNote(A0);
	delay(10);
}