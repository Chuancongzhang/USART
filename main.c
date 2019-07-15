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

//���ڷ����ַ�
 void Usart_SendByte( USART_TypeDef * pUSARTx, char ch)
 {
 //
 USART_SendData(pUSARTx,ch);
 
 //
 while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
 }
 
 //���ڷ����ַ���
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
 

//���ڳ�ʼ������
void USART_Configuration(u32 Baudrate)
{
		//PA9
	  //�����ṹ��
		GPIO_InitTypeDef GPIO_InitStructure; 
	  //����ѡ������
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9; 
		//��������������Ƶ��
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
		//Tx �����������ģʽ�������������������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 
		//�������õ�InitStructure��ʼ��GPIO��
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		//PA10
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10; 
		//�����������ģʽ����������������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
		//��������InitStructure��ʼ��GPIO��
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
		NVIC_Init (&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��NVIC�Ĵ���

	  USART_InitTypeDef USART_InitStructure; 
    USART_InitStructure.USART_BaudRate = Baudrate; //���ò�����    
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;   //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //�շ�ģʽ
	
    USART_Init(USART1,&USART_InitStructure); //��ʼ������1
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //�������ڽ�������ж�
		USART_ITConfig(USART1,USART_IT_IDLE,ENABLE); //�������ڿ����ж�
    USART_Cmd(USART1,ENABLE); //ʹ�ܴ���1
		
}

/*------------------------------------------------------------
                       main
------------------------------------------------------------*/
int main()
{
	//ʹ��GPIOA�ʹ���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	USART_Configuration(9600);
	
	while (1)
  {

	}
}

void USART1_IRQHandler(void)
{
	if((USART_GetITStatus(USART1,USART_IT_RXNE))!=RESET) //�ж��Ƿ����ж�0
	{
		aRxbuffer[counter++] = USART_ReceiveData(USART1); //�����ַ���������
		USART_ClearFlag(USART1,USART_IT_RXNE);	//����жϱ�־
	}
	else if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET)
	{
		USART1->SR;
		USART1->DR;
		counter = 0;
		
		if(memcmp((uint8_t*)aRxbuffer,(uint8_t *)aRxbuffCompare,BUFFERSIZE)==0)
		{
			Usart_SendString(USART1,(char *)aTxbuffer); //���յ������ַ������ͻ�Ӧ�ַ���
			memset(aRxbuffer, 0, sizeof(aRxbuffer));  //����ַ���
		}
		else
		{
			Usart_SendString(USART1,(char *)Errbuffer); //�����ַ��������ͱ����ַ���
			printf("\rLOL\n\r");
		}
	}
}
