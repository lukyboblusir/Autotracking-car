#include <run.h>
#include <Math.h>
#include <delay.h>
#include <stm32f10x.h>

void TIM4_PWM_Init(unsigned short arr, unsigned short psc) //行进初始化函数
{
	//取出结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //打开TIM4时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); //打开GPIO口PA与PB
	
	//左轮管脚1初始化
	GPIO_InitStructure.GPIO_Pin = LEFT_RUN_GO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//方式为推挽输出
	GPIO_Init(LEFT_RUN_GO_GPIO, &GPIO_InitStructure);
	
	//左轮管脚2初始化
	GPIO_InitStructure.GPIO_Pin = LEFT_RUN_PWM; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //方式为复用推挽输出
	GPIO_Init(LEFT_RUN_PWM_GPIO, &GPIO_InitStructure);
	
	//右轮管脚1初始化
	GPIO_InitStructure.GPIO_Pin = RIGHT_RUN_GO; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(RIGHT_RUN_GPIO, &GPIO_InitStructure);
	
	//右轮管脚2初始化
	GPIO_InitStructure.GPIO_Pin = RIGHT_RUN_PWM;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(RIGHT_RUN_PWM_GPIO, &GPIO_InitStructure);
	
	//时钟初始化
	TIM_TimeBaseStructure.TIM_Period = arr;						//设置周期
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//设置频率
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);				//初始化TIM4计时器的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;			  //设置脉冲宽度调剂模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //启用比较输出
	TIM_OCInitStructure.TIM_Pulse = 0;							  //占空比
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);					  //初始化CCR3上的定时器4
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);					  //初始化CCR4上的定时器4
	
	TIM_CtrlPWMOutputs(TIM4, ENABLE); //打开PWM主输出计时器
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //预装载
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE); //打开预加载寄存器
	TIM_Cmd(TIM4, ENABLE); // 打开定时器
	
}

//设置轮胎运行参数，rorl为0即设置右，为1即设置左，speed为速度，有正负之分。
//由于电机接线问题，对左轮来说speed为负时为前进，对右轮来说speed为正时为前进x
void SetRunSpeed(unsigned char rorl, signed char speed)
{
	short PWM;
	
	if (speed >= 100) // 最大速度
		speed = 100;
	if (speed <= -100)
		speed = -100;
	
	PWM = 7201 - fabs(speed) * 72; //求速度
	
		switch (rorl)
	{
	case 0:
		TIM_SetCompare3(TIM4, PWM); // compare的3表示是位于CH3的定时器，第一个参数是定时器的名字，第二个参数是输出高电平的**时间**，第二个参数除周期即得占空比
		if (speed > 0)
			RIGHT_RUN_GO_RESET;
		else if (speed < 0)
			RIGHT_RUN_GO_SET;
		break;
	case 1:
		TIM_SetCompare4(TIM4, PWM);
		if (speed > 0)
			LEFT_RUN_GO_SET;
		else if (speed < 0)
			LEFT_RUN_GO_RESET;
		break;
	}
}
////////////////////////////////小车行进函数/////////////////////////////////////
//第一个参数为速度最大值为100，第二个参数为持续时间，单位为毫秒
void Run_forward(signed char speed, int time) //前进
{
	signed char speed_al = -1*speed;
	SetRunSpeed(0, speed);
	SetRunSpeed(1, speed_al);
	delay_ms(time);
}

void Run_brake(int time) //刹车
{
	SetRunSpeed(0, 0);
	SetRunSpeed(1, 0);
	RIGHT_RUN_GO_RESET;
	LEFT_RUN_GO_RESET;
	delay_ms(time);
}
	
void Run_Left(signed char speed, int time) //左转
{
	SetRunSpeed(1, 0);
	SetRunSpeed(0, speed);
	delay_ms(time);
}

void Run_Spin_Left(signed char speed, int time) //原地左转
{
	SetRunSpeed(1, speed);
	SetRunSpeed(0, speed);
	delay_ms(time);
}

void Run_Right(signed char speed, int time) //右转
{
	signed char f_speed = -speed;
	SetRunSpeed(1, f_speed);
	SetRunSpeed(0, 0);
	delay_ms(time);
}

void Run_Spin_Right(signed char speed, int time) //原地右转
{
	signed char f_speed = -speed;
	SetRunSpeed(1, f_speed);
	SetRunSpeed(0, f_speed);
	delay_ms(time);
}

void Run_back(signed char speed, int time) //后退
{
	signed char speed_al = -1*speed;
	SetRunSpeed(1, speed);
	SetRunSpeed(0, speed_al);
	delay_ms(time);
}