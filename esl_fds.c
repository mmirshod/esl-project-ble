#include "esl_fds.h"

static fds_record_t prepare_record(void *p_data, uint32_t len) {
    fds_record_t record = {
        .key = LED_STATE_RECORD_KEY,
        .file_id = LED_STATE_FILE_ID,
        .data = {
            .length_words = 1,
            .p_data = p_data
        }
    };
    return record;
}

ret_code_t esl_fds_write(void *p_data, uint32_t len) {
    fds_record_t record = prepare_record(p_data, len);

    return fds_record_write(NULL, &record);
}

ret_code_t esl_fds_update(fds_record_desc_t *record_desc, void *p_data, uint32_t len) {
    fds_record_t record = prepare_record(p_data, len);

    return fds_record_update(record_desc, &record);
}

ret_code_t esl_fds_read(fds_record_desc_t *record_desc, void *buffer, uint32_t len) {
    fds_flash_record_t flash_record;

    ret_code_t err_code = fds_record_open(record_desc, &flash_record);
    if (err_code != NRF_SUCCESS) {
        return err_code;
    }

    memcpy(buffer, flash_record.p_data, len);
    err_code = fds_record_close(record_desc);
    return err_code;
}