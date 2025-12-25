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
    delay_ms(100);    // 等待系统稳定
    uart_init(115200);	//串口初始化波特率为115200
    esp_at_usart_init();//初始化ESP-AT模块所使用的串口USART2
    
    power_on_location_get();//上电获取IP及地理位置，初始化结构体IP_Location_t
    
    Print_IP_Info();
    while(1)
    {
        delay_ms(100);
    }
}









