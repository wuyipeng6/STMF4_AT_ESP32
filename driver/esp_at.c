#include "esp_at.h"
#include "delay.h"
#include "usart2_dma_at.h"
//初始化ESP-AT模块所使用的串口USART2
void esp_at_usart_init(void)//static函数等于文件的私有函数
{
    USART2_DMA_Config();//初始化USART2+DMA接收
}

//清空ESP-AT模块接收缓冲区
void esp_at_clear_buffer(void) {
    rx_flag = 0;
    rx_len = 0;
    memset(MAIN_RX_BUF, 0, UART_RX_BUF_SIZE);
}



/**
 * @brief  等待 ESP32 AT 指令的特定应答
 * @param  expected: 期待的关键字 (如 "OK", "READY", "CONNECT",保险一点可以加\r\n)
 * @param  timeout_ms: 最大等待时间 (毫秒)
 * @return true: 成功匹配到关键字; false: 超时未匹配或收到 ERROR
 */
bool esp_at_wait_response(const char* expected, uint16_t timeout_ms) {
    // 1. 获取起始时间 (使用你提供的 cpu_now() 封装的 ms 函数)
    uint64_t start_time = cpu_get_ms();

    while (cpu_get_ms() - start_time < (uint64_t)timeout_ms) {

        // 2. Check if IDLE interrupt indicates new data packet
        if (rx_flag) {
            //printf("Data: %s\r\n", (char*)MAIN_RX_BUF);
            // 3. 在转存的缓冲区中查找期待的字符串
            if (strstr((char*)MAIN_RX_BUF, expected) != NULL) {
                rx_flag = 0;  // 找到匹配后才清除标志
                return true;  // 找到了，立即返回
            }

            // 4. 可选：如果收到 "ERROR"，可以提前结束等待以节省时间
            if (strstr((char*)MAIN_RX_BUF, "ERROR") != NULL) {
                rx_flag = 0;  // 收到错误后才清除标志
                // 注意：有些指令失败是正常的，根据你的逻辑决定是否直接 return false
                return false; 
            }
            
            // 都不匹配，清除标志，继续等待下一包数据
            // 注意：不在这里 memset MAIN_RX_BUF，
            // 因为有些长响应（如扫描 WiFi）是分多次 IDLE 产生的。
            rx_flag = 0;
        }
        
        // 给 CPU 留一点点喘息时间（可选，视具体主频而定）
        // 如果主频很高，可以在这里加个微秒级小延时
    }

    return false; // 到时间了还没匹配到
}



bool send_at_command(char* cmd, char* expected_res, uint32_t timeout) {
    // 1. 发送前先清理现场
    esp_at_clear_buffer();
    
    // 2. 通过串口发送指令
    // 假设你有一个简单的串口发送字符串函数
    while (cmd && *cmd)//发送字符串，遇到字符串结束符'\0'停止
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);//等待发送区空
        USART_SendData(USART2, *cmd++);//*的优先级高于++
    }
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART_SendData(USART2, '\r');
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART_SendData(USART2, '\n');
    
    // 3. 调用等待函数
    return esp_at_wait_response(expected_res, timeout);
}

