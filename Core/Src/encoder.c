
#include "main.h"
#include "encoder.h"
#include "struct_define.h"

extern Encoder_State encoder_state;
extern TIM_HandleTypeDef htim3;

// 初始化记录变量
uint16_t encoder_old_value = 0, encoder_new_value = 0;
int32_t encoder_diff = 0;

void encoder_get_state(void)
{
    encoder_new_value = __HAL_TIM_GET_COUNTER(&htim3);

    encoder_diff += encoder_new_value - encoder_old_value;

    if (encoder_diff == 0)
    {
        // 没有转动
        ;
    }
    else
    {
        // 有转动，先判定是否经过0
        if (encoder_diff > 10000)
        {
            // 处理经过零的部分
            encoder_diff = 65536 - encoder_diff;
        }

        // 处理转了几格
        if (encoder_diff > 0)
        {
            // 正转
            encoder_state.Left = encoder_diff / 4;
            encoder_diff = encoder_diff % 4;
        }
        else
        {
            // 反转
            encoder_state.Right = -encoder_diff / 4;
            encoder_diff = encoder_diff % 4;
        }

        // 更改encoder_old值
        encoder_old_value = encoder_new_value;
    }
}