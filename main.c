#include "stm32f10x.h"
//#include "stm32f10x_conf.h"

#include "string.h"
#include "stdio.h"


#define BUFFERSIZE 8

void USART1_IRQHandler(void);

uint8_t aRxbuffCompare[] = "zhangchc";
uint8_t aTxbuffer[] = "It's my name!\n";
uint8_t Errbuffer[] = "It's Error!\n";
uint8_t aRxbuffer[BUFFERSIZE];

int counter = 0;

//串口发送字符
 void Usart_SendByte( USART_TypeDef * pUSARTx, char ch)
 {
 //
 USART_SendData(pUSARTx,ch);
 
 //
 while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
 }
 
 //串口发送字符串
 void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
 {
 unsigned int k=0;
 do {
 Usart_SendByte( pUSARTx, *(str + k) );
 k++;
 } while (*(str + k)!='\0');
 
 //
 while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET) {
 }
 }
 

//串口初始化配置
void USART_Configuration(u32 Baudrate)
{
		//PA9
	  //声明结构体
		GPIO_InitTypeDef GPIO_InitStructure; 
	  //设置选择引脚
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9; 
		//设置引脚最大输出频率
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
		//Tx 设置引脚输出模式————复用推挽输出
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 
		//根据设置的InitStructure初始化GPIO口
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		//PA10
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10; 
		//设置引脚输出模式————浮空输入
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
		//根据设置InitStructure初始化GPIO口
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //子优先级1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
		NVIC_Init (&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器

	  USART_InitTypeDef USART_InitStructure; 
    USART_InitStructure.USART_BaudRate = Baudrate; //设置波特率    
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;   //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //收发模式
	
    USART_Init(USART1,&USART_InitStructure); //初始化串口1
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //开启串口接收完成中断
		USART_ITConfig(USART1,USART_IT_IDLE,ENABLE); //开启串口空闲中断
    USART_Cmd(USART1,ENABLE); //使能串口1
		
}

/*------------------------------------------------------------
                       main
------------------------------------------------------------*/
int main()
{
	//使能GPIOA和串口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	USART_Configuration(9600);
	
	while (1)
  {

	}
}

void USART1_IRQHandler(void)
{
	if((USART_GetITStatus(USART1,USART_IT_RXNE))!=RESET) //判断是否有中断0
	{
		aRxbuffer[counter++] = USART_ReceiveData(USART1); //接收字符存入数组
		USART_ClearFlag(USART1,USART_IT_RXNE);	//清除中断标志
	}
	else if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET)
	{
		USART1->SR;
		USART1->DR;
		counter = 0;
		
		if(memcmp((uint8_t*)aRxbuffer,(uint8_t *)aRxbuffCompare,BUFFERSIZE)==0)
		{
			Usart_SendString(USART1,(char *)aTxbuffer); //接收到正常字符，发送回应字符串
			memset(aRxbuffer, 0, sizeof(aRxbuffer));  //清空字符串
		}
		else
		{
			Usart_SendString(USART1,(char *)Errbuffer); //发送字符串，发送报错字符串
			printf("\rLOL\n\r");
		}
	}
}
