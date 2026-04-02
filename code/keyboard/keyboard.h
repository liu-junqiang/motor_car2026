/*
 * keyboard.h
 *
 *  Created on: 2025年8月17日
 *      Author: Pathetic.
 */

#ifndef CODE_KEYBOARD_KEYBOARD_H_
#define CODE_KEYBOARD_KEYBOARD_H_

#include "zf_common_headfile.h"

// 键盘输入最大长度
#define KEYBOARD_NUM_INPUT_LEN      (5)

typedef enum
{
    INPUT_MODE,
    CHANGE_MODE,
}keyboard_mode_enum;

typedef struct
{
    keyboard_mode_enum keyboard_mode;
}keyboard_mode_struct;

typedef struct
{
    uint8 count_key;
    uint8 count_dot;
    uint8 count_flash_dot;
    uint8 count_keyboard_dot;
    uint16 number_temp[KEYBOARD_NUM_INPUT_LEN];
}keyboard_temp_struct;

extern keyboard_mode_struct* kybd_mode;
extern keyboard_temp_struct* kybd_temp;

extern uint8 matrix_keyboard_scan(void);
extern void keyboard_mode_selection(uint8 key_num);
extern void keyboard_input(uint8 key_num);
extern uint16 keyboard_input_num(void);
extern void keyboard_scan_enable(void);

#endif /* CODE_KEYBOARD_KEYBOARD_H_ */
