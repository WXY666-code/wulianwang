/**
 * @file app_weather.c
 * @brief Weather information module implementation
 *
 * This source file provides the implementation of weather functionality including
 * weather data fetching, display, and voice query capabilities.
 *
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 *
 */

#include "app_weather.h"
#include "app_display.h"
#include "tal_api.h"
#include "cJSON.h"
#include "lang_config.h"
#include <string.h>
#include <stdio.h>

/***********************************************************
************************macro define************************
***********************************************************/
#define DEFAULT_CITY "Beijing"
#define WEATHER_CITY_NAME_MAX_LEN 32
#define WEATHER_DESC_MAX_LEN 64

// Weather keywords for voice query
#define WEATHER_KEYWORDS "天气"
#define TEMPERATURE_KEYWORDS "温度"
#define HUMIDITY_KEYWORDS "湿度"

/***********************************************************
***********************variable define**********************
***********************************************************/
static APP_WEATHER_T sg_weather = {0};

// Weather icons mapping
static const char* sg_weather_icons[] __attribute__((unused)) = {
    "☀️",  // WEATHER_CLEAR
    "☁️",  // WEATHER_CLOUDY
    "🌧️",  // WEATHER_RAINY
    "❄️",  // WEATHER_SNOWY
    "🌫️",  // WEATHER_FOGGY
    "⛈️",  // WEATHER_THUNDER
    "💨",  // WEATHER_WINDY
    "☀️",  // WEATHER_MAX (fallback to clear)
};

/***********************************************************
***********************function define**********************
***********************************************************/

// Forward declarations
static OPERATE_RET __app_weather_parse_json(const char *json_data, WEATHER_DATA_T *weather);
static OPERATE_RET __app_weather_get_mock_data(const char *city, char *response, uint32_t max_len);

static void __app_weather_update_timer_cb(TIMER_ID timer_id, void *arg)
{
    app_weather_update();
}

static OPERATE_RET __app_weather_parse_json(const char *json_data, WEATHER_DATA_T *weather)
{
    if (!json_data || !weather) {
        return OPRT_INVALID_PARM;
    }

    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        PR_ERR("Failed to parse weather JSON");
        return OPRT_COM_ERROR;
    }

    // Parse current condition
    cJSON *current_condition = cJSON_GetObjectItem(root, "current_condition");
    if (current_condition) {
        cJSON *temp_c = cJSON_GetObjectItem(current_condition, "temp_C");
        cJSON *humidity = cJSON_GetObjectItem(current_condition, "humidity");
        cJSON *weather_desc = cJSON_GetObjectItem(current_condition, "weatherDesc");
        cJSON *windspeed_kmph = cJSON_GetObjectItem(current_condition, "windspeedKmph");

        if (temp_c) {
            weather->temperature = temp_c->valueint;
        }
        if (humidity) {
            weather->humidity = humidity->valueint;
        }
        if (windspeed_kmph) {
            weather->wind_speed = windspeed_kmph->valueint;
        }
        if (weather_desc && weather_desc->valuestring) {
            strncpy(weather->description, weather_desc->valuestring, WEATHER_DESC_MAX_LEN - 1);
            weather->description[WEATHER_DESC_MAX_LEN - 1] = '\0';
        }
    }

    // Parse location
    cJSON *nearest_area = cJSON_GetObjectItem(root, "nearest_area");
    if (nearest_area) {
        cJSON *area_name = cJSON_GetObjectItem(nearest_area, "areaName");
        if (area_name && area_name->valuestring) {
            strncpy(weather->city, area_name->valuestring, WEATHER_CITY_NAME_MAX_LEN - 1);
            weather->city[WEATHER_CITY_NAME_MAX_LEN - 1] = '\0';
        }
    }

    // Determine weather condition based on description
    if (strstr(weather->description, "rain") || strstr(weather->description, "雨")) {
        weather->condition = WEATHER_RAINY;
    } else if (strstr(weather->description, "snow") || strstr(weather->description, "雪")) {
        weather->condition = WEATHER_SNOWY;
    } else if (strstr(weather->description, "cloud") || strstr(weather->description, "云")) {
        weather->condition = WEATHER_CLOUDY;
    } else if (strstr(weather->description, "fog") || strstr(weather->description, "雾")) {
        weather->condition = WEATHER_FOGGY;
    } else if (strstr(weather->description, "thunder") || strstr(weather->description, "雷")) {
        weather->condition = WEATHER_THUNDER;
    } else if (strstr(weather->description, "wind") || strstr(weather->description, "风")) {
        weather->condition = WEATHER_WINDY;
    } else {
        weather->condition = WEATHER_CLEAR;
    }

    weather->update_time = tal_time_get_posix();
    weather->is_valid = 1;

    cJSON_Delete(root);
    return OPRT_OK;
}

static OPERATE_RET __app_weather_get_mock_data(const char *city, char *response, uint32_t max_len)
{
    // Generate mock weather data
    snprintf(response, max_len, 
        "{"
        "\"current_condition\":{"
        "\"temp_C\":\"25\","
        "\"humidity\":\"65\","
        "\"weatherDesc\":\"Partly cloudy\","
        "\"windspeedKmph\":\"12\""
        "},"
        "\"nearest_area\":{"
        "\"areaName\":\"%s\""
        "}"
        "}", city);
    
    return OPRT_OK;
}

