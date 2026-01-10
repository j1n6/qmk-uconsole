#include "quantum.h"
#include "glider.h"
#include <math.h>

void glider_set_direction(glider_t* gr, int8_t direction) {
  if (gr->direction != direction) {
    glider_stop(gr);
  }
  gr->direction = direction;
}

void glider_update(glider_t* gr, float speed, uint16_t sustain) {
  gr->speed = speed;
  gr->sustain = sustain;
  
  // Boost the release (coast) time for high-speed movements
  // If speed is high, the cursor 'glides' longer before stopping.
  if (speed > 2.0f) {
      gr->release = sustain + (uint16_t)(speed * 10.0f); 
  } else {
      gr->release = sustain;
  }
}

void glider_update_speed(glider_t* gr, float speed) {
  gr->speed = speed;
}

void glider_stop(glider_t* gr) {
  gr->speed = 0;
  gr->sustain = 0;
  gr->release = 0;
  gr->error = 0;
}

int8_t glider_glide(glider_t* gr, uint8_t delta) {
  bool already_stopped = gr->speed == 0;

  // Accumulate velocity into error buffer (use a local float for delta)
  float fdelta = (float)delta;
  gr->error += gr->speed * fdelta;

  int8_t distance = 0;

  // Use simple cast instead of floorf/ceilf: casting to int truncates toward zero,
  // which equals floor for positive values and ceil for negative values — matching previous logic.
  if (gr->error >= 1.0f || gr->error <= -1.0f) {
    if (gr->error > 127.0f) {
      distance = 127;
    } else if (gr->error < -127.0f) {
      distance = -127;
    } else {
      distance = (int8_t)gr->error;
    }
  }

  // Remove the integer part we are reporting, keep the remainder in gr->error
  gr->error -= (float)distance;

  // Handle sustain and release decay
  if (gr->sustain > 0) {
    uint16_t sustained = MIN(gr->sustain, (uint16_t)delta);
    gr->sustain -= sustained;
  } else if (gr->release > 0) {
    uint16_t released = MIN(gr->release, (uint16_t)delta);
    gr->speed = gr->speed * (float)(gr->release - released) / (float)gr->release;
    gr->release -= released;
  } else {
    gr->speed = 0;
  }

  if (!already_stopped && gr->speed == 0) {
    glider_stop(gr);
  }

  return gr->direction * distance;
}