#include "pointing_device.h"
#include "quantum.h"
#include "rate_meter.h"
#include "glider.h"
#include "trackball.h"
#include <math.h>

#define TB_LEFT  PAL_LINE(GPIOC, 11U)
#define TB_RIGHT PAL_LINE(GPIOC, 9U)
#define TB_UP    PAL_LINE(GPIOC, 8U)
#define TB_DOWN  PAL_LINE(GPIOC, 10U)

#define TB_DECR -1
#define TB_INCR 1

enum { AXIS_X = 0, AXIS_Y, AXIS_NUM };
enum { MODE_WHEEL, MODE_MOUSE };

static uint8_t last_mode = MODE_MOUSE;
static uint16_t last_report = 0;
volatile bool select_button_pressed = false; // toggled from keymap

static int8_t distances[AXIS_NUM] = {0};
static rate_meter_t rate_meters[AXIS_NUM] = {0};
static glider_t gliders[AXIS_NUM] = {0};

static const int8_t WHEEL_DENOM = 2; // Decrease this (e.g., to 1) for faster scrolling
static int8_t wheel_buffer[AXIS_NUM] = {0};

// Natural Acceleration Curve: High precision at low speeds, power curve at high speeds
static float rateToVelocityCurve(float input) {
    float abs_input = fabsf(input);
    if (abs_input < 0.05f) return 0;

    float x = abs_input - 0.05f;
    // Polynomial acceleration (x^1.5) for natural feel over long distances
    float accel = powf(x, 1.5f) / 40.0f;
    float linear = x / 20.0f;

    return 0.1f + linear + accel; 
}

static void trackball_move(uint8_t axis, int8_t direction) {
  // Always update distances[], regardless of the mode
  distances[axis] += direction;

  // Only run mouse movement-specific updates in MODE_MOUSE
  if (last_mode != MODE_WHEEL) {
    rate_meter_interrupt(&rate_meters[axis]);
    glider_set_direction(&gliders[axis], direction);

    const float rx = rate_meter_rate(&rate_meters[AXIS_X]);
    const float ry = rate_meter_rate(&rate_meters[AXIS_Y]);

    const float rate = sqrtf(rx * rx + ry * ry);
    const float ratio = (rate > 0) ? (rateToVelocityCurve(rate) / rate) : 0;

    const float vx = rx * ratio;
    const float vy = ry * ratio;

    if (axis == AXIS_X) {
      glider_update(&gliders[AXIS_X], vx, sqrtf(rate_meter_delta(&rate_meters[AXIS_X])));
      glider_update_speed(&gliders[AXIS_Y], vy);
    } else {
      glider_update_speed(&gliders[AXIS_X], vx);
      glider_update(&gliders[AXIS_Y], vy, sqrtf(rate_meter_delta(&rate_meters[AXIS_Y])));
    }
  }
}

static void trackball_left(void* arg) { (void)arg; trackball_move(AXIS_X, TB_DECR); }
static void trackball_right(void* arg) { (void)arg; trackball_move(AXIS_X, TB_INCR); }
static void trackball_up(void* arg) { (void)arg; trackball_move(AXIS_Y, TB_DECR); }
static void trackball_down(void* arg) { (void)arg; trackball_move(AXIS_Y, TB_INCR); }

bool pointing_device_driver_init(void) {
    palSetLineMode(TB_LEFT, PAL_MODE_INPUT_PULLUP);
    palSetLineMode(TB_RIGHT, PAL_MODE_INPUT_PULLUP);
    palSetLineMode(TB_UP, PAL_MODE_INPUT_PULLUP);
    palSetLineMode(TB_DOWN, PAL_MODE_INPUT_PULLUP);

    palEnableLineEvent(TB_LEFT, PAL_EVENT_MODE_BOTH_EDGES);
    palEnableLineEvent(TB_RIGHT, PAL_EVENT_MODE_BOTH_EDGES);
    palEnableLineEvent(TB_UP, PAL_EVENT_MODE_BOTH_EDGES);
    palEnableLineEvent(TB_DOWN, PAL_EVENT_MODE_BOTH_EDGES);

    palSetLineCallback(TB_LEFT, trackball_left, NULL);
    palSetLineCallback(TB_RIGHT, trackball_right, NULL);
    palSetLineCallback(TB_UP, trackball_up, NULL);
    palSetLineCallback(TB_DOWN, trackball_down, NULL);
    return true;
}

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
  int8_t x = 0, y = 0, h = 0, v = 0;
  chSysLock();

  const uint16_t now = timer_read();
  const uint16_t delta = TIMER_DIFF_16(now, last_report);
  last_report = now;

  const uint8_t mode = select_button_pressed ? MODE_WHEEL : MODE_MOUSE;
  if (last_mode != mode) {
    rate_meter_expire(&rate_meters[AXIS_X]);
    rate_meter_expire(&rate_meters[AXIS_Y]);
    glider_stop(&gliders[AXIS_X]);
    glider_stop(&gliders[AXIS_Y]);
    wheel_buffer[AXIS_X] = 0;
    wheel_buffer[AXIS_Y] = 0;
    distances[AXIS_X] = 0;
    distances[AXIS_Y] = 0;
  } else {
    rate_meter_tick(&rate_meters[AXIS_X], delta);
    rate_meter_tick(&rate_meters[AXIS_Y], delta);
  }
  last_mode = mode;

  switch(mode){
    case MODE_MOUSE:
      x = glider_glide(&gliders[AXIS_X], (uint8_t)delta);
      y = glider_glide(&gliders[AXIS_Y], (uint8_t)delta);
      distances[AXIS_X] = 0;
      distances[AXIS_Y] = 0;
      break;
    case MODE_WHEEL:
      // Accumulate trackball movement into wheel buffer to handle fractional scrolls
      wheel_buffer[AXIS_X] += distances[AXIS_X];
      wheel_buffer[AXIS_Y] += distances[AXIS_Y];
      
      // Calculate scroll amount from accumulated buffer
      h = wheel_buffer[AXIS_X] / WHEEL_DENOM;
      v = wheel_buffer[AXIS_Y] / WHEEL_DENOM;
      
      // Keep remainder in buffer for next report
      wheel_buffer[AXIS_X] -= h * WHEEL_DENOM;
      wheel_buffer[AXIS_Y] -= v * WHEEL_DENOM;
      
      // Clear raw distances after moving to buffer
      distances[AXIS_X] = 0;
      distances[AXIS_Y] = 0;
      break;
  }
  chSysUnlock();

  mouse_report.x = x;
  mouse_report.y = y;
  mouse_report.h = h;
  mouse_report.v = -v; // Inverted for natural scroll direction
  return mouse_report;
}

uint16_t pointing_device_driver_get_cpi(void) { return 0; }
void pointing_device_driver_set_cpi(uint16_t cpi) { (void)cpi; }

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    return process_record_user(keycode, record);
}