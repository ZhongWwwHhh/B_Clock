
#include "bluetooth.h"

extern UART_HandleTypeDef huart1;

// 向蓝牙模块发送数据
void bluetooth_send(const uint8_t *data, const uint8_t len)
{
    HAL_UART_Transmit_DMA(&huart1, data, len);
}

int8_t bluetooth_match_num(uint8_t space, uint8_t num1, uint8_t num2, int8_t limit_min, int8_t limit_max)
{
    if (space == ' ' && isdigit(num1) && isdigit(num2))
    {
        uint8_t str[2];
        str[0] = num1;
        str[1] = num2;
        int8_t num = atoi(str);
        if (num >= limit_min && num <= limit_max)
        {
            return num;
        }
    }
    return -1;
}