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

/* USER CODE END 0 */
