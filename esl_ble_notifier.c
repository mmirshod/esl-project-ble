#include "ble.h"

#include "esl_ble_service.h"
#include "esl_ble_notifier.h"

extern esl_ble_service_t m_esl_service;

APP_TIMER_DEF(notifier_timer);

static void handle_notifier_timeout(void *p_context) {
    if (m_esl_service.connection_handle != BLE_CONN_HANDLE_INVALID) {
        uint8_t type;
        ble_gatts_value_t cccd = {
            .p_value = &type,
            .len = sizeof(type),
            .offset = 0
        };
        
    }
}