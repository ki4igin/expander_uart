#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void UART4_IRQHandler(void);
void UART5_IRQHandler(void);
void USART6_IRQHandler(void);
void UART7_IRQHandler(void);
void UART8_IRQHandler(void);
void UART9_IRQHandler(void);
void ADC_IRQHandler(void);

#endif /* __STM32F4xx_IT_H */
