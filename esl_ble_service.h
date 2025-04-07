#ifndef ESL_SERVICE_H__
#define ESL_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"

#define ESL_BASE_UUID                           { 0x1E, 0xBF, 0xA6, 0x00, 0x00, 0x18, 0x45, 0x7B, 0x85, 0x03, 0x55, 0xAC, 0xFA, 0xFA, 0x25, 0xF9 }
#define ESL_SERVICE_UUID                        0xFAFA
#define ESL_BLE_CHAR_LED_STATE_UUID             0x0001
#define ESL_BLE_CHAR_RGB_R_STATE_UUID           0x0002
#define ESL_BLE_CHAR_RGB_G_STATE_UUID           0x0003
#define ESL_BLE_CHAR_RGB_B_STATE_UUID           0x0004

#define ESL_BLE_CHAR_PROPS_READ                 (1 << 0)    // 0b00000001
#define ESL_BLE_CHAR_PROPS_WRITE                (1 << 1)    // 0b00000010
#define ESL_BLE_CHAR_PROPS_NOTIFY               (1 << 2)    // 0b00000100
#define ESL_BLE_CHAR_PROPS_INDICATE             (1 << 3)    // 0b00001000

typedef struct
{
    ble_uuid_t uuid;
    uint16_t service_handle;
    uint16_t connection_handle;
    ble_gatts_char_handles_t char_led_state_handle;
    ble_gatts_char_handles_t char_rgb_r_state_handle;
    ble_gatts_char_handles_t char_rgb_g_state_handle;
    ble_gatts_char_handles_t char_rgb_b_state_handle;
} esl_ble_service_t;

ret_code_t esl_ble_service_init(esl_ble_service_t *service);

ret_code_t esl_ble_add_char(
    esl_ble_service_t *service,
    uint16_t char_uuid,
    ble_gatts_char_handles_t *char_handle,
    uint8_t *p_data
);

ret_code_t esl_ble_service_notify(
    esl_ble_service_t *service,
    ble_gatts_char_handles_t *char_handle 
);

ret_code_t esl_ble_char_value_update(
    uint16_t conn_handle,
    uint16_t value_handle,
    uint8_t* p_data
);

/**
 * @param conn_handle -- 16 bit connection handle
 * @param handle -- Handle for which extract value
 * @param p_type -- Pointer to variable to store the value
 */
ret_code_t esl_ble_char_handle_value_get(
    uint16_t conn_handle,
    uint16_t handle,
    uint8_t* p_val
);
#endif /* ESL_SERVICE_H__ */