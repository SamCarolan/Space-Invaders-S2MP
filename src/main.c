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

void LEDOn(void);
void LEDOff(void);

volatile uint32_t milliseconds;

const uint16_t space_invader[] = 
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9293,0,0,0,0,0,0,9293,0,0,0,0,0,9293,0,0,0,0,9293,0,0,0,0,0,9293,9293,9293,9293,9293,9293,9293,9293,0,0,0,9293,9293,0,9293,9293,9293,9293,0,9293,9293,0,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,0,9293,9293,9293,9293,9293,9293,9293,9293,0,9293,9293,0,9293,0,0,0,0,0,0,9293,0,9293,0,0,0,9293,9293,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t space_invader_ani[] = 
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9293,0,0,9293,0,0,0,0,0,0,0,9293,0,0,0,0,9293,0,0,0,0,0,9293,9293,9293,9293,9293,9293,9293,9293,0,0,0,9293,9293,0,9293,9293,9293,9293,0,9293,9293,0,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,0,9293,9293,9293,9293,9293,9293,9293,9293,0,9293,9293,0,9293,0,0,0,0,0,0,9293,0,9293,0,0,0,9293,0,0,0,0,9293,0,0,0,0,0,0,9293,0,0,0,0,9293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t second_space_invader[] = 
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9293,0,0,0,0,0,0,9293,0,0,0,0,0,9293,0,0,0,0,9293,0,0,0,0,0,9293,9293,9293,9293,9293,9293,9293,9293,0,0,0,9293,9293,0,9293,9293,9293,9293,0,9293,9293,0,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,0,9293,9293,9293,9293,9293,9293,9293,9293,0,9293,9293,0,9293,0,0,0,0,0,0,9293,0,9293,0,0,0,9293,9293,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t second_space_invader_ani[] = 
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9293,0,0,9293,0,0,0,0,0,0,0,9293,0,0,0,0,9293,0,0,0,0,0,9293,9293,9293,9293,9293,9293,9293,9293,0,0,0,9293,9293,0,9293,9293,9293,9293,0,9293,9293,0,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,0,9293,9293,9293,9293,9293,9293,9293,9293,0,9293,9293,0,9293,0,0,0,0,0,0,9293,0,9293,0,0,0,9293,0,0,0,0,9293,0,0,0,0,0,0,9293,0,0,0,0,9293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t space_ship[] =
{
	0,0,0,0,0,65535,65535,0,0,0,0,0,0,0,0,0,0,65535,65535,0,0,0,0,0,0,0,0,0,65535,65535,65535,65535,0,0,0,0,0,0,0,0,65535,65535,65535,65535,0,0,0,0,0,0,0,0,65535,65535,65535,65535,0,0,0,0,0,0,0,0,65535,65535,65535,65535,0,0,0,0,0,0,0,0,65535,65535,65535,65535,0,0,0,0,0,0,0,65535,65535,65535,65535,65535,65535,0,0,0,0,0,0,65535,65535,65535,65535,65535,65535,0,0,0,0,0,0,65535,0,65535,65535,0,65535,0,0,0,0,0,0,65535,65535,65535,65535,65535,65535,0,0,0,65535,0,0,65535,65535,65535,65535,65535,65535,0,0,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,0,0,0,65535,65535,0,0,0,65535,65535,0,65535,0,0,0,65535,65535,0,0,0,65535,0,
};

