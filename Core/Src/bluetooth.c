
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
