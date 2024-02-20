#ifndef __BSP_H
#define __BSP_H

#include "qpc.h" /* QP/C API */

#define BSP_TICKS_PER_SEC   (100U)

void BSP_init(void);
void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);
void BSP_ledYellowOn(void);
void BSP_ledYellowOff(void);
void BSP_ledOrangeOn(void);
void BSP_ledOrangeOff(void);

enum EventSignals
{
    BUTTON_PRESSED_SIG = Q_USER_SIG,
    BUTTON_RELEASED_SIG,
    BUTTON2_PRESSED_SIG,
    BUTTON2_RELEASED_SIG,
    TIMEOUT_SIG,
    /* ... */
    MAX_SIG
};

extern QActive *AO_TimeBomb;

#endif /* __BSP_H */
