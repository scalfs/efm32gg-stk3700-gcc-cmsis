
08 - Button {#mainpage}
============

This is the 2nd version of Button. It implements a interrupt based method to read the pushbuttons.
It uses the HAL for LEDs (STK3700) used in the last Blink example.


# Interrupts

The buttons are connected to pins 9 and 10 por GPIO Port B.

GPIO pins of all ports can generate two interrupts: IRQ_GPIO_EVEN and IRQ_GPIO_ODD according the pin number.
Given a pin number, only one GPIO port can generate an interrupt. Since each GPIO port has 16 pins, there are 16 sources of interrupt. 


The registers EXTIPSELL and EXTISELH specify which port can generate an interrupt for a given pin number.
The repisters EXTIRISE and EXTIFALL specify which changes on input generate an interrupt.
The registers IEN, IF, IFS and IFC controls the interrupt of each pin.

Since the pins used for buttons are 9 and 10, both interrupts can be generated and so, both interrupt routines GPIO_ODD_IRQHandler and GPIO_EVEN_IRQHandler must be defined. They area defined inside the button.c, but this can be a problem when there are interrupts, other than buttons, generated by GPIO pins.

The API can replicate the one used in the last example, based on polling. But the advantage of interrupt based implementation, is that it is possible to take notice of a event as soon as it occurs. This can be done using a callback mechanism. The user application specifies a function, which is called by the interrupt routine.


# Interrupt in Cortex M Microcontrollers

Interrupts in Cortex M microcontrollers are controller by the Nested V Interrupt Controller (NVIC), which is part of the microcontroller core.

The NVIC is controlled by a set of specific functions of CMSIS.


|     CMSIS function                  |   Description                                             |
|-------------------------------------|-----------------------------------------------------------|
| void NVIC_EnableIRQ(IRQn_Type IRQn) |   Enables an interrupt or exception.                      |
| void NVIC_DisableIRQ(IRQn_Type IRQn)	| Disables an interrupt or exception. | 
| void NVIC_SetPendingIRQ(IRQn_Type IRQn) | 	Sets the pending status of interrupt or exception to 1. | 
| void NVIC_ClearPendingIRQ(IRQn_Type IRQn) | 	Clears the pending status of interrupt or exception to 0. | 
| uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn) | Reads the pending status of interrupt or exception. This function returns non- zero value if the pending status is set to 1. | 
| void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority) | Sets the priority of an interrupt or exception with configurable priority level to 1. | 
| uint32_t NVIC_GetPriority(IRQn_Type IRQn) |  Reads the priority of an interrupt or exception with configurable priority level. This function return the current priority level. | 
| void 	NVIC_SystemReset (void)        | 	Reset the system. |


Two important observations:

1. All interrupts after reset occurs at level 0, highest level. They must ALWAYS be defined to another level.
2. All interrupts are disabled after reset. They must explicitely be enabled. 

# Button API (Application Programming Interface)

The buttons are represented as bits in a 32 bit unsigned integer.

The function implemented are:

    void        Button_Init(uint32_t buttons);
    uint32_t    Button_Read(void);
    uint32_t    Button_ReadChanges(void);
    uint32_t    Button_ReadPressed(void);
    uint32_t    Button_ReadReleased(void);
    void        Button_SetCallback( void (*callback)(uint32_t parms) );

    
# Main function

The functionalities are split between the main function and the callback function. A call to WFI in the main loop puts the processor in a energy saving mode until an interrupt occurs.

    void buttoncallback(uint32_t v) {
    
        if( Button_ReadReleased() )
            LED_Toggle(LED1);
    
    }
    ...
    int main(void) {
    
        /* Configure LEDs */
        LED_Init(LED0|LED1);
    
        /* Configure buttons */
        Button_Init(BUTTON0|BUTTON1);
        Button_SetCallback(buttoncallback);
    
        /* Configure Sys Tick */
        SysTickConfig(SystemCoreClock/1000);
    
        LED_Write(0,LED0|LED1);
        /*
         * Read button loop. ATTENTION: No debounce
         */
        while (1) {
            __WFI();
        }
    
    }
    
    