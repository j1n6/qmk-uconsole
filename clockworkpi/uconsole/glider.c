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
  gr->release = sustain;
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

  gr->error += gr->speed * delta;
  int8_t distance = 0;
  if (gr->error >= 127.0) {
    distance = 127;
  } else if (gr->error > 0) {
    distance = ceilf(gr->error);
  }
  gr->error -= distance;

  if (gr->sustain > 0) {
    uint16_t sustained = MIN(gr->sustain, (uint16_t)delta);
    gr->sustain -= sustained;
  } else if (gr->release > 0) {
    uint16_t released = MIN(gr->release, (uint16_t)delta);
    gr->speed = gr->speed * (gr->release - released) / gr->release;
    gr->release -= released;
  } else {
    gr->speed = 0;
  }

  if (!already_stopped && gr->speed == 0) {
    glider_stop(gr);
  }

  return gr->direction * distance;
}
