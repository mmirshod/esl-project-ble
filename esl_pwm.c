#include "esl_pwm.h"

void esl_pwm_init(esl_pwm_context_t *ctx) {
    static const nrfx_pwm_t pwm0_instance = NRFX_PWM_INSTANCE(0);
    nrfx_pwm_config_t pwm_config = NRFX_PWM_DEFAULT_CONFIG;
    
    pwm_config.output_pins[1] = ESL_IO_PIN_LED_R;
    pwm_config.output_pins[2] = ESL_IO_PIN_LED_G;
    pwm_config.output_pins[3] = ESL_IO_PIN_LED_B;

    pwm_config.top_value = PWM_TOP_VAL;
    pwm_config.step_mode = NRF_PWM_STEP_AUTO;
    pwm_config.load_mode = NRF_PWM_LOAD_INDIVIDUAL;
    pwm_config.base_clock = NRF_PWM_CLK_500kHz;

    nrfx_pwm_init(&pwm0_instance, &pwm_config, NULL);
    ctx->pwm_instance = &pwm0_instance;    
    
    ctx->pwm_seq_values = (nrf_pwm_values_individual_t){0};
    ctx->pwm_sequence = (nrf_pwm_sequence_t){
        .values.p_individual = &ctx->pwm_seq_values,
        .length = NRF_PWM_VALUES_LENGTH(ctx->pwm_seq_values),
        .repeats = 0,
        .end_delay = 0
    };

    ctx->rgb_state.red = 123;
    ctx->rgb_state.green = 25;
    ctx->rgb_state.blue = 87;
    ctx->rgb_state.is_on = 0;
}

void esl_pwm_update_duty_cycle(esl_pwm_context_t *ctx, esl_io_pin_t out_pin, uint8_t val) {
    switch (out_pin) {
        case ESL_IO_PIN_LED_R:
            ctx->pwm_seq_values.channel_1 = val > PWM_TOP_VAL ? PWM_TOP_VAL : val;
            break;
        case ESL_IO_PIN_LED_G:
            ctx->pwm_seq_values.channel_2 = val > PWM_TOP_VAL ? PWM_TOP_VAL : val;
            break;
        case ESL_IO_PIN_LED_B:
            ctx->pwm_seq_values.channel_3 = val > PWM_TOP_VAL ? PWM_TOP_VAL : val;
            break;
        default:
            break;
    }
}

void esl_pwm_update_rgb(esl_pwm_context_t *ctx) {
    if (ctx->rgb_state.is_on == 1) {
        esl_pwm_update_duty_cycle(ctx, ESL_IO_PIN_LED_R, ctx->rgb_state.red);
        esl_pwm_update_duty_cycle(ctx, ESL_IO_PIN_LED_G, ctx->rgb_state.green);
        esl_pwm_update_duty_cycle(ctx, ESL_IO_PIN_LED_B, ctx->rgb_state.blue);
    } else {
        esl_pwm_update_duty_cycle(ctx, ESL_IO_PIN_LED_R, 0);
        esl_pwm_update_duty_cycle(ctx, ESL_IO_PIN_LED_G, 0);
        esl_pwm_update_duty_cycle(ctx, ESL_IO_PIN_LED_B, 0);
    }
}


void esl_pwm_play_seq(esl_pwm_context_t *ctx) {
    nrfx_pwm_simple_playback(ctx->pwm_instance, &ctx->pwm_sequence, 1, NRFX_PWM_FLAG_LOOP);
}