#ifndef ESL_SERVICE_H__
#define ESL_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"

// Workshop 10 --> Generate random BLE UUID (Version 4 UUID)
#define ESL_BASE_UUID  { 0x1E, 0xBF, 0xA6, 0x93, 0x32, 0x18, 0x45, 0x7B, 0x85, 0x03, 0x55, 0xAC, 0xDC, 0x9B, 0x25, 0xF9 }

// Workshop 10 --> Pick a random service 16-bit UUID and define it
#define ESL_SERVICE_UUID 0xFAFA
// Workshop 11 --> Pick a characteristic UUID and define it:
#define ESL_GATT_CHAR_1_UUID 0x0001

typedef struct
{
    uint16_t service_handle;
    // Workshop 10 -->  Add handles for characterstic (type: ble_gatts_char_handles_t)
    uint16_t connection_handle;
    ble_gatts_char_handles_t char_1_handle;
} esl_ble_service_t;

ret_code_t esl_ble_service_init(esl_ble_service_t *service);

#endif /* ESL_SERVICE_H__ */