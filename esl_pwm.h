#ifndef ESL_PWM_H__
#define ESL_PWM_H__

#include "nrfx_pwm.h"
#include "esl_gpio.h"
#include <stdint.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t is_on;
} esl_pwm_rgb_t;

typedef struct {
    const nrfx_pwm_t * pwm_instance;
    nrf_pwm_values_individual_t pwm_seq_values;
    nrf_pwm_sequence_t pwm_sequence;
    esl_pwm_rgb_t rgb_state;
} esl_pwm_context_t;


void esl_pwm_init(esl_pwm_context_t *ctx);
void esl_pwm_update_duty_cycle(esl_pwm_context_t *ctx, esl_io_pin_t out_pin, uint8_t val);
void esl_pwm_update_rgb(esl_pwm_context_t *ctx);
void esl_pwm_play_seq(esl_pwm_context_t *ctx);

#endif // ESL_PWM_H__