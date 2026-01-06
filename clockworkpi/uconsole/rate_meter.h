#include "timeout.h"

typedef struct {
  uint16_t last_time_millis;
  uint16_t average_delta;
  timeout_t cutoff;
} rate_meter_t;

void rate_meter_interrupt(rate_meter_t* rm);
void rate_meter_tick(rate_meter_t* rm, unsigned long delta);
void rate_meter_expire(rate_meter_t* rm);
uint16_t rate_meter_delta(rate_meter_t* rm);
float rate_meter_rate(rate_meter_t* rm);
