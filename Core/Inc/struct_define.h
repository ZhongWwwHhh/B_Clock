/* USER CODE BEGIN 0 */
#ifndef __STRUCT_DEFINE_H
#define __STRUCT_DEFINE_H

#include <stdint.h>

// 时间设定，应使用24小时制
typedef struct
{
    int8_t hour;
    int8_t minute;
    int8_t second;
} Times;

// 闹钟设置
typedef struct
{
    // 闹钟鸣响频率
    int8_t alarm_frequency;
    // 闹钟时间
    Times time_alart;
    // 是否在响铃
    int16_t alarming_time;
} Alarm_Setting;

// 显示控制
typedef struct
{
    // 需要显示的界面编号
    int8_t screen_display_num;
    // 每次刷新屏幕置为1，刷新后自动置零
    int8_t clean_display;
} Screen;

// LED pwm 控制
typedef struct
{
    uint16_t LedpwmVal;        // 占空比调整参数
    uint8_t LedpwmVal_Dir : 1; // 调整方向，1-递增，0递减
} Led_Control;

// 编码器状态
typedef struct
{
    uint8_t Left;
    uint8_t Right;
} Encoder_State;

#endif

/* USER CODE END 0 */
