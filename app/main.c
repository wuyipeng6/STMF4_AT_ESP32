#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "esp_at.h"
#include <stdbool.h>

int main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组
    cpu_tick_init();  // 初始化系统滴答定时器
    delay_ms(100);    // 等待系统稳定
    uart_init(115200);	//串口初始化波特率为115200
    esp_at_usart_init();//初始化ESP-AT模块所使用的串口USART2
    if(send_at_command("AT", "OK", 1000) == true)
    {
        printf("ESP-AT成功\r\n");
    }
    else
    {
        printf("ESP-AT失败\r\n");
    }

    //delay_ms(500);

    if (send_at_command("AT+CWMODE=1", "OK", 1000) == true) {
        printf("模式设置成功\r\n");
    } else {
        printf("模式设置超时或失败\r\n");
    }
    //delay_ms(500);
    // 连接 WiFi 的指令通常需要很长时间 (5~10秒)
    if (send_at_command("AT+CWJAP=\"whatcanisay\",\"123456qq\"", "OK", 10000) == true) {
        printf("WiFi 已连接\r\n");
    }
    else {
        printf("WiFi 连接超时或失败\r\n");
    }
    while(1)
    {
        delay_ms(100);
    }
}