const uint16_t projectile[] = 
{
	0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,9293,9293,0,0,9293,9293,0,0,0,0,0,0,9293,9293,0,0,9293,9293,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9293,9293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

int main()
{
	uint16_t x = 64;
	uint16_t y = 120;
	uint16_t oldx = x;
	uint16_t oldy = y;

	uint16_t continuation_condition = 0;
	uint16_t count = 0;

	uint16_t animation = 0;
	uint16_t invader_x = rand() % 116;
	uint16_t invader_y = 0;
	uint16_t second_invader_x = rand() % 116;
	uint16_t second_invader_y = 0;
	uint16_t oldix = invader_x;
	uint16_t oldix_2 = second_invader_x;
	uint16_t shoot = 0;

	uint16_t projectile_x;
	uint16_t old_projectile_x;
	uint16_t projectile_y;
	uint16_t old_projectile_y;
	uint16_t projectile_active = 0;

	uint16_t speed = 1;
	uint16_t lose = 0;
	uint16_t win = 0;
	uint16_t invader_death = 0;
	uint16_t second_invader_death = 0;
	uint16_t stage = 0;
	uint16_t score = 0;

	uint16_t note;

	initClock();
	initSysTick();
	setupIO();
	initSound();
	initSerial();
	
	putImage(x,y,12,16,space_ship,0,0);

	//this while loop ensures that the two space invaders do not spawn directly ontop one another
	while(second_invader_x = invader_x)
	{
		second_invader_x = rand() % 116;

	}

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
		note = rand() % 5;

		switch(note){
		case 0:
			playNote(E3);
			break;
		case 1:
			playNote(E4);
			break;
		case 2:
			playNote(E5);
			break;
		case 3:
			playNote(F3);
			break;
		case 4:
			playNote(F4);
			break;
		case 5:
			playNote(F5);
			break;
		}
		//End of code for the song

		//Code to light up LEDs
		if(stage == 0 && count == 0) {
            LEDOn();
            delay(100); 
            LEDOff();
            delay(100);
			count++;
        } else if(stage == 1 && count == 1) {
            for(int i = 0; i < 2; i++) {
                LEDOn();
                delay(100);
                LEDOff();
                delay(100); 
            }
			count++;
        } else if(stage == 2 && count == 2) {
            for(int i = 0; i < 3; i++) {
                LEDOn();
                delay(100); 
                LEDOff();
                delay(100);
            }
			count = 0;
        }
		//End of LED code

		//Score and Stage code.
		fillRectangle(0,145,160,1,255);
		fillRectangle(0,140,160,1,255);
		printText("Score:",0,150,255,0);
		printNumber(score,40,150,255,0);
		printText("Stage:",75,150,255,0);
		printNumber(stage+1,115,150,2555,0);

		if(score >= 5 && score < 10){
			stage = 1;
			speed = 2;
		} else if(score >= 10 && score < 15){
			stage = 2;
			speed = 3;
		} else if(score >= 15){
			win++;
		}
		//End of Score and Stage code.

		// Code to make the invader fall down screen.
		if(invader_death != 0){
			fillRectangle(invader_x, invader_y,12,16,0);
			fillRectangle(projectile_x, projectile_y,12,16,0);
			projectile_x = x;
			projectile_y = y - 16;
			invader_death = 0;
			invader_y = 0;
			invader_x = rand() % 116;
		}

		if(second_invader_death != 0){
			fillRectangle(invader_x, invader_y,12,16,0);
			fillRectangle(projectile_x, projectile_y,12,16,0);
			projectile_x = x;
			projectile_y = y - 16;
			invader_death = 0;
			invader_y = 0;
			invader_x = rand() % 116;
		}

		fillRectangle(oldix,0,12,16,0);
		oldix = invader_x;

		if(animation % 3 != 0){
			putImage(invader_x, invader_y, 12, 16, space_invader_ani, 0, 0);
			putImage(invader_x, invader_y, 12, 16, second_space_invader_ani, 0, 0);
		} else {
			putImage(invader_x, invader_y, 12, 16, space_invader, 0, 0);
			putImage(invader_x, invader_y, 12, 16, second_space_invader, 0, 0);
		}
		animation++;
		
		if(invader_y < 146){
			invader_y = invader_y + speed;
		}
		if(invader_y >= 124){
			lose++;
		}
		// End of invader movement code.

		//Code for Lose screen.
		while(lose != 0){
			playNote(0);
			printText("LOSER",128/2-15, 160/2-30, 255, 0);
			printText("Push up to restart", 2, 160/2, 255, 0);
			if(UpPressed()){
				fillRectangle(0,0,128,160,0);
				continuation_condition = 0;
				count = 0;
				lose = 0;
				invader_death = 0;
				second_invader_death = 0;
				invader_y = 0;
				x = 64;
				y = 120;
				score = 0;
				stage = 0;
				speed = 1;
				break;
			}
		}
		//End of code for Lose screen.

		//Code for Win screen.
		while(win != 0){
			playNote(0);
			printText("WINNER",128/2-25, 160/2-30, 255, 0);
			printText("Push up to restart", 2, 160/2, 255, 0);
			if(UpPressed()){
				fillRectangle(0,0,128,160,0);
				continuation_condition = 0;
				win = 0;
				invader_death = 0;
				second_invader_death
				invader_y = 0;
				x = 64;
				y = 120;
				score = 0;
				stage = 0;
				speed = 1;
				break;
			}
		}
		//End of code for Win Screen

		//Code to see if the Alien and the player collides.
		if (isInside(x, y, 12, 16, invader_x, invader_y) || isInside(x, y, 12, 16, invader_x + 12, invader_y) || isInside(x, y, 12, 16, invader_x, invader_y + 16) || isInside(x, y, 12, 16, invader_x + 12, invader_y + 16)){
			lose++;
		}

		if (isInside(x, y, 12, 16, second_invader_x, second_invader_y) || isInside(x, y, 12, 16, second_invader_x + 12, second_invader_y) || isInside(x, y, 12, 16, second_invader_x, second_invader_y + 16) || isInside(x, y, 12, 16, second_invader_x + 12, second_invader_y + 16)){
			lose++;
		}
		//End of player Alien collision code.

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
			}
		} 
		if (LeftPressed() == 1 || receivedChar == 'a' || receivedChar == 'A') {
			if (x > 1) {
				x = x - 3;
			}
		} 
		if(receivedChar == ' ' || UpPressed()){
			old_projectile_x = projectile_x;
			old_projectile_y = projectile_y;
			projectile_x = x;
			projectile_y = y - 16;
			shoot++;
		}

		// Reset received character
		receivedChar = 0;
		charReceived = 0;
		//End of Serial Communications Code.

		//Code for projectile.
		if (shoot != 0 && projectile_active == 0 && continuation_condition != 0){
			shoot = 0;
			projectile_active = 1;
		}

		if (projectile_active != 0) {
			putImage(projectile_x, projectile_y, 12, 16, projectile, 0, 0);
			projectile_y = projectile_y - 1;
			if (projectile_y <= 0) {
				projectile_active = 0;
				fillRectangle(x, 0, 12, 16, 0);
			}

			if(shoot != 0){
				projectile_active = 0;
				fillRectangle(old_projectile_x,old_projectile_y,12,16,0);
				projectile_y = y - 16;
			}
		}

		if (isInside(projectile_x, projectile_y, 12, 16, invader_x, invader_y) || isInside(projectile_x, projectile_y, 12, 16, invader_x + 12, invader_y) || isInside(projectile_x, projectile_y, 12, 16, invader_x, invader_y + 16) || isInside(projectile_x, projectile_y, 12, 16, invader_x + 12, invader_y + 16)){
			invader_death++;
			score++;
			projectile_active = 0;
		}

		if (isInside(projectile_x, projectile_y, 12, 16, second_invader_x, second_invader_y) || isInside(projectile_x, projectile_y, 12, 16, second_invader_x + 12, second_invader_y) || isInside(projectile_x, projectile_y, 12, 16, second_invader_x, second_invader_y + 16) || isInside(projectile_x, projectile_y, 12, 16, second_invader_x + 12, second_invader_y + 16)){
			second_invader_death++;
			score++;
			projectile_active = 0;
		}
		//End of projectile code.
		fillRectangle(oldx,oldy,12,16,0);
		fillRectangle(oldix_2,oldy,12,16,0);
		oldx = x;
		oldy = y;					
		putImage(x,y,12,16,space_ship,0,0);
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
	pinMode(GPIOA,12,1);
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

void LEDOn(){
	GPIOA->ODR |= (1 << 12);
}

void LEDOff(){
	GPIOA->ODR &= ~(1 << 12);
}