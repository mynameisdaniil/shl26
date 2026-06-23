#include <stdint.h>

#define REG32(addr) (*(volatile uint32_t *)(addr))

#define RCC_APB2ENR  REG32(0x40021018)

#define GPIOB_CRH    REG32(0x40010C04)
#define GPIOB_ODR    REG32(0x40010C0C)

#define GPIOC_CRH    REG32(0x40011004)
#define GPIOC_ODR    REG32(0x4001100C)

#define PB_PIN       9
#define PB_MASK      (1u << PB_PIN)

#define PC13_MASK    (1u << 13)

void reset(void);

__attribute__((section(".vectors")))
const uint32_t vectors[] = {
    0x20005000,              // initial stack pointer
    (uint32_t)reset + 1      // reset handler, Thumb bit
};

void reset(void)
{
    RCC_APB2ENR |= (1u << 3) | (1u << 4);   // enable GPIOB + GPIOC

    // PB9 output push-pull, 2 MHz
    GPIOB_CRH = 0x44444424;

    // PC13 output push-pull, 2 MHz
    GPIOC_CRH = 0x44244444;

    while (1) {
        GPIOB_ODR ^= PB_MASK;
        GPIOC_ODR ^= PC13_MASK;

        for (volatile uint32_t i = 0; i < 800000; i++) {
        }
    }
}
