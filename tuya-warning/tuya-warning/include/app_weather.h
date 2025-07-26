/**
 * @file app_weather.h
 * @brief Weather information module for Tuya IoT device
 *
 * This header file provides the declarations for weather functionality including
 * weather data fetching, display, and voice query capabilities.
 *
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 *
 */

 #ifndef __APP_WEATHER_H__
 #define __APP_WEATHER_H__
 
 #include "tuya_cloud_types.h"
#include "tal_api.h"
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /***********************************************************
 ************************macro define************************
 ***********************************************************/
 #define WEATHER_UPDATE_INTERVAL (60 * 60 * 1000)  // 1 hour
 #define WEATHER_CITY_NAME_MAX_LEN 32
 #define WEATHER_DESC_MAX_LEN 64
 
 // Weather condition types
 typedef enum {
     WEATHER_CLEAR = 0,
     WEATHER_CLOUDY,
     WEATHER_RAINY,
     WEATHER_SNOWY,
     WEATHER_FOGGY,
     WEATHER_THUNDER,
     WEATHER_WINDY,
     WEATHER_MAX
 } WEATHER_CONDITION_E;
 
 // Weather data structure
 typedef struct {
     char city[WEATHER_CITY_NAME_MAX_LEN];
     char description[WEATHER_DESC_MAX_LEN];
     int temperature;
     int humidity;
     int wind_speed;
     WEATHER_CONDITION_E condition;
     uint32_t update_time;
     uint8_t is_valid;
 } WEATHER_DATA_T;
 
 /***********************************************************
***********************typedef define***********************
***********************************************************/
typedef struct {
    WEATHER_DATA_T current_weather;
    TIMER_ID update_timer;
    char city_name[WEATHER_CITY_NAME_MAX_LEN];
    uint8_t is_initialized;
} APP_WEATHER_T;

/***********************************************************
********************function declaration********************
***********************************************************/
 
 /**
  * @brief Initialize the weather module
  *
  * @return OPERATE_RET Initialization result, OPRT_OK indicates success
  */
 OPERATE_RET app_weather_init(void);
 
 /**
  * @brief Get current weather data
  *
  * @param weather_data Pointer to store weather data
  * @return OPERATE_RET Result of getting weather data
  */
 OPERATE_RET app_weather_get_data(WEATHER_DATA_T *weather_data);
 
 /**
  * @brief Update weather data from server
  *
  * @return OPERATE_RET Result of updating weather data
  */
 OPERATE_RET app_weather_update(void);
 
 /**
  * @brief Handle weather voice query
  *
  * @param query_text Voice query text
  * @param query_len Length of query text
  * @return OPERATE_RET Result of handling query
  */
 OPERATE_RET app_weather_handle_voice_query(const char *query_text, uint32_t query_len);
 
 /**
  * @brief Get weather icon based on condition
  *
  * @param condition Weather condition
  * @return const char* Weather icon string
  */
 const char* app_weather_get_icon(WEATHER_CONDITION_E condition);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* __APP_WEATHER_H__ */