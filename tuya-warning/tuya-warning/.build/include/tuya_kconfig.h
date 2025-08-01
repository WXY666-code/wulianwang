/* AUTO-GENERATED FILE. DO NOT MODIFY !!!
*
* This config file is automatically generated by build system.
* It should not be modified by hand.
*/

#ifndef OPENSDK_CONFIG_H
#define OPENSDK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define OPEN_VERSION "v1.4.0"
#define OPEN_COMMIT "15c2ceb336d48f0ba46a12586830c904f9154ce0"
#define PLATFORM_NAME "T5AI"
#define PLATFORM_COMMIT "4a2afa985a8e6caa1db2ae9559353defd3c1c52d"
#define PLATFORM_CHIP "T5AI"
#define PLATFORM_BOARD "TUYA_T5AI_BOARD"

//
// configure project
//
#define PROJECT_NAME "your_chat_bot"
#define PROJECT_VERSION "1.0.1"
#define PROJECT_FRAMEWORK_BASE 1
// CONFIG_PROJECT_FRAMEWORK_ARDUINO is not set
#define FRAMEWORK_CHOICE "base"
// end of configure project

//
// configure app (your_chat_bot)
//
#define TUYA_PRODUCT_KEY "alon7qgyjj8yus74"
// CONFIG_ENABLE_CHAT_MODE_KEY_PRESS_HOLD_SINGEL is not set
// CONFIG_ENABLE_CHAT_MODE_KEY_TRIG_VAD_FREE is not set
// CONFIG_ENABLE_CHAT_MODE_ASR_WAKEUP_SINGEL is not set
#define ENABLE_CHAT_MODE_ASR_WAKEUP_FREE 1
#define ENABLE_WAKEUP_KEYWORD_NIHAO_TUYA 1
// CONFIG_ENABLE_WAKEUP_KEYWORD_NIHAO_XIAOZHI is not set
// CONFIG_ENABLE_WAKEUP_KEYWORD_XIAOZHI_TONGXUE is not set
// CONFIG_ENABLE_WAKEUP_KEYWORD_XIAOZHI_GUANJIA is not set
#define ENABLE_CHAT_DISPLAY 1
#define ENABLE_GUI_WECHAT 1
// CONFIG_ENABLE_GUI_CHATBOT is not set
// CONFIG_ENABLE_GUI_OLED is not set
// CONFIG_ENABLE_GUI_EYES is not set
#define ENABLE_GUI_STREAM_AI_TEXT 1
// end of configure app (your_chat_bot)

