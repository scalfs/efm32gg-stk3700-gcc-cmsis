01 - Blink {#mainpage}
==========

This is the 1th version of Blink. It uses direct access to register of the EFM32GG990F1024 
microcontroller.

It is possible to use the Gecko SDK Library, which includes a HAL Library. For didatical reasons
and to avoid the restrictions imposed by the license, the direct access to registers is used.

The architecture of the software is shown below.

    +------------------------------------------------------+
    |                     Application                      |
    +------------------------------------------------------+
    |                      Hardware                        |
    +------------------------------------------------------+
    

To access the registers, it is necessary to know their addresses and fields. These information 
can be found the datasheet and other documents from the manufacturer (Silicon Labes). But the manufacturer (Silicon Labs) provides a CMSIS compatible files in the platform folder of the Gecko SDK Library.

The main documents are:

* [EFM32GG Reference Manual Giant Gecko Series](https://www.silabs.com/documents/public/reference-manuals/EFM32GG-RM.pdf)
* [Starter Kit EFM32GG-STK3700 User Manual](https://www.silabs.com/documents/public/user-guides/efm32gg-stk3700-ug.pdfhttps://www.silabs.com/documents/public/reference-manuals/EFM32GG-RM.pdf)
* [EFM32 Microcontroller Family Cortex M3 Reference Manual](https://www.silabs.com/documents/public/reference-manuals/EFM32-Cortex-M3-RM.pdf)
* [EFM32GG990 F1024/F512 Datasheet](https://www.silabs.com/documents/public/data-sheets/EFM32GG990.pdf)


The platform folder has the following subfolders of interest: Device and CMSIS. In the `Device/SiliconLabs/EFM32GG/Include/` folder there is a header file named `emf32gg990f1024.h`, which includes the definition of all registers of the microcontroller. One has to be careful because it includes a lot of other header files (`emf32gg_*.h`).

It is possible to include the `emf32gg990f1024.h` file directly in the code, like below.


    #include <emf32gg990f1024.h>


but a better alternative is to use a generic include and define which microcontroller as a define in the command line.


    #include "em_device.h"


The command line must then include the -DEMF32GG990F1024 flag. To use this alternative one has to copy the `em_device.h` to the project folder and used "" instead of <> in the include line.

Instead of using symbols like 0x2 to access the bit to control the LED0, it is better to use a  symbol LED0 as below.

    #define LED0 0x2

To define it a common idiom is to use a BIT macro defined as below (the parenthesis are recommended to avoid surprises).

    #define BIT(N) (1<<(N))

The symbols to access the LEDs in the GPIO Port E registers can be defined as

    #define LED0 BIT(2)
    #define LED1 BIT(3)


To use the GPIO Port, where the LEDs are attached, it is necessary to:

* Enable clock for peripherals
* Enable clock for GPIO
* Configure pins as outputs
* Set them to the desired values

To enable clock for peripherals, the HFPERCLKEN bit in the HFPERCLKDIV register must be set. To enable clock for the GPIO
the GPIO bit of the HFPERCLKNE0 register must be set. Both of them are done by or'ing the mask already defined in the header files to the registers.

    /* Enable Clock for GPIO */
    CMU->HFPERCLKDIV |= CMU_HFPERCLKDIV_HFPERCLKEN;     // Enable HFPERCLK
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;           // Enable HFPERCKL for GPIO

To access the register for GPIO Port E, a constant is defined, that points to the corresponding memory address.

    GPIO_P_TypeDef * const GPIOE = &(GPIO->P[4]);  // GPIOE

To configure the pins as outputs one has to set ghe mode fields in the MODE registers. There are two MODE registers: MODEL to configure pins 0 to 7 and MODEH, for pins 8 to 15. To drive the LEDs, the fields must be set to Push-Pull configuration, but just Or a binary value ist not enough. The field must be cleared (set to 0) before.

    /* Configure Pins in GPIOE */
    GPIOE->MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK|_GPIO_P_MODEL_MODE3_MASK);       // Clear bits
    GPIOE->MODEL |= (GPIO_P_MODEL_MODE2_PUSHPULL|GPIO_P_MODEL_MODE3_PUSHPULL);  // Set bits

Finallym to set the desired value, one can Or a value with a bit 1 in the desired position and all other bits set to 0.
    GPIOE->DOUT |= LED0;

To clear it, one must AND a value with a bit 0 in the desired position and all other bit set to 1
    GPIOE->DOUT &= ~LED0;

To toggle a bin, one can XOR a value with a bit 1 in the desired position (and other bits set to 0).
    GPIOE->DOUT ^= LED0;



