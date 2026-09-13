#include "stm32_render.h"

void delay(uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        __asm__("nop"); // Prevents the compiler from optimizing the loop away
    }
}

static void spi1_init(void)
{
    RCC_APB2ENR |= (1U << 2);   // GPIOA clock
    RCC_APB2ENR |= (1U << 12);  // SPI1 clock
    RCC_APB2ENR |= (1U << 3);   // GPIOB clock

    // PA5 = SCK, PA7 = MOSI -> AF push-pull 50 MHz
    GPIOA_CRL &= ~(0xFU << 20);
    GPIOA_CRL |=  (0xBU << 20);

    GPIOA_CRL &= ~(0xFU << 28);
    GPIOA_CRL |=  (0xBU << 28);

    // PA6 = MISO -> input floating
    GPIOA_CRL &= ~(0xFU << 24);
    GPIOA_CRL |=  (0x4U << 24);
    
    GPIOB_CRL &= ~(0xFU << 0);
    GPIOB_CRL |=  (0x3U << 0);  // output push-pull, 50 MHz

    SPI1_CR1 = 0;
    SPI1_CR1 |= (1U << 2);   // MSTR
    SPI1_CR1 |= (1U << 9);   // SSM
    SPI1_CR1 |= (1U << 8);   // SSI
    SPI1_CR1 |= (7U << 3);   // BR = /256
    SPI1_CR1 |= (0U << 1);   // CPOL = 0
    SPI1_CR1 |= (0U << 0);   // CPHA = 0
    SPI1_CR1 |= (1U << 11); // DFF = 1, 16-bit frame
    SPI1_CR1 |= (1U << 6);   // SPE
}
static void spi1_send(uint16_t data)
{
    while (!(SPI1_SR & (1U << 1))) {
    }
    SPI1_DR = data;
    while (SPI1_SR & (1U << 7)) {
    }
    (void)SPI1_DR;
}

