#include "duoji.h"


volatile unsigned char  duoji_value=0;

void Duoji_Init(u16 arr,u16 psc){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //ʹ��TIM3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period =  arr;//��������һ�������¼�װ�����Զ���װ�ؼĴ��������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;//����ʱ�ӷָTDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //���ö�ʱ��ģʽ��TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������ԣ�TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM3,&TIM_OCInitStructure); //����TIM_OCInitStructure��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);//CH1Ԥװ��ʹ��
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_CtrlPWMOutputs(TIM3,ENABLE); //MOE �����ʹ��
	TIM_Cmd(TIM3, ENABLE);  //TIM3ʹ��
	TIM3->CCR1= 735;
	//TIM3->CCR1= 300;//open
}

 

 
 


