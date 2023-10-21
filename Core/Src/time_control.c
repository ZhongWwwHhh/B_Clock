/* USER CODE BEGIN 0 */

#include "time_control.h"
#include "struct_define.h"

// 时间加一秒
void time_add(Times *time_old)
{
    time_old->second++;
    if (60 <= time_old->second)
    {
        time_old->second = 0;
        time_old->minute++;
    }
    if (60 <= time_old->minute)
    {
        time_old->minute = 0;
        time_old->hour++;
    }
    if (24 <= time_old->hour)
    {
        time_old->hour = 0;
    }
}

// 延时响铃5分钟
void time_alarm_delay(Alarm_Setting *alarm_setting)
{
    alarm_setting->time_alart_real.minute += 5;
    if (60 <= alarm_setting->time_alart_real.minute)
    {
        alarm_setting->time_alart_real.minute %= 60;
        alarm_setting->time_alart_real.hour++;
    }
    if (24 <= alarm_setting->time_alart_real.hour)
    {
        alarm_setting->time_alart_real.hour = 0;
    }
}

/* USER CODE END 0 */
