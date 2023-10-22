
#include "screen_content.h"

extern Times time_now;
extern Timing timing;
extern Alarm_Setting alarm_setting;
extern TempDataStruct Tempdata;
extern Encoder_State encoder_state;
extern Screen screen;
extern Bluetooth_Setting bluetooth_setting;

#include <stdlib.h>
extern Times time_game;

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

        // 显示闹钟被delay
        if (alarm_setting.is_alarm_delayed)
        {
            OLED_ShowChar(92, 28, 'D', 16, 1);
            OLED_ShowNum(100, 28, alarm_setting.is_alarm_delayed, 1, 16, 1);
        }

        // 显示提示
        OLED_ShowString(16, 50, "Press to setting", 8, 1);
        break;

    case 2: // 设置画面
        if (screen.screen_display_choose < 0 || screen.screen_display_choose > 4)
        {
            // 限定选择范围
            screen.screen_display_choose = 0;
        }
        OLED_ShowString(0, 02, " Back            ", 8, screen.screen_display_choose == 0 ? 0 : 1);
        OLED_ShowString(0, 12, " Time Setting    ", 8, screen.screen_display_choose == 1 ? 0 : 1);
        OLED_ShowString(0, 22, " Alarm Setting   ", 8, screen.screen_display_choose == 2 ? 0 : 1);
        OLED_ShowString(0, 32, " Bluetooth       ", 8, screen.screen_display_choose == 3 ? 0 : 1);
        OLED_ShowString(0, 42, " Timing          ", 8, screen.screen_display_choose == 4 ? 0 : 1);
        break;

    case 3: // 蓝牙画面
        OLED_ShowString(2, 2, "PIN: ", 16, 1);
        OLED_ShowNum(47, 2, bluetooth_setting.bluetooth_pin, 4, 16, 1);
        OLED_ShowString(2, 20, "NAME: Clock", 16, 1);
        OLED_ShowString(2, 38, "Connect and Set", 8, 1);
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
        // 显示提示
        OLED_ShowString(0, 44, "Rotate to change", 8, 1);
        OLED_ShowString(0, 54, "Press to next", 8, 1);
        break;

    case 5: // 闹钟设定界面
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
        // 显示提示
        OLED_ShowString(0, 44, "Rotate to change", 8, 1);
        OLED_ShowString(0, 54, "Press to next", 8, 1);
        break;

    case 6: // 正计时界面
        // 显示计时时间
        OLED_ShowNum(16, 2, timing.time_timing.hour, 2, 24, 1);
        OLED_ShowChar(40, 2, ':', 24, 1);
        OLED_ShowNum(52, 2, timing.time_timing.minute, 2, 24, 1);
        OLED_ShowChar(76, 2, ':', 24, 1);
        OLED_ShowNum(88, 2, timing.time_timing.second, 2, 24, 1);
        // 显示提示
        OLED_ShowString(0, 34, "Left rotate to clear", 8, 1);
        OLED_ShowString(0, 44, "Right rotate to back", 8, 1);
        OLED_ShowString(0, 54, "Press to start/stop", 8, 1);
        break;

    case 10: // 闹钟鸣响提示界面
        OLED_ShowString(16, 2, "Alarming", 16, 1);
        OLED_ShowNum(16, 20, time_now.hour, 2, 24, 1);
        OLED_ShowChar(40, 20, ':', 16, 1);
        OLED_ShowNum(52, 20, time_now.minute, 2, 24, 1);
        OLED_ShowChar(76, 20, ':', 16, 1);
        OLED_ShowNum(88, 20, time_now.second, 2, 24, 1);

        // 显示提示
        if (alarm_setting.is_alarm_delayed <= 2)
        {
            OLED_ShowString(16, 44, "Rotate to dalay", 8, 1);
        }
        OLED_ShowString(16, 54, "Press to stop", 8, 1);
        break;

    case 66: // 小游戏界面
        OLED_ShowChinese(00, 0, 0, 16, 1);
        OLED_ShowChinese(16, 0, 1, 16, 1);
        OLED_ShowChinese(32, 0, 2, 16, 1);
        OLED_ShowChinese(48, 0, 3, 16, 1);
        OLED_ShowChinese(64, 0, 4, 16, 1);
        // OLED_ShowChinese(80, 0, 5, 16, 1);
        // OLED_ShowChinese(96, 0, 6, 16, 1);
        // OLED_ShowChinese(114, 0, 7, 16, 1);
        // OLED_ShowChinese(128, 0, 8, 16, 1);

        OLED_ShowNum(16, 20, time_game.hour, 2, 24, 1);
        OLED_ShowChar(40, 20, ':', 16, 1);
        OLED_ShowNum(52, 20, time_game.minute, 2, 24, 1);
        OLED_ShowChar(76, 20, ':', 16, 1);
        OLED_ShowNum(88, 20, time_game.second, 2, 24, 1);

        if (time_game.hour == time_now.hour && time_game.minute == time_now.minute && abs(time_game.second - time_now.second) < 3)
        {
            OLED_ShowChinese(00, 44, 10, 16, 1);
            OLED_ShowChinese(16, 44, 11, 16, 1);
            OLED_ShowChinese(32, 44, 12, 16, 1);
            OLED_ShowChinese(48, 44, 13, 16, 1);
        }
        else
        {
            OLED_ShowChinese(00, 44, 15, 16, 1);
            OLED_ShowChinese(16, 44, 16, 16, 1);
            OLED_ShowChinese(32, 44, 14, 16, 1);
            OLED_ShowChinese(48, 44, 14, 16, 1);
        }
    };

    OLED_Refresh();
}