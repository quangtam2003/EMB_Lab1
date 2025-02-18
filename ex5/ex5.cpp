#include <lpc17xx.h>

#define PRESCALE (25000-1) //25000 PCLK clock cycles to increment TC by 1 

void delayMS(unsigned int milliseconds);
void initTimer0(void);
void led_pattern1();
void led_pattern2();

int num1[3] = {28,29,31};
int num2[5] = {2,3,4,5,6};

int main(void)
{
	//SystemInit(); //called by Startup Code before main(), hence no need to call again.
	initTimer0(); //Initialize Timer0 
	LPC_GPIO2->FIODIR = 0xFFFFFFFF; //Configure P2 as output
	LPC_GPIO1->FIODIR = 0xFFFFFFFF; //Configure P1 as output
	LPC_GPIO1->FIODIR &= ~((1<<24) | (1<<26)); //Configure P1.24 and P1.26 as input

	while(1)
	{
		if((LPC_GPIO1->FIOPIN & (1<<24))==0)
	{
		led_pattern1();
		delayMS(500);
	}
	if((LPC_GPIO1->FIOPIN & (1<<26))==0)
	{
		led_pattern2();
		delayMS(500);
	}
	}

	//return 0; //normally this wont execute ever
}

void led_pattern1()
{
		LPC_GPIO2 -> FIOCLR = 0xFFFFFFFF;
		LPC_GPIO1 -> FIOCLR = 0xFFFFFFFF;
		
		for( int i = 0; i < 3; i++)
		{
				LPC_GPIO1 -> FIOSET |= (1<<num1[i]);
				delayMS(500);
		}
		for( int j = 0; j < 5; j++)
		{
				LPC_GPIO2 -> FIOSET |= (1<<num2[j]);
				delayMS(500);
		}
		
		LPC_GPIO2 -> FIOCLR = 0xFFFFFFFF;
		LPC_GPIO1 -> FIOCLR = 0xFFFFFFFF;
		
		for ( int i = 2; i > 0; i--
		
}

void led_pattern2()
{
		for(int i=0; i<3;i++){
			LPC_GPIO1->FIOSET |= (1<<num1[i]); //Turn ON LED
			delayMS(500); //0.5 Second(s) Delay
			LPC_GPIO1->FIOCLR |= (1<<num1[i]); //Turn ON LED
		}
		for(int j=0; j<5;j++){
			LPC_GPIO2->FIOSET |= (1<<num2[j]); //Turn ON LED
			delayMS(500); //0.5 Second(s) Delay
			LPC_GPIO2->FIOCLR |= (1<<num2[j]); //Turn ON LED
		}
		
		delayMS(500);
		
		for(int j=4; j>=0;j--){
			LPC_GPIO2->FIOSET |= (1<<num2[j]); //Turn ON LED
			delayMS(500); //0.5 Second(s) Delay
			LPC_GPIO2->FIOCLR |= (1<<num2[j]); //Turn OFF LED
		}
		for(int i=2; i>=0;i--){
			LPC_GPIO1->FIOSET |= (1<<num1[i]); //Turn ON LED
			delayMS(500); //0.5 Second(s) Delay
			LPC_GPIO1->FIOCLR |= (1<<num1[i]); //Turn OFF LED
		}
		
		LPC_GPIO2->FIOCLR = 0xFFFFFFFF; //Turn all LED OFF
		LPC_GPIO1->FIOCLR = 0xFFFFFFFF; //Turn all LED OFF
	
		
}

void initTimer0(void)
{
	/*Assuming that PLL0 has been setup with CCLK = 100Mhz and PCLK = 25Mhz.*/
	LPC_SC->PCONP |= (1<<1); //Power up TIM0. By default TIM0 and TIM1 are enabled.
	LPC_SC->PCLKSEL0 &= ~(0x3<<3); //Set PCLK for timer = CCLK/4 = 100/4 (default)

	LPC_TIM0->CTCR = 0x0;
	LPC_TIM0->PR = PRESCALE; //Increment LPC_TIM0->TC at every 24999+1 clock cycles
	//25000 clock cycles @25Mhz = 1 mS

	LPC_TIM0->TCR = 0x02; //Reset Timer
	}

void delayMS(unsigned int milliseconds) //Using Timer0
{
	LPC_TIM0->TCR = 0x02; //Reset Timer
	LPC_TIM0->TCR = 0x01; //Enable timer

	while(LPC_TIM0->TC < milliseconds); //wait until timer counter reaches the desired delay

	LPC_TIM0->TCR = 0x00; //Disable timer
}