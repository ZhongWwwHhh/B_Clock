#include "error.h"

void error_show()
{
    OLED_Clear();
    OLED_ShowString(5, 5, "ERROR", 24, 1);
    OLED_ShowString(5, 30, "PLEASE RESTART", 24, 1);
    OLED_Refresh();
    return;
}