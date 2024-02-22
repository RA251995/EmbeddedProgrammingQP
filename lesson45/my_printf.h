#ifndef __MY_PRINTF_H
#define __MY_PRINTF_H

#ifdef SPY /* Tracing enabled? */

#include <stdio.h>

#define MY_PRINTF(format, ...) printf(format, ##__VA_ARGS__)
#define MY_PRINTF_INIT()       printf_init()

void printf_init(void);

#else /* Tracing disabled */

#define MY_PRINTF(format, ...) (0)
#define MY_PRINTF_INIT()       ((void)0)

#endif /* SPY */

#endif /* __MY_PRINTF_H */
