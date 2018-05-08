//蜂鸣器初始化
void beep_init()
{
	GPD0.CON = (GPD0.CON & (~(0xf<<0))) | (0x2<<0);
	PWM.TCFG0 = (PWM.TCFG0 & (~(0xff<<0))) | (0xff<<0);
	PWM.TCFG1 = (PWM.TCFG1 & (~(0xf<<0))) | (0x1<<0);
	PWM.TCNTB0 = 300;
	PWM.TCMPB0 = 250;
	PWM.TCON = (PWM.TCON & (~(0xf<<0))) | (1<<3 | 1<<1);
}

//蜂鸣器开
void beep_on()
{
	PWM.TCON = (PWM.TCON & (~(0xf<<0))) | (1<<3 | 1<<0);
}

//蜂鸣器关
void beep_off()
{

	PWM.TCON = (PWM.TCON & (~(0xf<<0))) | (0<<0);
}

int main(int argc,char **argv)
{
	beep_init();
	while(1)
	{
		beep_on();
		delay_ms(2500);
		beep_off();
		delay_ms(2500);
	}


#if 0
	int i;
	int ledno;

	//led跑马灯
	led_init();
	for(i=0; i<1000;i++)
	{
		ledno = i%4 + 1;
		led_on(ledno);
		delay_ms(1000);
		led_off(ledno);
	}
#endif

	return 0;
}
