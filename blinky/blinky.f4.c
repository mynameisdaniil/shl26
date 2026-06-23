/*
 * Minimal PC13 LED blinker for the STM32F411CEU6 “core board” by WeAct Studio.
 *
 * This program is a direct adaptation of the earlier STM32F103 Blue Pill
 * example.  It eschews the STM32 HAL and C runtime startup code, uses
 * only a tiny vector table (stack pointer plus reset handler) and
 * programs the hardware through the device’s memory‑mapped registers.
 *
 * On the F4 series, the RCC and GPIO peripherals reside at different
 * addresses than on the F1.  The LED on the WeAct core board is
 * connected to pin PC13 and, as on the Blue Pill, is typically
 * active‑low.  We enable the AHB1 clock for GPIOC, configure PC13 as
 * a general‑purpose output (MODER13=0b01) and then toggle the output
 * state in a tight loop.  A simple delay loop provides a visible
 * blink.  The initial stack pointer is set near the top of the
 * STM32F411’s 128 KiB SRAM (0x20020000).
 */

#include <stdint.h>

/* Base addresses and offsets for the STM32F411 peripherals.  See
 * RM0383 “STM32F411 reference manual” for details.
 */
#define RCC_BASE        0x40023800U
#define GPIOC_BASE      0x40020800U

/* Offsets of the registers we use.  */
#define RCC_AHB1ENR_OFF 0x30U
#define GPIO_MODER_OFF  0x00U
#define GPIO_ODR_OFF    0x14U

/* Volatile accessors.  */
#define REG32(addr) (*(volatile uint32_t *)(addr))

#define RCC_AHB1ENR REG32(RCC_BASE + RCC_AHB1ENR_OFF)
#define GPIOC_MODER REG32(GPIOC_BASE + GPIO_MODER_OFF)
#define GPIOC_ODR   REG32(GPIOC_BASE + GPIO_ODR_OFF)

void reset(void);

/* Minimal vector table: initial SP and reset handler.  */
__attribute__((section(".isr_vector")))
const uint32_t vectors[] = {
    0x20020000U,          /* initial stack pointer at top of 128 KiB SRAM */
    (uint32_t)reset + 1U  /* reset handler address with Thumb bit set */
};

/* Reset handler: set up GPIOC pin 13 and blink it forever.  */
void reset(void)
{
    /* Enable the clock for GPIO port C (bit 2) on the AHB1 bus.  */
    RCC_AHB1ENR |= (1U << 2);
    /* Configure PC13 as general‑purpose output.  Each pin’s mode is
     * controlled by two bits in MODER; for pin 13 (bits 27:26) we
     * clear the field then set it to 0b01 (output).  */
    GPIOC_MODER &= ~(3U << (13 * 2));
    GPIOC_MODER |=  (1U << (13 * 2));
    /* Loop forever, toggling PC13 with a crude delay.  On the board
     * the LED is wired to VCC through a resistor, so driving the pin
     * low turns it on and driving it high turns it off.  */
    while (1) {
        /* Toggle the PC13 output bit.  */
        GPIOC_ODR ^= (1U << 13);
        /* Busy‑wait delay for a visible blink.  Adjust the count to
         * change the blink period.  */
        for (volatile uint32_t i = 0; i < 1000000U; i++) {
            __asm__ volatile ("nop");
        }
    }
}
