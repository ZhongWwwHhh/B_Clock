#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "main.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

void bluetooth_send(const uint8_t *data, const uint8_t len);
int8_t bluetooth_match_num(uint8_t space, uint8_t num1, uint8_t num2, int8_t limit_min, int8_t limit_max);

#endif