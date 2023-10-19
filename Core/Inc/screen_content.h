#ifndef SCREEN_CONTENT_H
#define SCREEN_CONTENT_H

#include <stdint.h>
#include "oled.h"
#include "struct_define.h"
#include "LM75A.h"
#include "error.h"

void screen_show(int8_t *screen_display_num, int8_t *clean_display);

#endif