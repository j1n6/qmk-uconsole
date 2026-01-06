#pragma once

#define CUTOFF_MS 1000

typedef uint16_t timeout_t ;
typedef uint8_t millis_t;

timeout_t timeout_update(timeout_t, millis_t);
bool timeout_get(timeout_t); 
timeout_t timeout_expire(void);
timeout_t timeout_reset(void);
