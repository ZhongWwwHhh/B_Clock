
#include "screen_content.h"

extern Times time_now;
extern Alarm_Setting alarm_setting;
extern TempDataStruct Tempdata;
extern Encoder_State encoder_state;
extern Screen screen;

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
        OLED_ShowString(16, 28, "TEMP:", 16, 1);
        OLED_ShowNum(56, 28, Tempdata.Tens, 1, 16, 1);
        OLED_ShowNum(64, 28, Tempdata.Unit, 1, 16, 1);
        OLED_ShowChar(72, 28, '.', 16, 1);
        OLED_ShowNum(80, 28, Tempdata.Decimals, 1, 16, 1);

        // 显示提示
        OLED_ShowString(16, 50, "Press to setting", 8, 1);
        break;

    case 2: // 设置画面
        if (screen.screen_display_choose < 0 || screen.screen_display_choose > 3)
        {
            // 限定选择范围
            screen.screen_display_choose = 0;
        }
        OLED_ShowString(0, 02, " Back            ", 12, screen.screen_display_choose == 0 ? 0 : 1);
        OLED_ShowString(0, 16, " Time Setting    ", 12, screen.screen_display_choose == 1 ? 0 : 1);
        OLED_ShowString(0, 30, " Alarm Setting   ", 12, screen.screen_display_choose == 2 ? 0 : 1);
        OLED_ShowString(0, 44, " Bluetooth       ", 12, screen.screen_display_choose == 3 ? 0 : 1);
        break;

    case 3: // 蓝牙画面
        OLED_ShowString(0, 2, "PIN: 1234", 16, 1);
        OLED_ShowString(0, 20, "Connect and Set", 8, 1);
        OLED_ShowString(16, 50, "Press to back", 8, 1);
        break;

    case 4: // 时间设定界面
        if (screen.screen_display_choose > 2)
        {
            // 已经完成设置，回主界面
            screen.screen_display_num = 1;
            screen.screen_display_choose = -1;
            screen.clean_display = 1;
            break;
        }
        OLED_ShowString(0, 2, "Now:", 8, 1);
        OLED_ShowString(0, 12, "Hour:", 8, screen.screen_display_choose == 0 ? 0 : 1);
        OLED_ShowNum(30, 12, time_now.hour, 2, 8, screen.screen_display_choose == 0 ? 0 : 1);
        OLED_ShowString(50, 12, "Minute:", 8, screen.screen_display_choose == 1 ? 0 : 1);
        OLED_ShowNum(92, 12, time_now.minute, 2, 8, screen.screen_display_choose == 1 ? 0 : 1);
        OLED_ShowString(0, 22, "Second:", 8, screen.screen_display_choose == 2 ? 0 : 1);
        OLED_ShowNum(42, 22, time_now.second, 2, 8, screen.screen_display_choose == 2 ? 0 : 1);

        break;

    case 5: // 时间设定界面
        if (screen.screen_display_choose > 3)
        {
            // 已经完成设置，回主界面
            screen.screen_display_num = 1;
            screen.screen_display_choose = -1;
            screen.clean_display = 1;
            break;
        }
        OLED_ShowString(0, 2, "Alarm:", 8, 1);
        OLED_ShowString(0, 12, "Hour:", 8, screen.screen_display_choose == 0 ? 0 : 1);
        OLED_ShowNum(30, 12, alarm_setting.time_alart.hour, 2, 8, screen.screen_display_choose == 0 ? 0 : 1);
        OLED_ShowString(50, 12, "Minute:", 8, screen.screen_display_choose == 1 ? 0 : 1);
        OLED_ShowNum(92, 12, alarm_setting.time_alart.minute, 2, 8, screen.screen_display_choose == 1 ? 0 : 1);
        OLED_ShowString(0, 22, "Second:", 8, screen.screen_display_choose == 2 ? 0 : 1);
        OLED_ShowNum(42, 22, alarm_setting.time_alart.second, 2, 8, screen.screen_display_choose == 2 ? 0 : 1);
        OLED_ShowString(0, 32, "Frequency:", 8, screen.screen_display_choose == 3 ? 0 : 1);
        OLED_ShowNum(60, 32, alarm_setting.alarm_frequency, 2, 8, screen.screen_display_choose == 3 ? 0 : 1);
    };

    OLED_Refresh();
}