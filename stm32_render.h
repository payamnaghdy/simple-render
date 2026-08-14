#include "display.h"

#define WIDTH 296
#define HEIGHT 128

#define RCC_APB2ENR  (*(volatile uint32_t*)0x40021018)
#define GPIOC_CRH    (*(volatile uint32_t*)0x40011004)
#define GPIOC_ODR    (*(volatile uint32_t*)0x4001100C)


#define RCC_BASE   0x40021000UL
#define GPIOA_BASE  0x40010800UL
#define SPI1_BASE   0x40013000UL

#define RCC_APB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define GPIOA_CRL   (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define SPI1_CR1    (*(volatile uint32_t *)(SPI1_BASE + 0x00))
#define SPI1_SR     (*(volatile uint32_t *)(SPI1_BASE + 0x08))
#define SPI1_DR     (*(volatile uint32_t *)(SPI1_BASE + 0x0C))

#define GPIOB_BASE  0x40010C00UL
#define GPIOB_CRL   (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_ODR   (*(volatile uint32_t *)(GPIOB_BASE + 0x0C))


#define CS_LOW()   (GPIOB_ODR &= ~(1U << 0))
#define CS_HIGH()  (GPIOB_ODR |=  (1U << 0))

void delay(uint32_t count);
static void spi1_init(void);
static void spi1_send(uint8_t data);
