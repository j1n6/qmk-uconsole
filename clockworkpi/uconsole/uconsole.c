#include "quantum.h"

// Helper to safely clear the backup register
void clear_bootloader_flag(void) {
    // 1. Enable power and backup interface clocks
    RCC->APB1ENR |= (RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN);

    // 2. Enable access to the backup registers
    PWR->CR |= PWR_CR_DBP;

    // 3. Clear the magic value in BKP_DR10
    BKP->DR10 = 0;

    // 4. Disable backup domain access again
    PWR->CR &= ~PWR_CR_DBP;
}

void keyboard_pre_init_kb(void) {
    clear_bootloader_flag();
    keyboard_pre_init_user();
}

void mcu_reset(void) {
    clear_bootloader_flag();
    NVIC_SystemReset();
}

void bootloader_jump(void) {
    clear_bootloader_flag();
    NVIC_SystemReset();
}

#ifdef BACKLIGHT_ENABLE
#    include <hal.h>
#    define USER_PWM_MODE PWM_OUTPUT_ACTIVE_HIGH

static PWMConfig pwmCFG = {
    .frequency = 10000000, // 10MHz counter frequency
    .period    = 2000,     // 2000 ticks period -> 5kHz PWM frequency (200us)
    .callback  = NULL,
    .channels  = {
        {USER_PWM_MODE, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL}
    },
    .cr2       = 0,
#    if defined(STM32_PWM_USE_ADVANCED) && STM32_PWM_USE_ADVANCED
    .bdtr = 0,
#    endif
    .dier = 0
};

void backlight_init_ports(void) {
    palSetPadMode(GPIOA, 8, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
    pwmStart(&PWMD1, &pwmCFG);
}

void backlight_set(uint8_t level) {
    if (level == 0) {
        pwmDisableChannel(&PWMD1, 0);
    } else if (level == 1) {
        // Step 1: 500/2000 = 25% duty cycle
        pwmEnableChannel(&PWMD1, 0, 500); 
    } else {
        // Step 2: 2000/2000 = 100% duty cycle
        pwmEnableChannel(&PWMD1, 0, 2000);
    }
}

void backlight_task(void) {}
#endif
