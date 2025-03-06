#include "esl_ble_service.h"

#include "app_error.h"
#include "nrf_log.h"

#include "ble.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"

static ret_code_t esl_ble_add_char(esl_ble_service_t *service);

ret_code_t esl_ble_service_init(esl_ble_service_t *service)
{
    ret_code_t ret_code = NRF_SUCCESS;

    ble_uuid_t service_uuid;
    ble_uuid128_t base_uuid = {
        .uuid128 = ESL_BASE_UUID
    };
    service_uuid.uuid = ESL_SERVICE_UUID;

    ret_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(ret_code);

    ret_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &service->service_handle);
    APP_ERROR_CHECK(ret_code);

    UNUSED_VARIABLE(ret_code);

    return esl_ble_add_char(service);
}

static ret_code_t esl_ble_add_char(esl_ble_service_t *service)
{
    ret_code_t ret_code = NRF_SUCCESS;

    ble_uuid_t char_uuid;
    ble_uuid128_t base_uuid = {
        .uuid128 = ESL_BASE_UUID
    };

    char_uuid.uuid = ESL_GATT_CHAR_1_UUID;

    ret_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(ret_code);

    ble_gatts_char_md_t char_md = { 0 };
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;

    // Configures attribute metadata. For now we only specify that the attribute will be stored in the softdevice
    ble_gatts_attr_md_t attr_md = { 0 };
    attr_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    ble_gatts_attr_t attr_char_value = { 0 };
    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;

    attr_char_value.max_len = 5;
    attr_char_value.init_len = 0;
    attr_char_value.init_offs = 0;

    ret_code = sd_ble_gatts_characteristic_add(service->service_handle,
                                               &char_md,
                                               &attr_char_value, 
                                               &service->char_1_handle);
    APP_ERROR_CHECK(ret_code);

    return NRF_SUCCESS;
}