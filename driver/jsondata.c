#include "jsondata.h"
#include <string.h>
#include <stdio.h>



// 声明全局变量
IP_Location_t g_ip_info = {0};



/**
 * @brief 从 JSON 字符串中提取指定键的字符串值
 * @param json 原始 JSON 字符串
 * @param key  要查找的键名 (例如 "city")
 * @param result 存放结果的缓冲区
 * @param max_len 缓冲区最大长度
 * @return int 成功返回 0，失败返回 -1
 */
int get_json_value(const char *json, const char *key, char *result, int max_len) {
    char key_pattern[32];
    // 构造查找模式，例如: "city":"
    snprintf(key_pattern, sizeof(key_pattern), "\"%s\":\"", key);
    
    char *p_start = strstr(json, key_pattern);
    if (p_start == NULL) {
        return -1; // 没找到键名
    }
    
    // 指针移动到 " 之后（即值的起始位置）
    p_start += strlen(key_pattern);
    
    // 找到下一个双引号（即值的结束位置）
    char *p_end = strchr(p_start, '\"');
    if (p_end == NULL) {
        return -1; // 格式错误
    }
    
    // 计算值的长度
    int value_len = p_end - p_start;
    
    // 防止缓冲区溢出
    if (value_len >= max_len) {
        value_len = max_len - 1;
    }
    
    // 拷贝结果并添加结束符
    memcpy(result, p_start, value_len);
    result[value_len] = '\0';
    
    return 0;
}


/**
 * @brief 解析IP的原始数据并更新全局变量
 * @param raw_buffer 串口收到的原始字符串 (如 +HTTPCLIENT:...)
 * @return int 解析成功返回 1，失败返回 0
 */
int Parse_IP_JSON_To_Global(const char *raw_buffer) {
    // 1. 数据预处理：定位到 JSON 的起始括号 '{'
    const char *json_body = strchr(raw_buffer, '{');
    if (json_body == NULL) {
        g_ip_info.is_valid = 0;
        return 0; // 没有找到 JSON 数据
    }

    // 2. 清空全局变量
    memset(&g_ip_info, 0, sizeof(IP_Location_t));

    // 3. 依次提取各个字段
    // 注意：我们将 JSON 中的 "query" 提取并存入结构体的 "ip" 字段中
    int res = 0;
    res += get_json_value(json_body, "country",    g_ip_info.country,    32);
    res += get_json_value(json_body, "regionName", g_ip_info.regionName, 32);
    res += get_json_value(json_body, "city",       g_ip_info.city,       32);
    res += get_json_value(json_body, "query",      g_ip_info.ip,         32);

    // 4. 判断提取是否成功
    // 如果返回值为 0，说明至少成功提取了部分关键信息
    if (res <= 0 && strlen(g_ip_info.ip) > 0) {
        g_ip_info.is_valid = 1;
    } else {
        g_ip_info.is_valid = 0;
    }
    return g_ip_info.is_valid;
}


/**
 * @brief 打印全局变量中存储的 IP 地理位置信息
 */
void Print_IP_Info(void) {
    printf("\r\n========== 当前地理位置信息 ==========\n");
    
    if (g_ip_info.is_valid) {
        printf("公网 IP:   %s\n", g_ip_info.ip);
        printf("国家:      %s\n", g_ip_info.country);
        printf("省份:      %s\n", g_ip_info.regionName);
        printf("城市:      %s\n", g_ip_info.city);
    } else {
        printf("地理位置信息无效或未获取到数据\n"); 
    }
    printf("======================================\n\r\n");
}
