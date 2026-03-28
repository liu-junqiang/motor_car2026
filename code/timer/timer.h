/*
 * timer.h
 *
 *  Created on: 2025Äê9ÔÂ3ÈÕ
 *      Author: Pathetic.
 */

#ifndef CODE_TIMER_TIMER_H_
#define CODE_TIMER_TIMER_H_

#include "zf_common_headfile.h"

typedef struct
{
    uint16 count_time;
}timer_temp_struct;

extern timer_temp_struct* tim_temp;

extern void timer_keyboard(void);

#endif /* CODE_TIMER_TIMER_H_ */
