#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "main.h"
#include <stdbool.h>
#include <regex.h>

void bluetooth_send(uint8_t *data, uint8_t len);
bool bluetooth_matchRegex(const uint8_t *pattern, const uint8_t *userString);

#endif