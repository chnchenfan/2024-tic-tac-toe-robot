#include "beep.h"
#include "delay.h"





uc16 tone[] = {247,262,294,330,349,392,440,494,523,587,659,698,784,880,988,1046,0};

u8 scale = 16;

u8 BrokenMoon[]={
	3,5,6,8,
	9,8,9,10,8,6,
	5,3,8,9,6,6,8,
	9,8,9,10,12,13,
	15,14,13,14,13,12,13,12,10,
	9,10,8,9,8,9,
	10,6,8,9,8,6,6,5,
	6,5,6,8,9,10,9,5,
	6
};
u8 BrokenMoonTime[]={
	8,8,8,8,
	16,8,8,16,8,8,
	8,8,8,8,16,8,8,
	16,8,8,16,8,8,
	8,8,2,2,4,8,16,8,8,
	16,8,8,16,8,8,
	12,4,4,4,8,16,8,8,
	12,2,2,8,8,8,8,16,
	32
};








void BEEP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOE, GPIO_Pin_0); 
}

void BEEP_ON(void)
{
    BEEP = 1;
}

void BEEP_OFF(void)
{
    BEEP = 0;
}




void BUZZER_Play(u16 frq){
	u32 time;
	if(frq){

		time = 50000/((u32)frq);
		BEEP_ON();
		delay_us(time);
		BEEP_OFF();
		delay_us(time*9);
	}
	else delay_us(1000);
}

void MUSIC_Play(u8 *music,u8 *time,u16 length){
	u16 i,j;
	for(i=0;i<length;i++)
		for(j=0;j<((u16)time[i])*tone[music[i]]/scale;j++) 
			BUZZER_Play((u32)tone[music[i]]);
}

void MUSIC1_Play(void){
	MUSIC_Play(BrokenMoon,BrokenMoonTime,sizeof(BrokenMoon)/sizeof(BrokenMoon[0]));
}
