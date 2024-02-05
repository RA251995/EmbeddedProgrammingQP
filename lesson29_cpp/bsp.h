#ifndef __BSP_H
#define __BSP_H

#include "qpc.h"
#include <stdint.h>

#define BSP_TICKS_PER_SEC   (100U)

/* Function Prototypes */
void BSP_init(void);

void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);
void BSP_ledGreenToggle(void);

void BSP_ledYellowOn(void);
void BSP_ledYellowOff(void);
void BSP_ledYellowToggle(void);

void BSP_ledOrangeOn(void);
void BSP_ledOrangeOff(void);
void BSP_ledOrangeToggle(void);

void BSP_sendMorseCode(uint32_t bitmask);

extern QXSemaphore SW1_sem;

#endif /* __BSP_H */
