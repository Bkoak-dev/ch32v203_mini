/********************************** (C) COPYRIGHT  *******************************
 * File Name          : debug.h
 * Author             : jingfan.wang
 * Version            : V1.0.1
 * Date               : Oct 30, 2022
 * Description        : This file contains all the functions prototypes for UART
 *                      Printf , Delay functions.
 *******************************************************************************/
#ifndef __DEBUG_H
#define __DEBUG_H

#include "ch32v20x.h"
#include "autoconf.h"

void delay_init(void);
void delay_us(uint32_t n);
void delay_ms(uint32_t n);
void debug_uart_init(void);
void log_pr(char *format, ...);

#endif /* __DEBUG_H */
