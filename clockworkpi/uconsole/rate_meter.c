#include "quantum.h"
#include "rate_meter.h"

void rate_meter_interrupt(rate_meter_t* rm) {
  uint16_t now = timer_read();
  if (timeout_get(rm->cutoff)) {
    rm->average_delta = CUTOFF_MS;
  } else {
    uint16_t delta = MIN(TIMER_DIFF_16(now, rm->last_time_millis), CUTOFF_MS);
    if (delta < 6) {
      delta = 6;
    }
    if (rm->average_delta == CUTOFF_MS) {
      rm->average_delta = delta;
    } else {
      // Smoother weighted average: 75% previous state, 25% new input
      rm->average_delta = (rm->average_delta * 3 + delta) / 4;
    }
  }
  rm->last_time_millis = now;
  rm->cutoff = timeout_reset();
}

void rate_meter_tick(rate_meter_t* rm, millis_t delta) {
  rm->cutoff = timeout_update(rm->cutoff, delta);
}

void rate_meter_expire(rate_meter_t* rm) {
  rm->cutoff = timeout_expire();
}

uint16_t rate_meter_delta(rate_meter_t* rm) {
  return rm->average_delta;
}

float rate_meter_rate(rate_meter_t* rm) {
  if (timeout_get(rm->cutoff)) {
    return 0.0f;
  } else if (rm->average_delta == 0) {
    return 1000.0f;
  } else {
    uint16_t time_since_interrupt = TIMER_DIFF_16(timer_read(), rm->last_time_millis);
    uint16_t effective_delta = MAX(rm->average_delta, time_since_interrupt);
    return 1000.0f / (float)effective_delta;
  }
}