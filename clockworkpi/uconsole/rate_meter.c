#include "quantum.h"
#include "rate_meter.h"

void rate_meter_interrupt(rate_meter_t* rm) {
  uint16_t now = timer_read();
  if (timeout_get(rm->cutoff)) {
    rm->average_delta = CUTOFF_MS;
  } else {
    uint16_t delta = MIN(TIMER_DIFF_16(now, rm->last_time_millis), CUTOFF_MS);
    // Smoother weighted average: 75% previous state, 25% new input
    rm->average_delta = (rm->average_delta * 3 + delta) / 4;
  }
  rm->last_time_millis = now;
  rm->cutoff = timeout_reset();
}

void rate_meter_tick(rate_meter_t* rm, unsigned long delta) {
  rm->cutoff = timeout_update(rm->cutoff, delta);
  if (!timeout_get(rm->cutoff)) {
    // Gradually increase average_delta when no movement is detected to slow down glide
    rm->average_delta += delta;
  }
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
    return 1000.0f / (float)rm->average_delta;
  }
}