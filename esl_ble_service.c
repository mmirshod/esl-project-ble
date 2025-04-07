#include "esl_ble_service.h"

#include "app_error.h"
#include "nrf_log.h"

#include "ble_gatts.h"
#include "ble_srv_common.h"

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

ret_code_t esl_ble_add_char(
    esl_ble_service_t *service,
    uint16_t char_uuid,
    ble_gatts_char_handles_t *char_handle,
    uint8_t *p_data
) {
    ret_code_t ret_code = NRF_SUCCESS;

    ble_gatts_char_md_t char_md = { 
        .char_props = {
            .read = 1,
            .write = 1,
            .notify = 1,
        }
    };

    ble_gatts_attr_md_t cccd_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    char_md.p_cccd_md = &cccd_md;

    ble_gatts_attr_md_t attr_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_uuid_t char_ble_uuid = {
        .uuid = char_uuid,
        .type = service->uuid.type
    };

    ble_gatts_attr_t attr_val = {
        .init_len = 1,
        .max_len = 1,
        .init_offs = 0,
        .p_attr_md = &attr_md,
        .p_uuid = &char_ble_uuid,
        .p_value = p_data
    };

    ret_code = sd_ble_gatts_characteristic_add(
        service->service_handle,
        &char_md,
        &attr_val,
        char_handle
    );

    return ret_code;
}

ret_code_t esl_ble_service_init(esl_ble_service_t *service)
{
    ret_code_t ret_code = NRF_SUCCESS;
    ret_code = esl_ble_base_uuid_add(service);

    ret_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service->uuid,
                                        &service->service_handle);
    APP_ERROR_CHECK(ret_code);

    UNUSED_VARIABLE(ret_code);

    return ret_code;
}

ret_code_t esl_ble_service_notify(
    esl_ble_service_t *service,
    ble_gatts_char_handles_t *char_handle
) {
    ret_code_t err_code;

    uint8_t data;
    err_code = esl_ble_char_handle_value_get(service->connection_handle, char_handle->value_handle, &data);
    APP_ERROR_CHECK(err_code);

    uint16_t len = sizeof(data);
    ble_gatts_hvx_params_t hvx_params = {
        .handle = char_handle->value_handle,
        .offset = 0,
        .p_len = &len,
        .p_data = &data,
        .type = BLE_GATT_HVX_NOTIFICATION
    };

    err_code = sd_ble_gatts_hvx(service->connection_handle, &hvx_params);
    return err_code;
}

ret_code_t esl_ble_char_value_update(
    uint16_t conn_handle,
    uint16_t value_handle,
    uint8_t* p_data
) {
    uint16_t len_data = sizeof(*p_data);
    ble_gatts_value_t val_data = {
        .len = len_data,
        .offset = 0,
        .p_value = p_data
    };

    return sd_ble_gatts_value_set(
        conn_handle,
        value_handle,
        &val_data
    );
}

ret_code_t esl_ble_char_handle_value_get(
    uint16_t conn_handle,
    uint16_t handle,
    uint8_t* p_val
) {
    ble_gatts_value_t gatts_val = {
        .p_value = p_val,
        .offset = 0,
        .len = sizeof(*p_val)
    };

    return sd_ble_gatts_value_get(
        conn_handle,
        handle,
        &gatts_val
    );
}
