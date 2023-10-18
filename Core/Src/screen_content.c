
#include "screen_content.h"

extern Times time_now;
extern TempDataStruct Tempdata;

void screen_show(int8_t *screen_display_num, int8_t *clean_display)
{
    if (*clean_display)
    {
        OLED_Clear();
        *clean_display = 0;
    }

    switch (*screen_display_num)
    {
    case 0: // 开机画面
        OLED_ShowString(0, 0, "B Clock", 16, 1);
        OLED_ShowString(0, 17, "ZhongWuHe ZhouJiaWei", 8, 1);
        OLED_ShowString(0, 26, "HeMinYi", 8, 1);
        OLED_ShowString(0, 35, "2023010911021", 8, 1);
        OLED_ShowString(0, 44, "2023010911030", 8, 1);
        OLED_ShowString(0, 53, "2023010911011", 8, 1);

        break;

    case 1: // 主屏幕
        // 显示时间
        OLED_ShowNum(16, 2, time_now.hour, 2, 24, 1);
        OLED_ShowChar(40, 2, ':', 24, 1);
        OLED_ShowNum(52, 2, time_now.minute, 2, 24, 1);
        OLED_ShowChar(76, 2, ':', 24, 1);
        OLED_ShowNum(88, 2, time_now.second, 2, 24, 1);

        // 显示温度
        LM75A_GetTemperature();
        OLED_ShowString(16, 28, "TEMP:", 16, 1);
        OLED_ShowNum(56, 28, Tempdata.Tens, 1, 16, 1);
        OLED_ShowNum(64, 28, Tempdata.Unit, 1, 16, 1);
        OLED_ShowChar(72, 28, '.', 16, 1);
        OLED_ShowNum(80, 28, Tempdata.Decimals, 1, 16, 1);

        // 显示提示
        OLED_ShowString(16, 46, "Press to setting", 8, 1);
        break;

    default:
        break;
    };

    OLED_Refresh();
}