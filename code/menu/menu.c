///*
// * menu.c
// *
// *  Created on: 2025年8月28日
// *      Author: Pathetic.
// */
//
//#include "menu.h"
//
//
////------------------------------------------------------------------------------
//// 函数简介     屏幕显示
//// 参数说明     无
//// 返回参数     无
//// 备注信息     v1.0    ips200
////------------------------------------------------------------------------------
//void screen_show(void)
//{
//    static uint8 page = 1, state = 1;
//    uint8 key_num = 0;
//    if(glb_flag->keyboard_scan_enable == 1)
//    {
//        glb_flag->keyboard_scan_enable = 0;
//        key_num = matrix_keyboard_scan();
//    }
//    // 功能切换
//    keyboard_mode_selection(key_num);
//
//    if(kybd_mode->keyboard_mode == CHANGE_MODE)
//    {
//        // 翻页
//        //向后翻页
//        if(key_num == 6)
//        {
//            if(page == MENU_PAGE_MAX)
//                page = 1;
//            else
//                page ++;
//            state = 1;
//            ips200_clear();
//        }
//        //向前翻页
//        else if(key_num == 4)
//        {
//            if(page == 1)
//                page = MENU_PAGE_MAX;
//            else
//                page --;
//            state = 1;
//            ips200_clear();
//        }
//
//        // 换项
//        //向下换项
//        if(key_num == 8)
//        {
//            ips200_clear();
//            if(page == 1)
//            {
//                if(state == 6)
//                    state = 1;
//                else
//                    state ++;
//            }
//            else if(page == 2)
//            {
//                if(state == 6)
//                    state = 1;
//                else
//                    state ++;
//            }
//            else if(page == 3)
//            {
//                if(state == 9)
//                    state = 1;
//                else
//                    state ++;
//            }
//            else if(page == 4)
//            {
//                if(state == 8)
//                    state = 1;
//                else
//                    state ++;
//            }
//            else if(page == MENU_PAGE_MAX)
//            {
//                if(state == 2)
//                    state = 1;
//                else
//                    state ++;
//            }
//        }
//        //向上换项
//        else if(key_num == 2)
//        {
//            ips200_clear();
//            if(page == 1)
//            {
//                if(state == 1)
//                    state = 6;
//                else
//                    state --;
//            }
//            else if(page == 2)
//            {
//                if(state == 1)
//                    state = 6;
//                else
//                    state --;
//            }
//            else if(page == 3)
//            {
//                if(state == 1)
//                    state = 9;
//                else
//                    state --;
//            }
//            else if(page == 4)
//            {
//                if(state == 1)
//                    state = 8;
//                else
//                    state --;
//            }
//            else if(page == MENU_PAGE_MAX)
//            {
//                if(state == 1)
//                    state = 2;
//                else
//                    state --;
//            }
//        }
//
//        // 确认
//        if(key_num == 5)
//        {
//            ips200_clear();
//            if(page == 1)
//            {
//                switch(state)
//                {
//                    case 1:
//                        if(glb_flag->Close_speed_enable == 0)
//                            ctrl_speed->open_speed_straight = keyboard_input_num();
//                        else
//                            ctrl_speed->close_speed_straight = keyboard_input_num();
//                        break;
//                    case 2:
//                        if(glb_flag->Close_speed_enable == 0)
//                            ctrl_speed->open_speed_cross= keyboard_input_num();
//                        else
//                            ctrl_speed->close_speed_cross = keyboard_input_num();
//                        break;
//                    case 3:
//                        if(glb_flag->Close_speed_enable == 0)
//                            ctrl_speed->open_speed_island = keyboard_input_num();
//                        else
//                            ctrl_speed->close_speed_island = keyboard_input_num();
//                        break;
//                    case 4:
//
//
//                        break;
//                    case 5:
//
//                        break;
//                    case 6:
//
//                        break;
//                    default:
//                        break;
//                }
//                kybd_temp->count_key = 0;
//            }
//            else if(page == 2)
//            {
//                switch(state)
//                {
//                    case 1:
//
//                        break;
//                    case 2:
//
//                        break;
//                    case 3:
//
//                        break;
//                    case 4:
//
//                        break;
//                    case 5:
//
//                        break;
//                    case 6:
//
//                        break;
//                    default:
//                        break;
//                }
//                kybd_temp->count_key = 0;
//            }
//            else if(page == 3)
//            {
//                switch(state)
//                {
//                    case 1:
//                        ctrl_steering->steering_kp_straight = (float)(1.0f * keyboard_input_num() / powf(10, kybd_temp->count_dot));
//                        break;
//                    case 2:
//                        ctrl_steering->steering_kd_straight = (float)(1.0f * keyboard_input_num() / powf(10, kybd_temp->count_dot));
//                        break;
//                    case 3:
//                        ctrl_steering->steering_kk_straight = (float)(1.0f * keyboard_input_num() / powf(10, kybd_temp->count_dot));
//                        break;
//                    case 4:
//                        ctrl_steering->steering_kp_cross = (float)(1.0f * keyboard_input_num() / powf(10, kybd_temp->count_dot));
//                        break;
//                    case 5:
//                        ctrl_steering->steering_kd_cross = (float)(1.0f * keyboard_input_num() / powf(10, kybd_temp->count_dot));
//                        break;
//                    case 6:
//                        ctrl_steering->steering_kk_cross = (float)(1.0f * keyboard_input_num() / powf(10, kybd_temp->count_dot));
//                        break;
//                    case 7:
//                        ctrl_steering->steering_kp_island = (float)(1.0f * keyboard_input_num() / powf(10, kybd_temp->count_dot));
//                        break;
//                    case 8:
//                        ctrl_steering->steering_kd_island = (float)(1.0f * keyboard_input_num() / powf(10, kybd_temp->count_dot));
//                        break;
//                    case 9:
//                        ctrl_steering->steering_kk_island = (float)(1.0f * keyboard_input_num() / powf(10, kybd_temp->count_dot));
//                        break;
//                    default:
//                        break;
//                }
//                kybd_temp->count_key = 0;
//                kybd_temp->count_dot = 0;
//                kybd_temp->count_keyboard_dot = 0;
//            }
//            else if(page == MENU_PAGE_MAX)
//            {
//                switch(state)
//                {
//                    case 1:
//                        ips200_show_string(8*8, 16*0, "    ");
//                        flash_save_parameters();
//                        ips200_show_string(8*8, 16*0, "done");
//                        break;
//                    case 2:
//                        ips200_clear();
//                        page = 1;
//                        break;
//                    default:
//                        break;
//                }
//            }
//        }
//
//        // 跳转
//        if(key_num == 10)
//        {
//            ips200_clear();
//            state = 1;
//            page = MENU_PAGE_MAX;
//        }
//    }
//    else if(kybd_mode->keyboard_mode == INPUT_MODE)
//    {
//        keyboard_input(key_num);
//        kybd_temp->count_keyboard_dot = kybd_temp->count_key - kybd_temp->count_dot;
//        kybd_temp->count_flash_dot = kybd_temp->count_dot;
//    }
//
//    // 显示
//    ips200_show_string(8*3, 16*18, "page");
//    ips200_show_uint(8*9, 16*18, page, 2);
//    if(page == 1 || page == 2 || page == 3)
//        ips200_show_char(0, 16*(2 * state - 1), '-');
//    else
//        ips200_show_char(0, 16*(1 * state - 1), '-');
//
//    if(page == 1)
//    {
//        if(glb_flag->Close_speed_enable == 0)
//        {
//            // 电机速度
//            ips200_show_string(8*1, 16*0, "os_straight");
//            ips200_show_int   (8*1, 16*1, ctrl_speed->open_speed_straight, 5);
//            ips200_show_string(8*1, 16*2, "os_cross");
//            ips200_show_int   (8*1, 16*3, ctrl_speed->open_speed_cross, 5);
//            ips200_show_string(8*1, 16*4, "os_island");
//            ips200_show_int   (8*1, 16*5, ctrl_speed->open_speed_island, 5);
//
//        }
//        else
//        {
//            // 电机速度
//            ips200_show_string(8*1, 16*0, "cs_straight");
//            ips200_show_int   (8*1, 16*1, ctrl_speed->close_speed_straight, 5);
//            ips200_show_string(8*1, 16*2, "cs_cross");
//            ips200_show_int   (8*1, 16*3, ctrl_speed->close_speed_cross, 5);
//            ips200_show_string(8*1, 16*4, "cs_island");
//            ips200_show_int   (8*1, 16*5, ctrl_speed->close_speed_island, 5);
//
//
//        }
//    }
//    else if(page == 2)
//    {
//
//
//    }
//    else if(page == 3)
//    {
//        // 直线
//        ips200_show_string(8*1, 16*0, "steering_kp_straight");
//        ips200_show_float (8*1, 16*1, ctrl_steering->steering_kp_straight, 3, 3);
//        ips200_show_string(8*1, 16*2, "steering_kd_straight");
//        ips200_show_float (8*1, 16*3, ctrl_steering->steering_kd_straight, 3, 3);
//        ips200_show_string(8*1, 16*4, "steering_kk_straight");
//        ips200_show_float (8*1, 16*5, ctrl_steering->steering_kk_straight, 3, 3);
//        // 十字
//        ips200_show_string(8*1, 16*6, "steering_kp_cross");
//        ips200_show_float (8*1, 16*7, ctrl_steering->steering_kp_cross, 3, 3);
//        ips200_show_string(8*1, 16*8, "steering_kd_cross");
//        ips200_show_float (8*1, 16*9, ctrl_steering->steering_kd_cross, 3, 3);
//        ips200_show_string(8*1, 16*10, "steering_kk_cross");
//        ips200_show_float (8*1, 16*11, ctrl_steering->steering_kk_cross, 3, 3);
//        // 环岛
//        ips200_show_string(8*1, 16*12, "steering_kp_island");
//        ips200_show_float (8*1, 16*13, ctrl_steering->steering_kp_island, 3, 3);
//        ips200_show_string(8*1, 16*14, "steering_kd_island");
//        ips200_show_float (8*1, 16*15, ctrl_steering->steering_kd_island, 3, 3);
//        ips200_show_string(8*1, 16*16, "steering_kk_island");
//        ips200_show_float (8*1, 16*17, ctrl_steering->steering_kk_island, 3, 3);
//
////        ips200_show_uint(8*12, 16*18, kybd_temp->count_key, 1);
//        ips200_show_string(8*12, 16*18, "count_dot:");
//        ips200_show_uint(8*18, 16*18, kybd_temp->count_dot, 1);
////        ips200_show_uint(8*16, 16*18, kybd_temp->count_keyboard_dot, 1);
////        ips200_show_uint(8*18, 16*18, kybd_temp->count_flash_dot, 1);
//    }
//    else if(page == 4)
//    {
//
//
//    }
//    else if(page == MENU_PAGE_MAX)
//    {
//        ips200_show_string(8*1, 16*0, "save the parameters");
//        ips200_show_string(8*1, 16*1, "cancel");
//    }
//}
