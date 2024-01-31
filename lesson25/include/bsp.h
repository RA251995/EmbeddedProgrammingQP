#ifndef __BSP_H
#define __BSP_H

#include <stdint.h>

#define BSP_TICKS_PER_SEC   (100U)

#define LED_ORANGE          (0x1U)
#define LED_YELLOW          (0x1U << 1U)
#define LED_GREEN           (0x1U << 2U)
#define TEST_PIN1           (0x1U << 3U)
#define TEST_PIN2           (0x1U << 4U)

/* Function Prototypes */
void BSP_init(void);
void SysTick_Handler(void);
void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);
void BSP_ledYellowOn(void);
void BSP_ledYellowOff(void);
void BSP_ledOrangeOn(void);
void BSP_ledOrangeOff(void);

void Q_onAssert(const char *module, int32_t id);

#endif /* __BSP_H */
