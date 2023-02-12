/********************************** (C) COPYRIGHT  *******************************
 * File Name          : debug.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : This file contains all the functions prototypes for UART
 *                      Printf , Delay functions.
 *******************************************************************************/
#include <stdarg.h>
#include "debug.h"

static uint8_t  p_us = 0;
static uint16_t p_ms = 0;

/**
 * @fn delay_init
 * @brief Initializes Delay Funcation.
 * @return none
 */
void delay_init(void)
{
        p_us = SystemCoreClock / 8000000;
        p_ms = (uint16_t)p_us * 1000;
}

/*********************************************************************
 * @fn      delay_us
 * @brief   Microsecond Delay Time.
 * @param   n - Microsecond number.
 * @return  None
 */
void delay_us(uint32_t n)
{
        uint32_t i;

        SysTick->SR &= ~(1 << 0);
        i = (uint32_t)n * p_us;

        SysTick->CMP = i;
        SysTick->CTLR |= (1 << 4);
        SysTick->CTLR |= (1 << 5) | (1 << 0);

        while((SysTick->SR & (1 << 0)) != (1 << 0));
        SysTick->CTLR &= ~(1 << 0);
}

/*********************************************************************
 * @fn      delay_ms
 * @brief   Millisecond Delay Time.
 * @param   n - Millisecond number.
 * @return  None
 */
void delay_ms(uint32_t n)
{
        uint32_t i;

        SysTick->SR &= ~(1 << 0);
        i = (uint32_t)n * p_ms;

        SysTick->CMP = i;
        SysTick->CTLR |= (1 << 4);
        SysTick->CTLR |= (1 << 5) | (1 << 0);

        while((SysTick->SR & (1 << 0)) != (1 << 0));
        SysTick->CTLR &= ~(1 << 0);
}

#ifdef CONFIG_DEBUG_UART
/*********************************************************************
 * @fn      debug_uart_init
 * @brief   Initializes the USARTx peripheral.
 * @param   baudrate - USART communication baud rate.
 * @return  None
 */
void debug_uart_init(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;

#if defined (CONFIG_DEBUG_UART1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
#elif defined (CONFIG_DEBUG_UART2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
#elif defined (CONFIG_DEBUG_UART3)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif

        USART_InitStructure.USART_BaudRate = CONFIG_DB_BAUDRATE;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Tx;

#if defined (CONFIG_DEBUG_UART1)
        USART_Init(USART1, &USART_InitStructure);
        USART_Cmd(USART1, ENABLE);
#elif defined (CONFIG_DEBUG_UART2)
        USART_Init(USART2, &USART_InitStructure);
        USART_Cmd(USART2, ENABLE);
#elif defined (CONFIG_DEBUG_UART3)
        USART_Init(USART3, &USART_InitStructure);
        USART_Cmd(USART3, ENABLE);
#endif
}


static const int8_t* const g_pcHex1 = "0123456789abcdef";
static const int8_t* const g_pcHex2 = "0123456789ABCDEF";

static void printfsend(uint8_t* buf, int len)
{
        int i;

        for (i = 0; i < len; i++) {
#if defined (CONFIG_DEBUG_UART1)
                while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
                USART_SendData(USART1, *buf++);
#elif defined (CONFIG_DEBUG_UART2)
                while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
                USART_SendData(USART2, *buf++);
#elif defined (CONFIG_DEBUG_UART3)
                while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
                USART_SendData(USART3, *buf++);
#endif
        }
}

