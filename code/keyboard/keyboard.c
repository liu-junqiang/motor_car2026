/*
 * keyboard.c
 *
 *  Created on: 2025年8月17日
 *      Author: Pathetic.
 */

#include "keyboard.h"

// 结构体定义
keyboard_mode_struct keyboard_mode_selected;
keyboard_temp_struct keyboard_temp;

// 结构体指针初始化
keyboard_temp_struct* kybd_temp = &keyboard_temp;
keyboard_mode_struct* kybd_mode = &keyboard_mode_selected;

//------------------------------------------------------------------------------
// 函数简介     按键扫描
// 参数说明     无
// 返回参数     按键号
// 备注信息     v1.0    矩阵键盘
//------------------------------------------------------------------------------
uint8 matrix_keyboard_scan(void)
{
    unsigned char line_number = 0, premolar_number = 0;
    uint8 key_number = 0;
    uint8 delay_time_ms = 60;

    gpio_init(first_line,       GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(second_line,      GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(third_line,       GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(fourth_line,      GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(first_premolar,   GPI, GPIO_LOW,  GPI_PULL_DOWN);
    gpio_init(second_premolar,  GPI, GPIO_LOW,  GPI_PULL_DOWN);
    gpio_init(third_premolar,   GPI, GPIO_LOW,  GPI_PULL_DOWN);

    if (gpio_get_level(first_premolar) == 1 || gpio_get_level(second_premolar) == 1 || gpio_get_level(third_premolar) == 1)
    {
        system_delay_ms(delay_time_ms);
        if (gpio_get_level(first_premolar) == 1 || gpio_get_level(second_premolar) == 1 || gpio_get_level(third_premolar) == 1)
        {
            if (gpio_get_level(first_premolar) == 1 && gpio_get_level(second_premolar) != 1 && gpio_get_level(third_premolar) != 1)
                premolar_number = 1;
            else if (gpio_get_level(first_premolar) != 1 && gpio_get_level(second_premolar) == 1 && gpio_get_level(third_premolar) != 1)
                premolar_number = 2;
            else if (gpio_get_level(first_premolar) != 1 && gpio_get_level(second_premolar) != 1 && gpio_get_level(third_premolar) == 1)
                premolar_number = 3;

            gpio_init(first_line,       GPI, GPIO_LOW,  GPI_PULL_DOWN);
            gpio_init(second_line,      GPI, GPIO_LOW,  GPI_PULL_DOWN);
            gpio_init(third_line,       GPI, GPIO_LOW,  GPI_PULL_DOWN);
            gpio_init(fourth_line,      GPI, GPIO_LOW,  GPI_PULL_DOWN);
            gpio_init(first_premolar,   GPO, GPIO_HIGH, GPO_PUSH_PULL);
            gpio_init(second_premolar,  GPO, GPIO_HIGH, GPO_PUSH_PULL);
            gpio_init(third_premolar,   GPO, GPIO_HIGH, GPO_PUSH_PULL);

            system_delay_ms(delay_time_ms);
            if (gpio_get_level(first_line) == 1 && gpio_get_level(second_line) != 1 && gpio_get_level(third_line) != 1 && gpio_get_level(fourth_line) != 1)
                line_number = 1;
            else if (gpio_get_level(first_line) != 1 && gpio_get_level(second_line) == 1 && gpio_get_level(third_line) != 1 && gpio_get_level(fourth_line) != 1)
                line_number = 2;
            else if (gpio_get_level(first_line) != 1 && gpio_get_level(second_line) != 1 && gpio_get_level(third_line) == 1 && gpio_get_level(fourth_line) != 1)
                line_number = 3;
            else if (gpio_get_level(first_line) != 1 && gpio_get_level(second_line) != 1 && gpio_get_level(third_line) != 1 && gpio_get_level(fourth_line) == 1)
                line_number = 4;
        }
    }
    if (line_number == 0 && premolar_number == 0)
        key_number = 0;
    else
        key_number = ((line_number - 1) * 3 + premolar_number);
    return key_number;
}

//------------------------------------------------------------------------------
// 函数简介     键盘功能切换
// 参数说明     按键号
// 返回参数     无
// 备注信息     v1.0    矩阵键盘
//------------------------------------------------------------------------------
void keyboard_mode_selection(uint8 key_num)
{
    if(key_num == 12)
    {
        ips200_clear();
        if(keyboard_mode_selected.keyboard_mode == INPUT_MODE)
            keyboard_mode_selected.keyboard_mode = CHANGE_MODE;
        else if(keyboard_mode_selected.keyboard_mode == CHANGE_MODE)
            keyboard_mode_selected.keyboard_mode = INPUT_MODE;
    }
    if(keyboard_mode_selected.keyboard_mode == INPUT_MODE)
        ips200_show_string(16*4, 16*19, "keyboard_mode: input");
    else if(keyboard_mode_selected.keyboard_mode == CHANGE_MODE)
        ips200_show_string(16*4, 16*19, "keyboard_mode: change");
}

//------------------------------------------------------------------------------
// 函数简介     键盘输入
// 参数说明     按键号
// 返回参数     无
// 备注信息     v1.0    矩阵键盘
//------------------------------------------------------------------------------
void keyboard_input(uint8 key_num)
{
    switch(key_num)
    {
        case 1:
            kybd_temp->number_temp[kybd_temp->count_key] = 1;
            kybd_temp->count_key++;
            ips200_clear();
            break;
        case 2:
            kybd_temp->number_temp[kybd_temp->count_key] = 2;
            kybd_temp->count_key++;
            ips200_clear();
            break;
        case 3:
            kybd_temp->number_temp[kybd_temp->count_key] = 3;
            kybd_temp->count_key++;
            ips200_clear();
            break;
        case 4:
            kybd_temp->number_temp[kybd_temp->count_key] = 4;
            kybd_temp->count_key++;
            ips200_clear();
            break;
        case 5:
            kybd_temp->number_temp[kybd_temp->count_key] = 5;
            kybd_temp->count_key++;
            ips200_clear();
            break;
        case 6:
            kybd_temp->number_temp[kybd_temp->count_key] = 6;
            kybd_temp->count_key++;
            ips200_clear();
            break;
        case 7:
            kybd_temp->number_temp[kybd_temp->count_key] = 7;
            kybd_temp->count_key++;
            ips200_clear();
            break;
        case 8:
            kybd_temp->number_temp[kybd_temp->count_key] = 8;
            kybd_temp->count_key++;
            ips200_clear();
            break;
        case 9:
            kybd_temp->number_temp[kybd_temp->count_key] = 9;
            kybd_temp->count_key++;
            ips200_clear();
            break;
        case 11:
            kybd_temp->number_temp[kybd_temp->count_key] = 0;
            kybd_temp->count_key++;
            ips200_clear();
            break;
        case 10:
            glb_flag->keyboard_timer_flag = 1;
            system_delay_ms(10);
            if(glb_flag->keyboard_long_press_flag == 1)
            {
                for(uint8 i = 0; i < KEYBOARD_NUM_INPUT_LEN; i++)
                    kybd_temp->number_temp[i] = 0;
                kybd_temp->count_key = 0;
                kybd_temp->count_dot = 0;
                kybd_temp->count_keyboard_dot = 0;
                ips200_clear();
                glb_flag->keyboard_long_press_flag = 0;
                glb_flag->keyboard_timer_flag = 0;
            }
            else
                kybd_temp->count_dot ++;
            break;
        default:
            glb_flag->keyboard_timer_flag = 0;
            break;
    }
}

//------------------------------------------------------------------------------
// 函数简介     键盘输入值
// 参数说明     无
// 返回参数     输入值
// 备注信息     v1.0    矩阵键盘
//------------------------------------------------------------------------------
uint16 keyboard_input_num(void)
{
    uint16 number_input = 0;
    for(uint8 i = 0; i < kybd_temp->count_key; i++)
        number_input += powf(10, kybd_temp->count_key - 1 - i) * kybd_temp->number_temp[i];
    if(kybd_temp->count_key > 2 && kybd_temp->number_temp[0] !=0)
        number_input += 1;
    return number_input;
}

//------------------------------------------------------------------------------
// 函数简介     允许扫描按键
// 参数说明     无
// 返回参数     无
// 备注信息     v1.0    矩阵键盘
//------------------------------------------------------------------------------
void keyboard_scan_enable(void)
{
    glb_flag->keyboard_scan_enable = 1;
}
