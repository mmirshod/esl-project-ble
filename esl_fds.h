#ifndef ESL_FDS_H__
#define ESL_FDS_H__

#include "fds.h"
#include "stdint.h"
#include "string.h"

#ifndef LED_STATE_FILE_ID
#define LED_STATE_FILE_ID               0x1111
#endif

#ifndef LED_STATE_RECORD_KEY
#define LED_STATE_RECORD_KEY            0x2222
#endif


ret_code_t esl_fds_write(void *p_data, uint32_t len);
ret_code_t esl_fds_update(fds_record_desc_t *record_desc, void *p_data, uint32_t len);
ret_code_t esl_fds_read(fds_record_desc_t *record_desc, void *buffer, uint32_t len);

#endif // ESL_FDS_H__