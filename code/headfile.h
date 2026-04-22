/*
 * headfile.h
 *
 *  Created on: 2025年8月16日
 *      Author: Pathetic.
 */

#ifndef CODE_HEADFILE_H_
#define CODE_HEADFILE_H_

#include "zf_common_headfile.h"

//-------------------电机------------------

//#define SMALL_DRIVER_UART                       (UART_2        )
//#define SMALL_DRIVER_BAUDRATE                   (460800        )
//#define SMALL_DRIVER_RX                         (UART2_TX_P02_0)
//#define SMALL_DRIVER_TX                         (UART2_RX_P02_1)

//#define SMALL_DRIVER_UART                       (UART_3        )
//#define SMALL_DRIVER_BAUDRATE                   (460800        )
//#define SMALL_DRIVER_RX                         (UART3_TX_P20_0)
//#define SMALL_DRIVER_TX                         (UART3_RX_P20_3)
#define SMALL_DRIVER_UART                       (UART_3        )
#define SMALL_DRIVER_BAUDRATE                   (460800        )
#define SMALL_DRIVER_RX                         (UART3_TX_P15_6)
#define SMALL_DRIVER_TX                         (UART3_RX_P15_7)
//-------------------舵机------------------
#define steering_freq       (120)
#define steering_pwm        (ATOM0_CH1_P33_9)
#define steering_right       (2975)
#define steering_left       (675)
#define steering_middle     (1900)//middle1900,left675,right2975

//-------------------按键------------------
//矩阵键盘
#define first_line          (P13_0)
#define second_line         (P13_1)
#define third_line          (P13_2)
#define fourth_line         (P13_3)
#define first_premolar      (P22_0)
#define second_premolar     (P22_1)
#define third_premolar      (P22_2)

//------------------蜂鸣器------------------
#define buzzer              (P33_10)

//-------------------LED-------------------
#define LED                 (P20_8)

//-------------------脉冲-------------------
#define maichong            (49479)

#endif /* CODE_HEADFILE_H_ */
