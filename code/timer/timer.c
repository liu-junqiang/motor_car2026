/*
 * timer.c
 *
 *  Created on: 2025年9月3日
 *      Author: Pathetic.
 */

#include "timer.h"

// 结构体定义
timer_temp_struct timer_temp;
// 结构体指针初始化
timer_temp_struct* tim_temp = &timer_temp;

//------------------------------------------------------------------------------
// 函数简介     键盘长短按计时
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0    矩阵键盘
//------------------------------------------------------------------------------
void timer_keyboard(void)
{
    if(glb_flag->keyboard_timer_flag == 1)
    {
        tim_temp->count_time ++;
        if(tim_temp->count_time >= 100)
        {
            glb_flag->keyboard_long_press_flag = 1;
            tim_temp->count_time = 0;
            glb_flag->keyboard_timer_flag = 0;
        }
    }
    else
        tim_temp->count_time = 0;
}
