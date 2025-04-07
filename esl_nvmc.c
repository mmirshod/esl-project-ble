#include "esl_nvmc.h"

ret_code_t esl_nvmc_save_rgb(esl_nvmc_rgb_data_t *rgb_data) {

    uint32_t bits = 0;
    memcpy(&bits, &rgb_data, sizeof(rgb_data));

    nrfx_nvmc_page_erase(LAST_COLOR_PG_ADDR);

    if (nrfx_nvmc_word_writable_check(LAST_COLOR_PG_ADDR, bits)) {
        nrfx_nvmc_word_write(LAST_COLOR_PG_ADDR, bits);

        return NRF_SUCCESS;
    } else {
        return NRF_ERROR_INVALID_DATA;  // Not Writable
    }
}

static ret_code_t esl_nvmc_read(uint32_t addr, void *buffer, size_t size) {
    if (addr % sizeof(uint32_t) != 0) {
        return NRF_ERROR_INVALID_ADDR;
    }
    memcpy(buffer, (const void *)addr, size);
    return NRF_SUCCESS;
}

ret_code_t esl_nvmc_retrieve_last_rgb(esl_nvmc_rgb_data_t *rgb_data) {
    ret_code_t err_code = NRF_SUCCESS;
    
    err_code = esl_nvmc_read(LAST_COLOR_PG_ADDR, rgb_data, sizeof(*rgb_data));
    
    if (err_code != NRF_SUCCESS) {
        return err_code;
    }

    if (rgb_data->magic_number != ESL_NVMC_BYTE_VALID) {
        return NRF_ERROR_INVALID_DATA;
    } else {
        return NRF_SUCCESS;
    }
}