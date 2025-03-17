#ifndef ESL_SERVICE_H__
#define ESL_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"

#define ESL_BASE_UUID                           { 0x1E, 0xBF, 0xA6, 0x00, 0x00, 0x18, 0x45, 0x7B, 0x85, 0x03, 0x55, 0xAC, 0xFA, 0xFA, 0x25, 0xF9 }
#define ESL_SERVICE_UUID                        0xFAFA
#define ESL_GATT_CHAR_1_UUID                    0x0001
#define ESL_NOTIFICATIONS_CHAR_UUID             0x0002
#define ESL_INDICATIONS_CHAR_UUID               0x0003
#define ESL_BLE_CHAR_DESC_MAX_LEN               24

#define ESL_BLE_CHAR_PROPS_READ                  (1 << 0)    // 0b00000001
#define ESL_BLE_CHAR_PROPS_WRITE                 (1 << 1)    // 0b00000010
#define ESL_BLE_CHAR_PROPS_NOTIFY                (1 << 2)    // 0b00000100
#define ESL_BLE_CHAR_PROPS_INDICATE              (1 << 3)    // 0b00001000

typedef struct
{
    ble_uuid_t uuid;
    uint16_t service_handle;
    // Workshop 10 -->  Add handles for characterstic (type: ble_gatts_char_handles_t)
    uint16_t connection_handle;
    ble_gatts_char_handles_t char_1_handle;
    ble_gatts_char_handles_t char_2_handle;
    ble_gatts_char_handles_t char_3_handle;
} esl_ble_service_t;

ret_code_t esl_ble_service_init(esl_ble_service_t *service);


#endif /* ESL_SERVICE_H__ */