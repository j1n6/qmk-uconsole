#include "timeout.h"

timeout_t timeout_update(timeout_t t, millis_t delta) {
  if (t > delta) {
    return t - delta;
  } else {
    return 0;
  }
}

timeout_t timeout_expire(void) {
  return 0;
}

bool timeout_get(timeout_t t) {
  return t == 0;
} 

timeout_t timeout_reset(void) {
  return CUTOFF_MS;
}
