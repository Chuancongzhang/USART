/* * libc_printf.c * 
*		ʹ�ñ�׼C��ʱ,��ӳ��printf������������ļ� 
*    ����ڹ����ڼ�����Ч(����ѡ��semihost����) 
*/ 
#include <stdio.h>
#include "stm32f10x.h"    

#pragma import(__use_no_semihosting)  

//��׼�������֧�ֺ���                
struct __FILE
{
	int handle; 
};
FILE __stdout; 
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x)
{	
	x = x;
}
//��ӳ��fputc�������˺���Ϊ�����������Ļ�������
int fputc(int ch, FILE *f)
{	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
	USART_SendData(USART1, (uint8_t) ch);	
	return ch;	
}
