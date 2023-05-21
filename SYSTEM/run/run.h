#ifndef __RUN_H_
#define __RUN_H_

#include <stm32f10x.h>

void TIM4_PWM_Init(unsigned short arr, unsigned short psc);
void SetRunSpeed(unsigned char ucChannel, signed char cSpeed);

void Run_forward(signed char speed, int time);
void Run_brake(int time);
void Run_Left(signed char speed, int time);
void Run_Spin_Left(signed char speed, int time);
void Run_Right(signed char speed, int time);
void Run_Spin_Right(signed char speed, int time);
void Run_back(signed char speed, int time);

// 左轮管脚1，BP7，设定方向
#define LEFT_RUN_GO GPIO_Pin_7
#define LEFT_RUN_GO_GPIO GPIOB
#define LEFT_RUN_GO_SET GPIO_SetBits(LEFT_RUN_GO_GPIO, LEFT_RUN_GO)
#define LEFT_RUN_GO_RESET GPIO_ResetBits(LEFT_RUN_GO_GPIO, LEFT_RUN_GO)

// 左轮管脚2，BP8，输出PWM
#define LEFT_RUN_PWM GPIO_Pin_8
#define LEFT_RUN_PWM_GPIO GPIOB
#define LEFT_RUN_PWM_SET GPIO_SetBits(LEFT_RUN_PWM_GPIO, LEFT_RUN_PWM)
#define LEFT_RUN_PWM_RESET GPIO_ResetBits(LEFT_RUN_PWM_GPIO, LEFT_RUN_PWM)

// 右轮管脚1，AP4，设定方向
#define RIGHT_RUN_GO GPIO_Pin_4
#define RIGHT_RUN_GPIO GPIOA
#define RIGHT_RUN_GO_SET GPIO_SetBits(RIGHT_RUN_GPIO, RIGHT_RUN_GO)
#define RIGHT_RUN_GO_RESET GPIO_ResetBits(RIGHT_RUN_GPIO, RIGHT_RUN_GO)

// 右轮管脚2，BP9，输出PWM
#define RIGHT_RUN_PWM GPIO_Pin_9
#define RIGHT_RUN_PWM_GPIO GPIOB
#define RIGHT_RUN_PWM_SET GPIO_SetBits(RIGHT_RUN_PWM_GPIO, RIGHT_RUN_PWM)
#define RIGHT_RUN_PWM_RESET GPIO_ResetBits(RIGHT_RUN_PWM_GPIO, RIGHT_RUN_PWM)

#endif