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

static const int16_t WHEEL_DENOM = 24; // Finer grain control for "High Res" feel
static int16_t wheel_buffer[AXIS_NUM] = {0};

// Anti-rebound / Consistency Filter
// Low threshold to catch rebounds even on short movements
#define TB_LOCK_THRESHOLD 3
// Increased correction limit (2) to absorb double-tick noise bursts which are common
// with this sensor, ensuring smoothest possible glide.
#define TB_CORRECT_LIMIT 2

static int16_t consecutive_steps[AXIS_NUM] = {0};
static int8_t  locked_direction[AXIS_NUM] = {0};
static int8_t  correction_count[AXIS_NUM] = {0};
static uint16_t last_axis_activity[AXIS_NUM] = {0};

// Natural Acceleration Curve: High precision at low speeds, power curve at high speeds
static float rateToVelocityCurve(float input) {
    float abs_input = fabsf(input);
    if (abs_input < 0.02f) return 0; // Lower deadzone for finer control

    float x = abs_input - 0.02f;
    // Polynomial acceleration (x^1.5) for natural feel over long distances
    float accel = powf(x, 1.5f) / 40.0f;
    float linear = x / 20.0f;

    return 0.1f + linear + accel; 
}

static void trackball_move(uint8_t axis, int8_t direction) {
  // Check for idle reset
  uint16_t now = timer_read();
  if (TIMER_DIFF_16(now, last_axis_activity[axis]) > 200) {
      consecutive_steps[axis] = 0;
      locked_direction[axis] = 0;
      correction_count[axis] = 0;
  }
  last_axis_activity[axis] = now;

  // Anti-rebound Filter
  bool is_reverse = (locked_direction[axis] != 0) && (direction != locked_direction[axis]);

  // SCENARIO 1 & 3: Axis Flipping / Rebound Filtering
  // The EVQWJN007 sensor is prone to reporting reversed direction when the ball is 
  // shifted slightly (0.01mm) at the edge of a stroke or when pressure is applied.
  // This can happen on X or Y axis independently.
  //
  // STRATEGY: DROP NOISE (Do not invent data)
  // If we have established momentum (consecutive_steps >= threshold), and detect a sudden reversal,
  // we assume it is noise and DROP the packet entirely.
  // - This prevents the "Glider Stop" (Zig-Zag) because we don't send the reverse signal.
  // - This prevents "Jumping Around" because we don't substitute fake forward motion.
  // - The cursor simply "Coasts" over the noise.

  if (is_reverse) {
      if (consecutive_steps[axis] >= TB_LOCK_THRESHOLD) {
          // Dynamic Limit:
          // Low Speed: 1 tick check (Fast response for precision)
          // High Speed: 2 tick check (Suppress mechanical bounce)
          int8_t limit = (gliders[axis].speed > 1.5f) ? 2 : 1;
          
          if (correction_count[axis] < limit) {
              // IGNORE this event. Treat it as if the hardware never triggered.
              correction_count[axis]++;
              return; 
          } else {
              // Limit exceeded, accept the reversal as valid user intent
              locked_direction[axis] = direction;
              consecutive_steps[axis] = 1;
              correction_count[axis] = 0;
          }
      } else {
          // Not enough momentum to filter, accept immediately (allows micro-adjustments)
          locked_direction[axis] = direction;
          consecutive_steps[axis] = 1;
          correction_count[axis] = 0;
      }
  } else {
      // Continuing same direction
      if (direction == locked_direction[axis]) {
          if (consecutive_steps[axis] < 32000) consecutive_steps[axis]++;
          correction_count[axis] = 0;
      } else {
          // First move from rest
          locked_direction[axis] = direction;
          consecutive_steps[axis] = 1;
          correction_count[axis] = 0;
      }
  }

  // Always update distances[], regardless of the mode
  distances[axis] += direction;

  // Always run glider/rate meter updates to allow momentum in both modes
  {
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
    consecutive_steps[AXIS_X] = 0; locked_direction[AXIS_X] = 0; correction_count[AXIS_X] = 0;
    consecutive_steps[AXIS_Y] = 0; locked_direction[AXIS_Y] = 0; correction_count[AXIS_Y] = 0;
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
      // Use glider for smoothed momentum scrolling
      // Accumulate smoothed movement into wheel buffer
      // Note: We use the same gliders as mouse mode for consistent feel
      wheel_buffer[AXIS_X] += glider_glide(&gliders[AXIS_X], (uint8_t)delta);
      wheel_buffer[AXIS_Y] += glider_glide(&gliders[AXIS_Y], (uint8_t)delta);
      
      // Calculate scroll amount from accumulated buffer
      h = wheel_buffer[AXIS_X] / WHEEL_DENOM;
      v = wheel_buffer[AXIS_Y] / WHEEL_DENOM;
      
      // Keep remainder in buffer for next report
      wheel_buffer[AXIS_X] -= h * WHEEL_DENOM;
      wheel_buffer[AXIS_Y] -= v * WHEEL_DENOM;
      
      // Clear raw distances (consumed by glider logic in trackball_move/glider_glide updates)
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