#ifndef ESL_NVMC_H__
#define ESL_NVMC_H__

#include "nrfx_nvmc.h"
#include "app_error.h"
#include "stdint.h"
#include <string.h>

#define ESL_NVMC_BYTE_VALID         (0xA5)
#define ESL_NVMC_BYTE_NOT_INIT      (0xFF)
#define BOOTLOADER_START_ADDR       (0x000E0000)
#define PAGE_SIZE                   (0x1000)
#define APP_DATA_END_ADDR           BOOTLOADER_START_ADDR
#define APP_DATA_START_ADDR         BOOTLOADER_START_ADDR - 3 * PAGE_SIZE
#define LAST_COLOR_PG_ADDR          APP_DATA_START_ADDR

typedef struct {
    uint8_t magic_number;
    uint8_t r_val;
    uint8_t g_val;
    uint8_t b_val;
} esl_nvmc_rgb_data_t;

/**
 * @brief Function to save given rgb color
 * @param[in] rgb_data - Pointer to the rgb data to be written from
 * 
 * @retval NRF_SUCCESS - RGB Data saved successfully
 * @retval NRF_ERROR_INVALID_DATA - given data is not writable
 */
ret_code_t esl_nvmc_save_rgb(esl_nvmc_rgb_data_t *rgb_data);

/**
 * @brief Function to save given rgb color
 * @param[out] rgb_data - Pointer to the rgb data to be retrieved to
 * 
 * @retval NRF_SUCCESS - RGB Data retrieved successfully
 * @retval NRF_ERROR_INVALID_ADDR - Invalid address alignment
 * @retval NRF_ERROR_INVALID_DATA - RGB data is corrupted
 */
ret_code_t esl_nvmc_retrieve_last_rgb(esl_nvmc_rgb_data_t *rgb_data);


#endif // ESL_NVMC_H__