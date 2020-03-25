/**
  ******************************************************************************
  * File Name          : LEG.c
  * Description        : �Ȳ��˶��Ķ��ʵ�ּ������ʼ��.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 Little-Doggy.
  * All rights reserved.</center></h2>
  *
  ******************************************************************************
  */

#include "leg.h"

/**
  * @brief ���ڴ洢������λ�õ�double����
	* @attention 
	* ֻ�ܷŵ�.c��ŵ�.h�ᱨ�����£�
	* Error: L6200E: Symbol zero_pos multiply defined (by control_task.o and leg.o).
	* Error: L6200E: Symbol zero_pos multiply defined (by main.o and leg.o).
  */
double zero_pos[8];

Leg_t LF = LEG_DEFAULT, RF = LEG_DEFAULT, LB = LEG_DEFAULT, RB = LEG_DEFAULT;


/**
  * @brief  ���ö�������λ��
  * @retval None
  */
void Set_ZeroPos()
{
	zero_pos[RF_SERVO_1] = 100;
	zero_pos[RF_SERVO_2] = 100;
	zero_pos[RB_SERVO_1] = 100;
	zero_pos[RB_SERVO_2] = 100;
	zero_pos[LF_SERVO_1] = 100;
	zero_pos[LF_SERVO_2] = 100;
	zero_pos[LB_SERVO_1] = 80;
	zero_pos[LB_SERVO_2] = 80;
	//��ʼ��λ��LBΪ180 ����200
}

/**
  * @brief  �����ʼ��
  * @retval None
  */
void Leg_Init(void)
{
	Set_ZeroPos();
	
	LF.index = LF_LEG;
	RF.index = RF_LEG;
	LB.index = LB_LEG;
	RB.index = RB_LEG;
	
	LF.zero_y = 10.7 + -1;
	LB.zero_y	= 10.5 + -1;
	RB.zero_y	= 10.2 + -1;
	RF.zero_y	= 10.4 + -1;
	
	
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);  //�˺Ŷ��
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);  //�ĺŶ��
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);  //�ߺŶ��
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);  //���Ŷ�� 
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);  //���Ŷ��
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);  //���Ŷ�� 
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);  //��Ŷ�� 
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);  //һ�Ŷ�� 
}

/**
  * @brief  ���ö��ת���Ƕ�
  * @param  servo: ѡ����Ҫ���ƵĶ��
  * @param  degree: �趨�Ķ��ת���Ƕ�
  * @retval None
  */
void Set_Servo_Angle(Servo servo, double degree)
{
	degree *= -180.0/200;
	switch(servo)
	{
		case RF_SERVO_1: TIM4 -> CCR4 = degree - zero_pos[servo]; break;
		case RF_SERVO_2: TIM4 -> CCR2 = degree - zero_pos[servo]; break;
		case RB_SERVO_1: TIM4 -> CCR3 = degree - zero_pos[servo]; break;
		case RB_SERVO_2: TIM4 -> CCR1 = degree - zero_pos[servo]; break;
		case LF_SERVO_1: TIM8 -> CCR4 = degree - zero_pos[servo]; break;
		case LF_SERVO_2: TIM8 -> CCR2 = degree - zero_pos[servo]; break;
		case LB_SERVO_1: degree += 90; degree = -degree; degree -= 90; TIM8 -> CCR3 = degree - zero_pos[servo]; break;
		case LB_SERVO_2: degree += 90; degree = -degree; degree -= 90; TIM8 -> CCR1 = degree - zero_pos[servo]; break;
		
		default: break;
	}
}

/**
  * @brief  ����Leg_Indexת��
  * @param  leg: ѡ����Ҫ���Ƶ�Leg_Index
  * @param  degree1: Leg_Index�Ķ��1��ת��
  * @param  degree2: Leg_Index�Ķ��2��ת��
  * @retval None
  */
void Set_Leg_Angle(Leg_Index leg, double degree1, double degree2)
{
	switch(leg)
	{
		case LF_LEG: 
			Set_Servo_Angle(LF_SERVO_1, degree1);
			Set_Servo_Angle(LF_SERVO_2, degree2);
			break;
		case RF_LEG:
			Set_Servo_Angle(RF_SERVO_1, degree1);
			Set_Servo_Angle(RF_SERVO_2, degree2);
			break;
		case LB_LEG:
			Set_Servo_Angle(LB_SERVO_1, degree1);
			Set_Servo_Angle(LB_SERVO_2, degree2);
			break;
		case RB_LEG:
			Set_Servo_Angle(RB_SERVO_1, degree1);
			Set_Servo_Angle(RB_SERVO_2, degree2);
			break;
		
		default: break;
	}
}

/**
  * @brief  ��̬���㺯������(x, y)����Ϊ��Ӧ���ת��(alpha, theta)
  * @param  leg: ��Ҫ������̬�����Leg_Index
  * @param  x: ����x����
  * @param  y: ����y����
  * @retval None
  */
void Set_Feet_Pos(Leg_Index leg, double x, double y)
{
	double x1 = 8, x2 = 16;
	static double alpha, theta;

	double ans = (x1*x1+x*x+y*y-x2*x2)/(2*x1*sqrt(x*x+y*y));
	alpha = acos(ans) + atan(x/y);
	theta = acos(ans) - atan(x/y);
	if (leg == LB_LEG) Set_Leg_Angle(leg, rad_2_deg(theta), rad_2_deg(alpha));
	else Set_Leg_Angle(leg, rad_2_deg(alpha), rad_2_deg(theta));
}



