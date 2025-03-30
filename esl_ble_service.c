#include "esl_ble_service.h"

#include "app_error.h"
#include "nrf_log.h"

#include "ble_gatts.h"
#include "ble_srv_common.h"

extern uint8_t global_counter;

static ret_code_t esl_ble_base_uuid_add(esl_ble_service_t* service) {
    ret_code_t ret_code = NRF_SUCCESS;
    
    ble_uuid128_t base_uuid = {
        .uuid128 = ESL_BASE_UUID
    };

    ble_uuid_t service_uuid;
    service_uuid.uuid = ESL_SERVICE_UUID;
    
    ret_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(ret_code);

    service->uuid = service_uuid;

    UNUSED_VARIABLE(ret_code);

    return ret_code;
}

static ret_code_t esl_ble_add_char(
    esl_ble_service_t *service,
    uint16_t char_uuid,
    ble_gatts_char_handles_t *char_handle,
    uint8_t props,
    uint16_t max_len
) {
    ret_code_t ret_code = NRF_SUCCESS;

    // Characteristics Meta Data
    ble_gatts_char_md_t char_md = { 0 };
    char_md.char_props.read  = (props & ESL_BLE_CHAR_PROPS_READ) ? 1 : 0;
    char_md.char_props.write = (props & ESL_BLE_CHAR_PROPS_WRITE) ? 1 : 0;
    char_md.char_props.notify = (props & ESL_BLE_CHAR_PROPS_NOTIFY) ? 1 : 0;
    char_md.char_props.indicate = (props & ESL_BLE_CHAR_PROPS_INDICATE) ? 1 : 0;

    if (char_md.char_props.notify || char_md.char_props.indicate)
    {
        ble_gatts_attr_md_t cccd_md = {
            .vloc = BLE_GATTS_VLOC_STACK
        };
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
        char_md.p_cccd_md = &cccd_md;
    }

    // Attribute meta data
    ble_gatts_attr_md_t attr_md = {
        .vloc = BLE_GATTS_VLOC_STACK
    };

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);

    if (char_md.char_props.write) {
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    }

    ble_uuid_t char_ble_uuid;
    char_ble_uuid.uuid = char_uuid;
    char_ble_uuid.type = service->uuid.type; 

    ble_gatts_attr_t attr_char_value = { 
        .p_uuid = &char_ble_uuid,
        .p_attr_md = &attr_md,
        .init_offs = 0,
        .init_len = 0,
        .max_len = max_len
    };

    // if (char_md.char_props.notify || char_md.char_props.indicate) {
    //     attr_char_value.init_len = sizeof(notifier_val);
    //     attr_char_value.max_len = sizeof(notifier_val);
    // }

    ret_code = sd_ble_gatts_characteristic_add(service->service_handle,
                                               &char_md,
                                               &attr_char_value, 
                                               char_handle);
    
    APP_ERROR_CHECK(ret_code);

    return ret_code;
}

static ret_code_t esl_ble_add_char_indicator(esl_ble_service_t *service) {
    ret_code_t ret = NRF_SUCCESS;

    ble_uuid_t indicator_char_uuid = {
        .uuid = ESL_INDICATIONS_CHAR_UUID,
        .type = service->uuid.type
    };

    ble_gatts_attr_md_t attr_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);

    ble_gatts_attr_md_t cccd_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    ble_gatts_attr_t indicator_attr_val = {
        .p_uuid = &indicator_char_uuid,
        .p_attr_md = &attr_md,
        .init_len = sizeof(global_counter),
        .max_len = sizeof(global_counter),
        .init_offs = 0,
        .p_value = &global_counter
    };

    ble_gatts_char_md_t indicator_char_md = {
        .char_props.indicate = 1,
        .p_cccd_md = &cccd_md,
    };

    ret = sd_ble_gatts_characteristic_add(service->service_handle,
                                          &indicator_char_md,
                                          &indicator_attr_val,
                                          &service->char_3_handle);
    return ret;
}

static ret_code_t esl_ble_add_char_notifier(esl_ble_service_t *service) {
    ret_code_t ret = NRF_SUCCESS;

    ble_uuid_t notifier_char_uuid = {
        .uuid = ESL_NOTIFICATIONS_CHAR_UUID,
        .type = service->uuid.type
    };

    ble_gatts_attr_md_t attr_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_md_t cccd_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    ble_gatts_attr_t notifier_attr_val = {
        .p_uuid = &notifier_char_uuid,
        .p_attr_md = &attr_md,
        .init_len = sizeof(global_counter),
        .max_len = sizeof(global_counter),
        .init_offs = 0,
        .p_value = &global_counter
    };

    ble_gatts_char_md_t notifier_char_md = {
        .char_props.notify = 1,
        .char_props.write = 1,
        .p_cccd_md = &cccd_md,
    };

    ret = sd_ble_gatts_characteristic_add(service->service_handle,
                                          &notifier_char_md,
                                          &notifier_attr_val,
                                          &service->char_2_handle);

    return ret;
}

ret_code_t esl_ble_service_init(esl_ble_service_t *service)
{
    ret_code_t ret_code = NRF_SUCCESS;
    ret_code = esl_ble_base_uuid_add(service);

    ret_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service->uuid,
                                        &service->service_handle);
    APP_ERROR_CHECK(ret_code);

    ret_code = esl_ble_add_char(service, ESL_GATT_CHAR_1_UUID, &service->char_1_handle, 
                                ESL_BLE_CHAR_PROPS_READ | ESL_BLE_CHAR_PROPS_WRITE, 5);
    APP_ERROR_CHECK(ret_code);

    ret_code = esl_ble_add_char_notifier(service);
    APP_ERROR_CHECK(ret_code);

    ret_code = esl_ble_add_char_indicator(service);
    APP_ERROR_CHECK(ret_code);

    UNUSED_VARIABLE(ret_code);

    return ret_code;
}