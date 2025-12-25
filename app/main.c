#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "esp_at.h"
#include <stdbool.h>
#include "jsondata.h"
int main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组
    cpu_tick_init();  // 初始化系统滴答定时器
    uart_init(115200);	//串口初始化波特率为115200


    esp32_at_demo();//ESP32 AT 模块功能演示

    while(1)
    {
        delay_ms(100);
    }
}









