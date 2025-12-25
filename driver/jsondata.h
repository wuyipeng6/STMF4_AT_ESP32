#ifndef JSONDATA_H
#define JSONDATA_H
#include "stm32f4xx.h"

// 定义一个结构体来存储IP地理位置信息(utf-8编码)
//如果用串口输出中文，那么串口助手需要设置为utf-8编码格式
//如果要用GBK编码格式输出中文，需要在拷贝字符串后进行编码转换，比较麻烦，这里就不做了。
//如果需要在OLED等显示屏上显示中文，需要对字库进行索引，那么字库的编码格式也需要是utf-8格式。所以最好文件都用utf-8编码格式。
typedef struct {
    char country[32];    // 国家
    char regionName[32]; // 省份
    char city[32];       // 城市
    char ip[32];         // 公网IP
    int  is_valid;       // 数据是否有效 (1有效, 0无效)
} IP_Location_t;

extern IP_Location_t g_ip_info; 

int get_json_value(const char *json, const char *key, char *result, int max_len);//从 JSON 字符串中提取指定键的字符串值
int Parse_IP_JSON_To_Global(const char *json);//解析IP的JSON数据并存入结构体
void Print_IP_Info(void);//打印全局变量中存储的 IP 地理位置信息

#endif /* JSONDATA_H */
