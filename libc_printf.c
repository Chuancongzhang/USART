/* * libc_printf.c * 
*		使用标准C库时,重映射printf等输出函数的文件 
*    添加在工程内即可生效(切勿选择semihost功能) 
*/ 
#include <stdio.h>
#include "stm32f10x.h"    

#pragma import(__use_no_semihosting)  

//标准库所需的支持函数                
struct __FILE
{
	int handle; 
};
FILE __stdout; 
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x)
{	
	x = x;
}
//重映射fputc函数，此函数为多个输出函数的基础函数
int fputc(int ch, FILE *f)
{	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
	USART_SendData(USART1, (uint8_t) ch);	
	return ch;	
}
