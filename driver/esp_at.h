#ifndef _ESP_AT_H_
#define _ESP_AT_H_
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void esp_at_clear_buffer(void);
void esp_at_usart_init(void);
bool send_at_command(char* cmd, char* expected_res, uint32_t timeout);
bool esp_at_wait_response(const char* expected,uint16_t timeout);



#endif /* _ESP_AT_H_ */