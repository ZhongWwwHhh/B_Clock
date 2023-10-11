/* USER CODE BEGIN 0 */
#include <stdint.h>

// 时间设定，应使用24小时制
struct Times
{
    int8_t hour;
    int8_t minute;
    int8_t second;
};

// 闹钟设置
struct Alarm_Setting
{
    // 闹钟鸣响频率
    int8_t alarm_frequency;
    // 闹钟时间
    struct Times time_alart;
};

// 显示控制
struct Screen
{
    // 需要显示的界面编号
    int8_t screen_display;
    // 每次刷新屏幕置为1，刷新后自动置零
    int8_t clean_display;
};

// 记录现在时间
extern struct Times time_now;

// 记录设置
extern struct Alarm_Setting alarm_setting;

// 记录屏幕内容
extern struct Screen screen;

/* USER CODE END 0 */