#define BOARD_ENABLE_UBUNTU 1
#define BOARD_ENABLE_T2 1
#define BOARD_ENABLE_T3 1
#define BOARD_ENABLE_T5AI 1
#define BOARD_ENABLE_ESP32 1
#define BOARD_ENABLE_LN882H 1
#define BOARD_ENABLE_BK7231X 1
// CONFIG_BOARD_CHOICE_UBUNTU is not set
#define PLATFORM_CHOICE "T5AI"
#define OPERATING_SYSTEM 98
#define ENABLE_WIFI 1
#define ENABLE_BLUETOOTH 1
#define LITTLE_END 1
#define MBEDTLS_CONFIG_FILE "tuya_tls_config.h"
// CONFIG_BOARD_CHOICE_T2 is not set
#define ENABLE_ADC 1
#define ENABLE_GPIO 1
#define ENABLE_PWM 1
#define ENABLE_I2C 1
#define ENABLE_SPI 1
#define ENABLE_TIMER 1
#define ENABLE_WATCHDOG 1
#define BOARD_CHOICE "TUYA_T5AI_BOARD"
// CONFIG_BOARD_CHOICE_T3 is not set
#define ENABLE_HCI 1
#define ENABLE_RTC 1
#define ENABLE_UART 1
#define ENABLE_FLASH 1
#define TUYA_FLASH_TYPE_MAX_PARTITION_NUM 10
#define ENABLE_PM 1
#define ENABLE_WAKEUP 1
#define ENABLE_PINMUX 1
#define CHIP_CHOICE "T5AI"
#define BOARD_CONFIG 1
#define BOARD_CHOICE_T5AI 1
#define PLATFORM_T5 1
#define ENABLE_EXT_RAM 1
#define ENABLE_QSPI 1
#define ENABLE_MCU8080 1
#define ENABLE_MEDIA 1
#define ENABLE_RGB 1
#define ENABLE_PIN2FUNC 1
#define ENABLE_T5AI_OS_SERVICE 1
#define BOARD_CHOICE_TUYA_T5AI_BOARD 1
// CONFIG_TUYA_T5AI_BOARD_EX_MODULE_NONE is not set
#define TUYA_T5AI_BOARD_EX_MODULE_35565LCD 1
// CONFIG_TUYA_T5AI_BOARD_EX_MODULE_EYES is not set
// CONFIG_TUYA_T5AI_BOARD_EX_MODULE_29E_INK is not set
// CONFIG_BOARD_CHOICE_TUYA_T5AI_EVB is not set
// CONFIG_BOARD_CHOICE_TUYA_T5AI_MINI is not set
// CONFIG_BOARD_CHOICE_T5AI_MOJI_1_28 is not set
// CONFIG_BOARD_CHOICE_ESP32 is not set
// CONFIG_BOARD_CHOICE_LN882H is not set
// CONFIG_BOARD_CHOICE_BK7231X is not set

//
// configure tuyaopen
//

//
// configure tuya cloud service
//
#define TUYA_SECURITY_LEVEL 1
#define ENABLE_BT_SERVICE 1
#define ENABLE_BT_NETCFG 1
#define ENABLE_BT_CTRL 1
#define BT_ADV_INTERVAL_MIN 30
#define BT_ADV_INTERVAL_MAX 60
#define BT_REMOTE_CTRL_SCAN_INTERVAL 30
#define BT_REMOTE_CTRL_SCAN_WINDOWS 10
#define ENABLE_NIMBLE 1
#define TUYA_HS_BLE_HOST_TASK_PRIORITY 9
#define TUYA_HS_BLE_HOST_STACK_SIZE 5120
// end of configure tuya cloud service

//
// AI Protocol Config
//
#define AI_MAX_FRAGMENT_LENGTH 8192
// CONFIG_ENABLE_AI_PROTO_DEBUG is not set
#define AI_BIZ_TASK_DELAY 10
#define AI_SESSION_MAX_NUM 2
#define AI_MAX_SESSION_ID_NUM 5
#define AI_MAX_ATTR_NUM 10
#define AI_PACKET_SECURITY_LEVEL 4
#define AI_CLIENT_STACK_SIZE 4096
#define AI_READ_SOCKET_BUF_SIZE 0
#define AI_WRITE_SOCKET_BUF_SIZE 8192
// CONFIG_AI_HEAP_IN_PSRAM is not set
// CONFIG_AI_STACK_IN_PSRAM is not set
// end of AI Protocol Config

