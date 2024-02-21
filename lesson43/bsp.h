#ifndef __BSP_H
#define __BSP_H

#include <stdint.h>
#include <qpc.h>

#define BSP_TICKS_PER_SEC   (1000U)

enum
{
    TIMEOUT_SIG = Q_USER_SIG,
    BUTTON_PRESSED_SIG
};

void BSP_init(void);
void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);
void BSP_ledYellowOn(void);
void BSP_ledYellowOff(void);
void BSP_ledOrangeOn(void);
void BSP_ledOrangeOff(void);

extern QActive * const AO_Blinky1;
extern QActive * const AO_Blinky2;

#endif /* __BSP_H */
