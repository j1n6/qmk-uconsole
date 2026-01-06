#pragma once

#undef HAL_USE_PWM
#define HAL_USE_PWM TRUE

#undef HAL_USE_PAL
#define HAL_USE_PAL TRUE

#undef PAL_USE_CALLBACKS
#define PAL_USE_CALLBACKS TRUE

#include_next <halconf.h>
