#ifndef __BSP_H
#define __BSP_H

#include <stdint.h>

#define BSP_TICKS_PER_SEC   (1000U)

extern QXSemaphore SW1_sem;

void BSP_init(void);
void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);
void BSP_ledYellowOn(void);
void BSP_ledYellowOff(void);
void BSP_ledOrangeOn(void);
void BSP_ledOrangeOff(void);

#endif /* __BSP_H */
