/*
 * encoder_fun.h
 *
 *  Created on: Oct 12, 2023
 *      Author: 千涣
 */

#ifndef INC_ENCODER_FUN_H_
#define INC_ENCODER_FUN_H_



#endif /* INC_ENCODER_FUN_H_ */
#include <main.h>

int Spincount(void)
{
    int enc1_old=0;
    int enc1_new=0;
    int spintimes;
    int spindir=1;
    enc1_new = __HAL_TIM_GET_COUNTER(&htim3);
    if (enc1_new != enc1_old && abs(enc1_new - enc1_old) % 4 == 0)
    {
        int i = 0;
        if (abs(enc1_new - enc1_old) >= 10000)
        {
            i = 65536 - abs(enc1_new - enc1_old);
        }
        else
        {
            i = abs(enc1_new - enc1_old);
        }
        spintimes=i/4;
        if (enc1_new>enc1_old && enc1_new-enc1_old<10000)
        {
            spindir=-1;
        }
        enc1_old = enc1_new;
    }
    return spintimes,spindir;
}