//
// configure enable/disable liblwip
//
#define ENABLE_LIBLWIP 1
#define LWIP_TCP_HEAD_ALLOC_REUSE_POLL 0
#define LWIP_TUYA_APP_TCPIP_THREAD_SWITCH 1
#define IPEF_TEST_OPTIMIZE 0
#define MEM_SIZE 51200
#define MEMP_NUM_UDP_PCB 10
#define MEMP_NUM_TCP_PCB 10
#define MEMP_NUM_TCP_PCB_LISTEN 5
#define MEMP_NUM_NETCONN 21
#define MEMP_NUM_PBUF 100
#define MEMP_NUM_TCP_SEG 80
#define IP_REASS_MAX_PBUFS 10
#define PBUF_POOL_SIZE 20
#define PBUF_LINK_ENCAPSULATION_HLEN 96
#define TCP_SND_BUF 32768
#define TCP_SND_QUEUELEN 44
#define MEMP_NUM_NETBUF 32
#define TCPIP_MBOX_SIZE 6
#define DEFAULT_UDP_RECVMBOX_SIZE 24
#define DEFAULT_TCP_RECVMBOX_SIZE 6
#define DEFAULT_RAW_RECVMBOX_SIZE 6
#define DEFAULT_ACCEPTMBOX_SIZE 6
#define MEMP_NUM_SYS_TIMEOUT 12
#define LWIP_SO_SNDTIMEO 1
#define LWIP_SO_RCVTIMEO 1
#define LWIP_SO_LINGER 1
#define LWIP_TUYA_SO_LINGER_RST 1
#define LWIP_PROVIDE_ERRNO 1
#define LWIP_IPV6 0
#define LWIP_NETIF_HOSTNAME 0
#define LWIP_TCPIP_CORE_LOCKING 0
#define LWIP_EAPOL_SUPPORT 0
#define TCPIP_THREAD_STACKSIZE 4096
#define TCPIP_THREAD_PRIO 9
#define DHCPC_THREAD_STACKSIZE 2048
#define DHCPC_THREAD_PRIO 5
#define LWIP_TX_PBUF_ZERO_COPY 0
#define CONFIG_TUYA_SOCK_SHIM 0
#define SOCK_API_SYNC 0
#define LWIP_DHCPC_STATIC_IPADDR_ENABLE 1
#define LWIP_NETCONN_SEM_PER_THREAD 1
#define ETHARP_SUPPORT_STATIC_ENTRIES 1
#define LWIP_NETIF_STATUS_CALLBACK 1
#define LWIP_COMPAT_MUTEX_ALLOWED 1
#define LWIP_TIMEVAL_PRIVATE 0
#define IN_ADDR_T_DEFINED 1
#define LWIP_NETCONN_FULLDUPLEX 1
// CONFIG_MEM_LIBC_MALLOC is not set
// CONFIG_LWIP_DUAL_NET_SUPPORT is not set
#define ENABLE_LWIP_DHCP 1
// end of configure enable/disable liblwip

//
// configure mbedtls
//
// CONFIG_ENABLE_MBEDTLS_DEBUG is not set
// CONFIG_ENABLE_CUSTOM_CONFIG is not set
// end of configure mbedtls

//
// configure system parameter
//
#define STACK_SIZE_TIMERQ 4096
#define STACK_SIZE_WORK_QUEUE 5120
#define MAX_NODE_NUM_WORK_QUEUE 100
#define STACK_SIZE_MSG_QUEUE 4096
#define MAX_NODE_NUM_MSG_QUEUE 100
// end of configure system parameter

#define ENABLE_LIBLVGL 1
#define LVGL_ENABLE_TOUCH 1
// CONFIG_ENABLE_LVGL_DEMO is not set
// CONFIG_LVGL_ENABLE_ENCODER is not set
// CONFIG_LVGL_COLOR_16_SWAP is not set
#define LV_DRAW_BUF_PROPORTION_10 1
// CONFIG_LV_DRAW_BUF_PROPORTION_5 is not set
#define LV_DRAW_BUF_PARTS 10

//
// configure device driver
//
#define ENABLE_BUTTON 1
#define BUTTON_NUM 1
#define BUTTON_NAME "button"
#define ENABLE_LED 1
#define LED_NUM 1
#define LED_NAME "led"
#define ENABLE_AUDIO_CODECS 1
#define AUDIO_CODECS_NUM 1
#define AUDIO_CODEC_NAME "audio_codec"
// CONFIG_ENABLE_AUDIO_AEC is not set
#define ENABLE_DISPLAY 1
#define DISPLAY_NUM 1
#define DISPLAY_NAME "display"
#define ENABLE_TOUCH 1
// CONFIG_ENABLE_ENCODER is not set
// end of configure device driver
// end of configure tuyaopen


#ifdef __cplusplus
}
#endif

#endif  // OPENSDK_CONFIG_H


