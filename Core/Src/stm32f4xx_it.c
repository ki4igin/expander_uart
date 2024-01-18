#include "main.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_ll_tim.h"
#include "usart_ex.h"
#include "adc_ex.h"
#include "gpio.h"
#include "gpio_ex.h"
#include "aura.h"

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
    while (1) {
			led_blink_red();
    }
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    while (1) {
			led_blink_red();
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
    while (1) {
			led_blink_red();
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    while (1) {
			led_blink_red();
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    while (1) {
			led_blink_red();
    }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

#define declare_usart_irq_handler(_name)             \
    void _name##_IRQHandler(void)                    \
    {                                                \
        uart_irq_callback(&uarts[UART_NUM_##_name]); \
    }

// clang-format off
declare_usart_irq_handler(USART1)
declare_usart_irq_handler(USART2)
declare_usart_irq_handler(USART3)
declare_usart_irq_handler(UART4)
declare_usart_irq_handler(UART5)
declare_usart_irq_handler(USART6)
declare_usart_irq_handler(UART7)
declare_usart_irq_handler(UART8)
declare_usart_irq_handler(UART9)
    // clang-format on
    
void ADC_IRQHandler(void)
{
  if(LL_ADC_IsActiveFlag_OVR(ADC1))
  {
    /* Clear flag ADC group regular overrun */
    LL_ADC_ClearFlag_OVR(ADC1);
    led_blink_red();
  }
}

void DMA2_Stream0_IRQHandler(void)
{
  /* Check whether DMA transfer complete caused the DMA interruption */
  if(LL_DMA_IsActiveFlag_TC0(DMA2))
  {
    /*  Clear Stream  transfer complete flag*/
    LL_DMA_ClearFlag_TC0(DMA2);
    /* Call interruption treatment function */
    adc_dma_irq_callback();
  }
  
  /* Check whether DMA transfer error caused the DMA interruption */
  if(LL_DMA_IsActiveFlag_TE0(DMA2))
  {
    LL_DMA_ClearFlag_TE0(DMA2);
    led_blink_red();
  }
}

void TIM6_DAC_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM6))
  {
    LL_TIM_ClearFlag_UPDATE(TIM6);
    tim6_update_callback();
  }
  
}

void TIM7_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM7))
  {
    LL_TIM_ClearFlag_UPDATE(TIM7);
    tim7_update_callback();
  } 
}
