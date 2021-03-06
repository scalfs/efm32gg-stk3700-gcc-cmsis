/** ***************************************************************************
 * @file    main.c
 * @brief   Simple UART Demo for EFM32GG_STK3700
 * @version 1.0
******************************************************************************/

#include <stdint.h>
/*
 * Including this file, it is possible to define which processor using command line
 * E.g. -DEFM32GG995F1024
 * The alternative is to include the processor specific file directly
 * #include "efm32gg995f1024.h"
 */
#include "em_device.h"
#include "clock_efm32gg.h"

#include "ministdio.h"
#include "led.h"
#include "uart.h"


/*************************************************************************//**
 * @brief  Sys Tick Handler
 */
const int TickDivisor = 1000; // milliseconds

volatile uint64_t tick = 0;

void SysTick_Handler (void) {
static int counter = 0;

    tick++;

    if( counter == 0 ) {
        counter = TickDivisor;
        // Process every second
        LED_Toggle(LED0);
    }
    counter--;
}


void Delay(int delay) {
uint64_t l = tick+delay;

    while(tick<l) {}

}


/**************************************************************************//**
 * @brief  Input/output functions for ministdio functions
 */

///@{
int putchar(int c) { UART_SendChar(c); return 0; }
int getchar(int c) { return UART_GetChar(); }
///@}

/**************************************************************************//**
 * @brief  Main function
 *
 * @note   Using external crystal oscillator
 *         HFCLK = HFXO
 *         HFCORECLK = HFCLK
 *         HFPERCLK  = HFCLK
 */

int main(void) {
char line[100];

    /* Configure LEDs */
    LED_Init(LED0|LED1);

    // Set clock source to external crystal: 48 MHz
    (void) SystemCoreClockSet(CLOCK_HFXO,1,1);

    /* Turn on LEDs */
    LED_Write(0,LED0|LED1);

    /* Configure SysTick */
    SysTick_Config(SystemCoreClock/TickDivisor);

    /* Configure UART */
    UART_Init();

    __enable_irq();

    printf("\r\n\n\n\rHello\n\r");
    while (1) {
        printf("\r\n\n\n\rWhat is your name?\n");
        fgets(line,99,stdin);
        printf("Hello %s\n",line);
    }

}
