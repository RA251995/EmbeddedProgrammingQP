#ifndef __BSP_H
#define __BSP_H

#include <stdint.h>

#define LED_ORANGE          (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_GREEN           (0x1U << 2U)

/* Function Prototypes */
void assert_failed(const char* file, int32_t line);
void SysTick_Handler(void);

#endif /* __BSP_H */
