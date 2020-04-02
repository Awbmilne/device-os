
/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2019 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "logging.h"
#include "service_debug.h"
#include "platforms.h"


#if PLATFORM_ID == PLATFORM_TRACKER
#define PORT_SPI        SPI1
#define PORT_CS_PIN     WIFI_CS
#define PORT_INT_PIN    WIFI_INT
#define PORT_EN_PIN     WIFI_EN
#define PORT_BOOT_PIN   WIFI_BOOT
#else
#define PORT_SPI        SPI
#define PORT_CS_PIN     A0
#define PORT_INT_PIN    A1
#define PORT_EN_PIN     A2
#define PORT_BOOT_PIN   A3
#endif


//typedef SemaphoreHandle_t AT_MUTEX_T;
typedef void* AT_MUTEX_T;

typedef int32_t esp_err_t;

#ifndef assert
#define assert SPARK_ASSERT
#endif

/* Definitions for error constants. */

#define ESP_OK                  0
#define ESP_FAIL                -1
#define ESP_ERR_NO_MEM          0x101
#define ESP_ERR_INVALID_ARG     0x102
#define ESP_ERR_INVALID_STATE   0x103
#define ESP_ERR_INVALID_SIZE    0x104
#define ESP_ERR_NOT_FOUND       0x105
#define ESP_ERR_NOT_SUPPORTED   0x106
#define ESP_ERR_TIMEOUT         0x107
#define ESP_ERR_INVALID_RESPONSE    0x108

#define at_debugLevel 2

#define ESP_AT_LOGE(x, ...) {if(at_debugLevel >= 0) {LOG_DEBUG_PRINTF(ERROR, "E %s: ",x); LOG_DEBUG_PRINTF(ERROR, __VA_ARGS__); LOG_DEBUG_PRINTF(ERROR, "\r\n");}}
#define ESP_AT_LOGW(x, ...) {if(at_debugLevel >= 1) {LOG_DEBUG_PRINTF(WARN, "W %s: ",x);  LOG_DEBUG_PRINTF(WARN, __VA_ARGS__);  LOG_DEBUG_PRINTF(WARN, "\r\n");}}
#define ESP_AT_LOGI(x, ...) {if(at_debugLevel >= 2) {LOG_DEBUG_PRINTF(INFO, "I %s: ",x);  LOG_DEBUG_PRINTF(INFO, __VA_ARGS__);  LOG_DEBUG_PRINTF(INFO, "\r\n");}}
#define ESP_AT_LOGD(x, ...) {if(at_debugLevel >= 3) {LOG_DEBUG_PRINTF(TRACE, "D %s: ",x); LOG_DEBUG_PRINTF(TRACE, __VA_ARGS__); LOG_DEBUG_PRINTF(TRACE, "\r\n");}}
#define ESP_AT_LOGV(x, ...) {if(at_debugLevel >= 4) {LOG_DEBUG_PRINTF(TRACE, "V %s: ",x); LOG_DEBUG_PRINTF(TRACE, __VA_ARGS__); LOG_DEBUG_PRINTF(TRACE, "\r\n");}}

// Forces data to be 4 bytes aligned
#define WORD_ALIGNED_ATTR __attribute__((aligned(4)))

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set cs line to high.
 * 
 * @return None
 */
void at_cs_high(void);

/**
 * @brief Set cs line to low.
 * 
 * @return None
 */
void at_cs_low(void);

/**
  * @brief  Delay some time.
  *
  * @param  uint32_t  Delay time in ms.
  *
  * @return None
  */
void at_do_delay(uint32_t wait_ms);

/**
 * @brief Send a SPI transaction, wait for it to complete, and return the result.
 * 
 * @param tx_buff Pointer to transmit buffer
 * @param rx_buff Pointer to receive buffer
 * @param len     Total data length, in bytes
 * 
 * @return 
 *   - ESP_OK if success
 *   - ESP_ERR_INVALID_ARG if channal not valid 
 */
esp_err_t at_spi_transmit(void* tx_buff, void* rx_buff, uint32_t len);

/**
 * @brief Initialize peripherals, include SPI and GPIO.
 * 
 * @return 
 *   - ESP_OK if success
 *   - ESP_FAIL if fail
 */
esp_err_t at_spi_slot_init(void);

esp_err_t at_spi_int_init(void);

/**
 * @brief Wait interrupt line.
 * 
 * @param wait_ms Wait time in ms.
 * 
 * @return 
 *   - ESP_OK if success
 *   - ESP_ERR_TIMEOUT if timeout 
 */
esp_err_t at_spi_wait_int(uint32_t wait_ms);

/**
 * @brief create a new mutex
 * 
 * @return 
 *   - The handle to the created mutex if the mutex type semaphore was created successfully
 *   - NULL if fail
 */
AT_MUTEX_T at_mutex_init(void);

/**
 * @brief lock a mutex
 * 
 * @param pxMutex -- the mutex to lock
 * 
 * @return None
 */
void at_mutex_lock(AT_MUTEX_T pxMutex);

/**
 * @brief unlock a mutex
 * 
 * @param pxMutex -- the mutex to unlock
 * 
 * @return None
 */
void at_mutex_unlock(AT_MUTEX_T pxMutex);

/**
 * @brief Delete a mutex
 * 
 * @param pxMutex -- the mutex to delete
 * 
 * @return None
 */
void at_mutex_free(AT_MUTEX_T pxMutex);

#ifdef __cplusplus
}
#endif
