/* USER CODE BEGIN 0 */

#include "struct_define.h"
#include "time_control.h"

// 时间加一秒
void time_add(struct Time *time)
{
    time.second++;
    if (60 <= time.second)
    {
        time.second = 0;
        time.minute++;
    }
    if (60 <= time.minute)
    {
        time.minute = 0;
        time.hour++;
    }
    if (24 <= time.hour)
    {
        time.hour = 0;
    }
}

/* USER CODE END 0 */
