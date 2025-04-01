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
    uint8_t props,
    void *p_val,
    uint16_t val_len
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
        .init_len = val_len,
        .max_len = val_len,
        .p_value = (uint8_t*)p_val
    };

    ret_code = sd_ble_gatts_characteristic_add(service->service_handle,
                                               &char_md,
                                               &attr_char_value, 
                                               char_handle);    
    APP_ERROR_CHECK(ret_code);

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
    ble_gatts_char_handles_t *char_handle,
    uint8_t *p_data
) {

    uint16_t len = sizeof(p_data);
    ble_gatts_hvx_params_t hvx_params = {
        .handle = char_handle->value_handle,
        .offset = 0,
        .p_len = &len,
        .p_data = p_data,
        .type = BLE_GATT_HVX_NOTIFICATION
    };

    ret_code_t err_code = sd_ble_gatts_hvx(service->connection_handle, &hvx_params);
    return err_code;
}