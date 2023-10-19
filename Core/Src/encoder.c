
#include "main.h"
#include "encoder.h"
#include "struct_define.h"

extern Encoder_State encoder_state;
extern TIM_HandleTypeDef htim3;

uint16_t encoder_old_value = 0, encoder_new_value = 0;

void encoder_get_state(void)
{
    encoder_new_value = __HAL_TIM_GET_COUNTER(&htim3);
}