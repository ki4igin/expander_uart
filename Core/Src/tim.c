#include "tim.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_bus.h"

// Таймер используется для таймаута UART, период переполнения 1мс
void MX_TIM6_Init(void)
{
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

    NVIC_SetPriority(TIM6_DAC_IRQn,
                     NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 10, 0));
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    LL_TIM_InitTypeDef TIM_InitStruct = {
        .Prescaler = 100 - 1,
        .CounterMode = LL_TIM_COUNTERMODE_UP,
        .Autoreload = 1000 - 1,
    };
    LL_TIM_Init(TIM6, &TIM_InitStruct);
    LL_TIM_EnableARRPreload(TIM6);
    LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_RESET);
    LL_TIM_DisableMasterSlaveMode(TIM6);
    LL_TIM_EnableCounter(TIM6);
    LL_TIM_EnableIT_UPDATE(TIM6);
}
