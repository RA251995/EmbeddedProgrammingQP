#ifndef __BSP_H
#define __BSP_H

#include "uc_ao.h"  /* uC/AO API */

void BSP_init(void);
void BSP_start(void);

void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);
void BSP_ledYellowOn(void);
void BSP_ledYellowOff(void);
void BSP_ledOrangeOn(void);
void BSP_ledOrangeOff(void);

enum EventSignals
{
    BUTTON_PRESSED_SIG = USER_SIG,
    BUTTON_RELEASED_SIG,
    TIMEOUT_SIG,
    /* ... */
};

extern Active *AO_BlinkyButton;

#endif /* __BSP_H */