OPERATE_RET app_weather_init(void)
{
#if !defined(ENABLE_WEATHER_FEATURE) || (ENABLE_WEATHER_FEATURE != 1)
    return OPRT_OK;
#endif

    if (sg_weather.is_initialized) {
        return OPRT_OK;
    }

    // Initialize default city
    strncpy(sg_weather.city_name, DEFAULT_CITY, WEATHER_CITY_NAME_MAX_LEN - 1);
    sg_weather.city_name[WEATHER_CITY_NAME_MAX_LEN - 1] = '\0';
    
    // Initialize weather data
    memset(&sg_weather.current_weather, 0, sizeof(WEATHER_DATA_T));
    sg_weather.current_weather.is_valid = 0;

    // Create update timer
    tal_sw_timer_create(__app_weather_update_timer_cb, NULL, &sg_weather.update_timer);
    tal_sw_timer_start(sg_weather.update_timer, WEATHER_UPDATE_INTERVAL, TAL_TIMER_CYCLE);

    sg_weather.is_initialized = 1;
    PR_INFO("Weather module initialized (mock data mode)");

    // Initial weather update
    app_weather_update();

    return OPRT_OK;
}

OPERATE_RET app_weather_get_data(WEATHER_DATA_T *weather_data)
{
#if !defined(ENABLE_WEATHER_FEATURE) || (ENABLE_WEATHER_FEATURE != 1)
    return OPRT_INVALID_PARM;
#endif

    if (!weather_data) {
        return OPRT_INVALID_PARM;
    }

    if (!sg_weather.is_initialized) {
        return OPRT_INVALID_PARM;
    }

    memcpy(weather_data, &sg_weather.current_weather, sizeof(WEATHER_DATA_T));
    return OPRT_OK;
}

OPERATE_RET app_weather_update(void)
{
#if !defined(ENABLE_WEATHER_FEATURE) || (ENABLE_WEATHER_FEATURE != 1)
    return OPRT_INVALID_PARM;
#endif

    if (!sg_weather.is_initialized) {
        return OPRT_INVALID_PARM;
    }

    char response[2048] = {0};
    OPERATE_RET ret = __app_weather_get_mock_data(sg_weather.city_name, response, sizeof(response) - 1);
    if (ret != OPRT_OK) {
        PR_ERR("Failed to generate mock weather data");
        return ret;
    }

    ret = __app_weather_parse_json(response, &sg_weather.current_weather);
    if (ret == OPRT_OK) {
        PR_INFO("Mock weather updated: %s, %d°C, %s", 
                sg_weather.current_weather.city,
                sg_weather.current_weather.temperature,
                sg_weather.current_weather.description);

        // Send weather info to display
#if defined(ENABLE_CHAT_DISPLAY) && (ENABLE_CHAT_DISPLAY == 1)
        char weather_str[128] = {0};
        snprintf(weather_str, sizeof(weather_str), "%s %d°C", 
                app_weather_get_icon(sg_weather.current_weather.condition),
                sg_weather.current_weather.temperature);
        app_display_send_msg(TY_DISPLAY_TP_STATUS, (uint8_t *)weather_str, strlen(weather_str));
#endif
    }

    return ret;
}

OPERATE_RET app_weather_handle_voice_query(const char *query_text, uint32_t query_len)
{
#if !defined(ENABLE_WEATHER_FEATURE) || (ENABLE_WEATHER_FEATURE != 1)
    return OPRT_INVALID_PARM;
#endif

    if (!query_text || query_len == 0) {
        return OPRT_INVALID_PARM;
    }

    PR_INFO("Weather voice query: %.*s", query_len, query_text);

    // Check if query contains weather keywords
    if (strstr(query_text, WEATHER_KEYWORDS)) {
        PR_INFO("Weather keyword detected, processing weather query");
        
        WEATHER_DATA_T weather;
        OPERATE_RET ret = app_weather_get_data(&weather);
        if (ret == OPRT_OK && weather.is_valid) {
            char response[256] = {0};
            snprintf(response, sizeof(response), 
                    "当前%s的天气是%s，温度%d度，湿度%d%%，风速%d公里每小时",
                    weather.city, weather.description, weather.temperature, 
                    weather.humidity, weather.wind_speed);
            
            PR_INFO("Weather response: %s", response);
            
            // Send response to display
#if defined(ENABLE_CHAT_DISPLAY) && (ENABLE_CHAT_DISPLAY == 1)
            app_display_send_msg(TY_DISPLAY_TP_ASSISTANT_MSG, (uint8_t *)response, strlen(response));
#endif
            return OPRT_OK;
        } else {
            PR_WARN("Failed to get weather data: ret=%d, valid=%d", ret, weather.is_valid);
        }
    } else {
        PR_DEBUG("No weather keyword found in query");
    }

    return OPRT_INVALID_PARM;
}

const char* app_weather_get_icon(WEATHER_CONDITION_E condition)
{
    if (condition >= WEATHER_MAX) {
        return sg_weather_icons[WEATHER_CLEAR];
    }
    return sg_weather_icons[condition];
} 