#include "exynos_4412.h"

void led_init(void)
{
	GPX2.CON = (GPX2.CON & (~(0xf<<28))) | (0x1<<28);
//31:28   27-0
	//1111 0000 0000 0000 0000 0000 0000 0000
	//0001 0000 0000 0000 0000 0000 0000 0000
	GPX1.CON = (GPX1.CON & (~(0xf<<0))) | (0x1<<0);
	GPF3.CON = (GPF3.CON & (~(0xff<<16))) | (0x11<<16);
}
void led_on(int num)
{
	switch(num)
	{
		case 1:
			GPX2.DAT = GPX2.DAT | 0X1<<7;
			break;
		case 2:
			GPX1.DAT = GPX1.DAT | 0X1<<0;
			break;
		case 3:
			GPF3.DAT = GPF3.DAT | 0X1<<4;
			break;
		case 4:
			GPF3.DAT = GPF3.DAT | 0X1<<5;
			break;

	}
}
void led_off(int num)
{
	switch(num)
	{
		case 1:
			GPX2.DAT = GPX2.DAT & (~(0X1<<7));
			break;
		case 2:
			GPX1.DAT = GPX1.DAT & (~(0X1<<0));
			break;
		case 3:
			GPF3.DAT = GPF3.DAT & (~( 0X1<<4));
			break;
		case 4:
			GPF3.DAT = GPF3.DAT & (~(0X1<<5));
			break;
	}
}
void delay_ms(int num)
{
	int i,j;
	for(i=num;i>0;i--)
	{
		for(j=1000;j>0;j--);
	}
}
void beep_init(void)
{
	GPD0.CON = (GPD0.CON &(~(0xf<<0))) | (0x2<<0);
	PWM.TCFG0 = (PWM.TCFG0 &(~(0xff<<0))) | (0xff<<0);
	PWM.TCFG1 = (PWM.TCFG1 &(~(0xf<<0))) | (0x2<<0);
	PWM.TCNTB0 = 500;
	PWM.TCMPB0 = 250;
	PWM.TCON = (PWM.TCON &(~ (0xf<<0)))  | (1<<3|1<<1);
}
void beep_on(void)
{
	PWM.TCON = (PWM.TCON &(~ (0xf<<0)))  | (1<<3 | 1<<0);
}
void beep_off(void)
{
	PWM.TCON = (PWM.TCON &(~ (0xf<<0)))  | (0<<0);
}
#define BEEP_HZ 25000

void beep_set_frequence(unsigned int fre)
{
	if(fre == 0)
	{
		return;
	}

	PWM.TCNTB0 = 25000/fre;
	PWM.TCMPB0 = 25000/(2*fre);
}

const unsigned char dahai[] =
{
	0x13,  0x15 , 0x16, 0x16, 0x16, 0x16, 0x21, 0x16, 0x15, 0x15, 0x16, 0x15,  //哀愁
	0x13,  0x12 , 0x11, 0x11, 0x11, 0x11, 0x12, 0x13,
	0x13,  0x12 , 0x11, 0x11, 0x11, 0x11, 0x21, 0x16, 0x15, 0x15, 0x16, 0x15,
	0x13,  0x15,  0x16, 0x21, 0x21, 0x16, 0x15, 0x15,//飘远
};
const unsigned char happy[] =
{
	0x15,  0x15 , 0x16, 0x15, 0x21, 0x17, 0x15, 0x15, 0x16, 0x15, 0x22, 0x21,  //哀愁
	0x13,  0x12 , 0x11, 0x11, 0x11, 0x11, 0x12, 0x13,
	0x13,  0x12 , 0x11, 0x11, 0x11, 0x11, 0x21, 0x16, 0x15, 0x15, 0x16, 0x15,
	0x13,  0x15,  0x16, 0x21, 0x21, 0x16, 0x15, 0x15,//飘远
};
/*
 * 大海的简谱, 控制每一个音的时间 1为长时间 4为短时间
 */
const unsigned char time[] =
{
	4,		4,   4,     2,    4 ,   2,    4,     4,    4,    2,    4,    2 ,//哀愁
	4,      4,   4,     2,    4,    2,    2,     1,
	4,      4,   4,     2,    4,    2,    4,     4,     4,   2,    4,    2,
	4,      4,   2,      4,   2,     4,   4,     1  //飘远

};
const unsigned int yinyue[3][7]
={
	262,  294,  330,  370,  415,  266,  294,  //低音
	523,  578,  659,  698,  784,  880,  988,  //中音
	1046, 1174, 1318, 1396, 1567, 1700, 1975  //高音
};
int main(int argc,char **argv)
{

	led_init();
	beep_init();

	GPX1.CON = (GPX1.CON & (~(0xf<<4)) ) | (0x0<<0);

	while(1)
	{
		if(!(GPX1.DAT & (0x1<<1)))
		{
			delay_ms(10);
			if(!(GPX1.DAT & (0x1<<1)))
			{
				//	按键按下
				led_on(1);
				beep_on();
				delay_ms(500);
				led_off(1);
				beep_off();
			}
		}
	}


#if 0
	int loop,a,b,delay;

	beep_init();
	while(1)
	{
		//循环播放 音乐
		for(loop=0; loop<sizeof(happy)/(sizeof(unsigned char)); loop++)
		{

			//查大海的简谱表
			a = happy[loop]>>4 ;       		    //计算出高低音
			b = (happy[loop] & 0x0f )-1;  	    //计算出DO RE MI FA SO LA XI
			delay = 0x1500/time[loop];
			beep_set_frequence( yinyue[a][b] ); //重新设定频率
			beep_on();			//发出一个音
			delay_ms(delay);

			beep_off();         //关闭蜂鸣器， 每个音播放完成后有间隔感
		//	delay_ms(0x10);   //关闭蜂鸣器
			delay_ms(100);
		}
	}





	int i;
	int ledno;

	led_init();

	for(i=0;i<1000;i++)
	{
		ledno = i%4 + 1;
		led_on(ledno);
		delay_ms(1000);
		led_off(ledno);
	}
#endif
	return 0;
}
