/********************************** (C) COPYRIGHT  *******************************
 * File Name          : main.c
 * Author             : jingfan.wang
 * Version            : V1.0.0
 * Date               : Feb 12, 2023
 * Description        : main file.
 *******************************************************************************/
#include "debug.h"

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        delay_init();
#ifdef CONFIG_DEBUG_UART
        debug_uart_init();
        log_pr("This is %s printf example\r\n", CONFIG_IMG_NAME);
        log_pr("SystemClk:%d\r\n", SystemCoreClock);
#endif

        while (1) {
        }
}

/*********************************************************************
 * @fn      NMI_Handler
 * @brief   This function handles NMI exception.
 * @return  none
 */
void NMI_Handler(void)
{
}

/*********************************************************************
 * @fn      HardFault_Handler
 * @brief   This function handles Hard Fault exception.
 * @return  none
 */
void HardFault_Handler(void)
{
        while (1) {
        }
}
