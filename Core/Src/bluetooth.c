
#include "bluetooth.h"

extern UART_HandleTypeDef huart1;

// 向蓝牙模块发送数据
void bluetooth_send(uint8_t *data, uint8_t len)
{
    for (int i; i < len; i++)
    {
        HAL_UART_Transmit(&huart1, &data[i], 1, 1000);
    }
}

bool bluetooth_matchRegex(const uint8_t *pattern, const uint8_t *userString)
{
    bool result = false;

    regex_t regex;
    int regexInit = regcomp(&regex, (char *)pattern, REG_EXTENDED);
    if (regexInit)
    {
        ; // fail
    }
    else
    {
        int reti = regexec(&regex, (char *)userString, 0, NULL, 0);
        if (reti)
        {
            // match failed
        }
        else
        {
            result = true;
        }
    }
    regfree(&regex);
    return result;
}
