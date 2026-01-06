#pragma once

typedef struct {
  int8_t direction;
  float speed;
  uint16_t sustain;
  uint16_t release;
  float error;
  int8_t value;
} glider_t;

void glider_set_direction(glider_t*, int8_t);
void glider_update(glider_t*, float velocity, uint16_t sustain);
void glider_update_speed(glider_t*, float velocity);
void glider_stop(glider_t*);
int8_t glider_glide(glider_t*, uint8_t delta);
