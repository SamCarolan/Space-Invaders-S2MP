#include <stm32f031x6.h>
#include "display.h"
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);

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

	uint16_t invader_x = 0;
	uint16_t oldix = invader_x;
	uint16_t target = 115;

	uint16_t projectile_x = 0;
	uint16_t projectile_y = 124;
	uint16_t oldpx = projectile_x;
	uint16_t oldpy = projectile_y;

	initClock();
	initSysTick();
	setupIO();
	
	putImage(x,y,12,16,space_ship,0,0);

	while(1)
	{
		// Code for welcome screen.
		while(continuation_condition != 1){
			printText("Space Invaders",128/2-47, 160/2-30, 255, 0);
			printText("Push down to start", 2, 160/2, 255, 0);

			if ( (GPIOA->IDR & (1 << 11)) == 0){
				fillRectangle(0,160/2-30,160,40, 0);
				continuation_condition = 1;
			}
		}
		// End of code for welcome screen.

		// Code to make the invader move from side to side on the screen.
		switch(invader_x){
			case 115:
				target = 1;
				break;
			case 1:
				target = 115;
				break;
		}	

		if(invader_x != target){
			fillRectangle(oldix,0,12,16,0);
			oldix = invader_x;
			putImage(invader_x, 0, 12,16,space_invader,0,0);
			
			if(target == 115){
				invader_x = invader_x + 1;
			} else {
				invader_x = invader_x - 1;
			}
		}
		// End of invader movement code.

		hmoved = vmoved = 0;

		if ((GPIOB->IDR & (1 << 4))==0) // right pressed
		{					
			if (x < 115)
			{
				x = x + 3;
				hmoved = 1;
			}						
		}
		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{			
			
			if (x > 1)
			{
				x = x - 3;
				hmoved = 1;
			}			
		}
		if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
		{
			if (y < 140)
			{
				// y = y + 1;			
				vmoved = 1;
			}

			//printTextX2("SHIELD/BOMB!", 10, 60, RGBToWord(0xff,0xff,0), 0);
			//putImage(x, 124, 16, 12, temp_proj, 1,1);
		}
		if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{			
			if (y > 16)
			{
				// y = y - 1;
				vmoved = 1;
			}

			// Code to shoot the projectile from the spaceship.
			projectile_x = x;
			projectile_y = 124;
			while(projectile_y > 0){
				fillRectangle(oldpx,oldpy,12,16,0);
				oldpy = projectile_y;
				putImage(projectile_x, projectile_y, 12,16,temp_proj,0,0);
				projectile_y = projectile_y - 1;
			}
			fillRectangle(projectile_x, projectile_y,12,16,0);
			// End of projectile code.
		}

		// Clean this code (Remove the hmoved elements as the rocket only goes side to side. The toggle can be fully removed from the code).
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

			// Now check for an overlap by checking to see if ANY of the 4 corners of deco are within the target area
			if (isInside(20,80,12,16,x,y) || isInside(20,80,12,16,x+12,y) || isInside(20,80,12,16,x,y+16) || isInside(20,80,12,16,x+12,y+16) )
			{
				//printTextX2("GLUG!", 10, 20, RGBToWord(0xff,0xff,0), 0);
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