void log_pr(char *format, ...)
{
        uint32_t ulIdx, ulValue, ulPos, ulCount, ulBase, ulNeg;
        int8_t*pcStr, pcBuf[16], cFill;
        char HexFormat = 0;
        va_list vaArgP;

        va_start(vaArgP, format);

        while (*format) {
                // Find the first non-% character, or the end of the string.
                for (ulIdx = 0; (format[ulIdx] != '%') && (format[ulIdx] != '\0'); ulIdx++) {
                }

                // Write this portion of the string.
                if (ulIdx > 0) {
                    printfsend((uint8_t *)format, ulIdx);
                }
                format += ulIdx;

                if (*format == '%') {
                        format++;

                        // Set the digit count to zero, and the fill character to space
                        // (i.e. to the defaults).
                        ulCount = 0;
                        cFill = ' ';

again:
                        switch (*format++) {
                                case '0':
                                case '1':
                                case '2':
                                case '3':
                                case '4':
                                case '5':
                                case '6':
                                case '7':
                                case '8':
                                case '9': {
                                        if ((format[-1] == '0') && (ulCount == 0)) {
                                                cFill = '0';
                                        }

                                        ulCount *= 10;
                                        ulCount += format[-1] - '0';

                                        goto again;
                                }

                                case 'c': {
                                        ulValue = va_arg(vaArgP, unsigned long);
                                        printfsend((uint8_t *)&ulValue, 1);
                                        break;
                                }

                                case 'd': {
                                        ulValue = va_arg(vaArgP, unsigned long);
                                        ulPos = 0;

                                        if ((long)ulValue < 0) {
                                                ulValue = -(long)ulValue;
                                                ulNeg = 1;
                                        } else {
                                                ulNeg = 0;
                                        }

                                        ulBase = 10;
                                        goto convert;
                                }

                                case 's': {
                                        pcStr = (int8_t *)va_arg(vaArgP, char *);
                                        for(ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++) {
                                        }

                                        printfsend((uint8_t *)pcStr, ulIdx);

                                        if (ulCount > ulIdx) {
                                                ulCount -= ulIdx;
                                                while (ulCount--) {
                                                        printfsend((uint8_t *)" ", 1);
                                                }
                                        }
                                        break;
                                }

                                case 'u': {
                                        ulValue = va_arg(vaArgP, unsigned long);
                                        ulPos = 0;
                                        ulBase = 10;
                                        ulNeg = 0;
                                        goto convert;
                                }

                                case 'X': {
                                        ulValue = va_arg(vaArgP, unsigned long);
                                        ulPos = 0;
                                        ulBase = 16;
                                        ulNeg = 0;
                                        HexFormat='X';
                                        goto convert;
                                }

                                case 'x':

                                case 'p': {
                                        ulValue = va_arg(vaArgP, unsigned long);
                                        ulPos = 0;
                                        ulBase = 16;
                                        ulNeg = 0;
                                        HexFormat='x';

convert:
                                        for (ulIdx = 1;
                                                (((ulIdx * ulBase) <= ulValue) && (((ulIdx * ulBase) / ulBase) == ulIdx));
                                                ulIdx *= ulBase, ulCount--) {
                                        }

                                        if (ulNeg) {
                                                ulCount--;
                                        }

                                        if (ulNeg && (cFill == '0')) {
                                                pcBuf[ulPos++] = '-';
                                                ulNeg = 0;
                                        }

                                        if ((ulCount > 1) && (ulCount < 16)) {
                                                for(ulCount--; ulCount; ulCount--) {
                                                        pcBuf[ulPos++] = cFill;
                                                }
                                        }

                                        if (ulNeg) {
                                                pcBuf[ulPos++] = '-';
                                        }

                                        for (; ulIdx; ulIdx /= ulBase) {
                                                if (HexFormat=='x')
                                                        pcBuf[ulPos++] = g_pcHex1[(ulValue / ulIdx) % ulBase];//x
                                                else
                                                        pcBuf[ulPos++] = g_pcHex2[(ulValue / ulIdx) % ulBase];//X
                                        }

                                        printfsend((uint8_t *)pcBuf, ulPos);
                                        break;
                                }

                                case '%': {
                                        printfsend((uint8_t *)format - 1, 1);
                                        break;
                                }

                                default: {
                                        printfsend((uint8_t *)"ERROR", 5);
                                        break;
                                }
                        }
                }
        }
        va_end(vaArgP);
}
#endif
