
#include "main.h"
#include "stm32f405xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "usb_device.h"
#include "usbd_def.h"
#include "usbd_cdc_if.h"
#include "mecha_emu.h"

uint8_t SCPH_90004_NVM_bin[] = {
  0xb4, 0x00, 0x2b, 0xbc, 0xfe, 0x9f, 0x02, 0x2a, 0x14, 0xec, 0x50, 0xb0,
  0x4d, 0x7f, 0x7f, 0x4d, 0x96, 0x10, 0x50, 0xb2, 0xa0, 0x12, 0x24, 0x23,
  0x05, 0x14, 0x37, 0x23, 0x3a, 0x45, 0x30, 0x45, 0xc8, 0x25, 0x0a, 0x0a,
  0x03, 0x00, 0x20, 0x02, 0x5a, 0x5a, 0x10, 0x02, 0x02, 0x00, 0x00, 0x00,
  0x38, 0x2a, 0x50, 0xee, 0x21, 0x92, 0x21, 0x92, 0x02, 0x30, 0x30, 0x48,
  0x18, 0x30, 0x39, 0x30, 0x70, 0x08, 0x08, 0x06, 0x05, 0x05, 0x0e, 0x04,
  0x81, 0x53, 0x7f, 0x78, 0x83, 0x2e, 0x7f, 0x4f, 0x82, 0x3f, 0x7f, 0x21,
  0x05, 0x03, 0x00, 0xda, 0x36, 0x80, 0x0a, 0x02, 0x08, 0x08, 0x2a, 0x81,
  0x5d, 0x00, 0x00, 0x02, 0x14, 0x20, 0x7f, 0x02, 0xf8, 0x04, 0xd3, 0x00,
  0x7e, 0x02, 0x33, 0x6a, 0x00, 0x85, 0x88, 0x88, 0x88, 0x88, 0x88, 0x13,
  0x3c, 0x70, 0x50, 0x41, 0x96, 0x02, 0x40, 0x40, 0x63, 0x33, 0x00, 0x05,
  0x0b, 0x00, 0x2e, 0x00, 0x29, 0x06, 0x0c, 0x00, 0x01, 0x1e, 0x28, 0x00,
  0x65, 0x06, 0x06, 0x00, 0x3b, 0x07, 0x36, 0x00, 0xf8, 0x01, 0x14, 0x00,
  0x00, 0x34, 0x8f, 0x00, 0x67, 0x67, 0x3f, 0x4b, 0x3a, 0x3a, 0xfc, 0x01,
  0xfc, 0x01, 0x14, 0x14, 0x3e, 0x3a, 0xfd, 0xfd, 0x07, 0xf2, 0x07, 0xf6,
  0x06, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x0c, 0x00, 0x33, 0x4c, 0x72, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x62, 0x66, 0x82, 0x8a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x48,
  0x50, 0x68, 0x68, 0x68, 0x68, 0x68, 0x68, 0x68, 0x86, 0x86, 0x86, 0x8c,
  0x95, 0xa3, 0xb3, 0xd0, 0xe8, 0x00, 0x7f, 0x81, 0x86, 0x87, 0x89, 0x8b,
  0x8d, 0x8f, 0x93, 0xff, 0x00, 0x7a, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88,
  0x8a, 0x8c, 0x5a, 0x3f, 0x3c, 0x00, 0xff, 0xff, 0x25, 0xc4, 0x0f, 0x07,
  0x25, 0xc4, 0x24, 0x00, 0x62, 0xb2, 0xad, 0x41, 0x49, 0x09, 0x98, 0xa4,
  0x90, 0xb2, 0x7e, 0xfa, 0xc9, 0xda, 0x6e, 0x11, 0xfa, 0xb7, 0xd9, 0x67,
  0x96, 0x60, 0x7b, 0x20, 0x2b, 0x66, 0x41, 0x44, 0x55, 0x70, 0x8d, 0x63,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x70, 0x00, 0xe5, 0x9f,
  0x3c, 0x00, 0x19, 0x9f, 0x1c, 0x00, 0x86, 0xb5, 0x24, 0x00, 0xd3, 0xac,
  0x1c, 0x00, 0x95, 0x42, 0x2c, 0x00, 0x26, 0x1f, 0x25, 0x00, 0xc3, 0x34,
  0x33, 0x00, 0x83, 0x4a, 0x33, 0x00, 0x10, 0x08, 0xe9, 0xe9, 0xce, 0xce,
  0xce, 0x00, 0x18, 0x18, 0x03, 0x03, 0x2d, 0x00, 0x2b, 0x03, 0x48, 0x0b,
  0x18, 0x18, 0x20, 0x18, 0x4f, 0x4f, 0x2c, 0x00, 0xff, 0xff, 0xff, 0xff,
  0x45, 0x45, 0x65, 0x6e, 0x67, 0x45, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x1f, 0xa7, 0x8c, 0xe6, 0x95, 0x8d, 0x41, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07,
  0x53, 0x43, 0x50, 0x48, 0x2d, 0x39, 0x30, 0x30, 0x30, 0x34, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0xff, 0xff, 0xff, 0x05, 0xb0,
  0x41, 0xd1, 0x5b, 0xca, 0x46, 0x41, 0x31, 0x8c, 0xfa, 0xcc, 0x29, 0x77,
  0x0a, 0x1c, 0x7f, 0x6c, 0x31, 0x88, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x21, 0x85, 0xd4, 0x20, 0x17, 0xdf, 0xf9, 0x80, 0x00, 0xf6, 0xfc, 0xc7,
  0x50, 0x00, 0x00, 0xec, 0x84, 0xd4, 0x11, 0x01, 0xe8, 0x20, 0x06, 0x21,
  0x00, 0x66, 0x20, 0x05, 0x08, 0x08, 0x00, 0xca, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x01, 0x0a,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x53, 0x55, 0x6e, 0x65,
  0x6e, 0x65, 0x6e, 0xbd, 0x65, 0x00, 0x00, 0xcd, 0xab, 0x00, 0x00, 0x11,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf1, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x73, 0xf5, 0x0e, 0x05, 0x00, 0x00, 0x00, 0x7b,
  0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0x40, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x21, 0xc0, 0x3c,
  0x60, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xdc, 0xcd, 0x7d,
  0xd3, 0x83, 0xff, 0x90, 0x03, 0xdc, 0xcd, 0x7d, 0xd3, 0x83, 0xff, 0x90,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff
};
uint8_t SCPH_90004_NVM_bin_len = 1024;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);



void SystemClock_Config(void)
{
 RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

// FPGA interface pins
#define FPGA_CS_GPIO_Port    GPIOA
#define FPGA_CS_Pin          GPIO_PIN_4

#define FPGA_SCK_GPIO_Port   GPIOA
#define FPGA_SCK_Pin         GPIO_PIN_3

#define FPGA_MISO_GPIO_Port  GPIOA
#define FPGA_MISO_Pin        GPIO_PIN_1

#define FPGA_MOSI_GPIO_Port  GPIOA
#define FPGA_MOSI_Pin        GPIO_PIN_2

#define FPGA_IRQ_GPIO_Port   GPIOA
#define FPGA_IRQ_Pin         GPIO_PIN_0

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    /* USER CODE BEGIN MX_GPIO_Init_1 */

    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
__HAL_RCC_USB_OTG_FS_CLK_ENABLE();

GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // CS, SCK and MOSI: outputs
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = FPGA_CS_Pin;
    HAL_GPIO_Init(FPGA_CS_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = FPGA_SCK_Pin;
    HAL_GPIO_Init(FPGA_SCK_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = FPGA_MOSI_Pin;
    HAL_GPIO_Init(FPGA_MOSI_GPIO_Port, &GPIO_InitStruct);

    // MISO: input
    GPIO_InitStruct.Pin  = FPGA_MISO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(FPGA_MISO_GPIO_Port, &GPIO_InitStruct);

    // IRQ: input
    GPIO_InitStruct.Pin  = FPGA_IRQ_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(FPGA_IRQ_GPIO_Port, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    // Idle state: CS high, SCK low, MOSI low
    HAL_GPIO_WritePin(FPGA_CS_GPIO_Port, FPGA_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FPGA_SCK_GPIO_Port, FPGA_SCK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FPGA_MOSI_GPIO_Port, FPGA_MOSI_Pin, GPIO_PIN_RESET);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;  // Two-digit year: 10 means 2010
} FakeRTC;

static FakeRTC rtc = {
    .second = 0,
    .minute = 0,
    .hour   = 0,
    .day    = 1,
    .month  = 1,
    .year   = 10
};

static uint32_t rtc_last_tick;

static uint8_t to_bcd(uint8_t value)
{
    return (uint8_t)(((value / 10) << 4) | (value % 10));
}

static uint8_t days_in_month(uint8_t month, uint8_t year)
{
    static const uint8_t days[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if (month == 2 &&
        ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
        return 29;

    return days[month - 1];
}

static void fake_rtc_init(void)
{
    rtc_last_tick = HAL_GetTick();
}

static void fake_rtc_update(void)
{
    uint32_t now = HAL_GetTick();
    uint32_t elapsed = now - rtc_last_tick;

    while (elapsed >= 1000) {
        rtc_last_tick += 1000;
        elapsed -= 1000;

        if (++rtc.second >= 60) {
            rtc.second = 0;

            if (++rtc.minute >= 60) {
                rtc.minute = 0;

                if (++rtc.hour >= 24) {
                    rtc.hour = 0;

                    if (++rtc.day > days_in_month(rtc.month, rtc.year)) {
                        rtc.day = 1;

                        if (++rtc.month > 12) {
                            rtc.month = 1;
                            rtc.year = (rtc.year + 1) % 100;
                        }
                    }
                }
            }
        }
    }
}

void usb_log(const char *fmt, ...)
{
    char buffer[32];

    va_list args;
    va_start(args, fmt);

    vsnprintf(buffer, sizeof(buffer), fmt, args);

    va_end(args);
uint8_t timeOut = 255;
while (CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)) == USBD_BUSY)
    {
        HAL_Delay(1);
        if(timeOut == 0) break;
        else timeOut--;
    }}



uint8_t spi_transfer(uint8_t tx)
{
    uint8_t rx = 0;

    for (int i = 7; i >= 0; i--)
    {
        // Set MOSI while SCK is low
        HAL_GPIO_WritePin(FPGA_MOSI_GPIO_Port, FPGA_MOSI_Pin,
                          (tx & (1U << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);

        // Rising edge: slave samples MOSI
        HAL_GPIO_WritePin(FPGA_SCK_GPIO_Port, FPGA_SCK_Pin, GPIO_PIN_SET);

        // Sample MISO
        rx = (rx << 1) |
             (HAL_GPIO_ReadPin(FPGA_MISO_GPIO_Port, FPGA_MISO_Pin) == GPIO_PIN_SET);

        // Falling edge: slave prepares next MISO bit
        HAL_GPIO_WritePin(FPGA_SCK_GPIO_Port, FPGA_SCK_Pin, GPIO_PIN_RESET);
    }

    return rx;
}
void fpga_spi_begin(void)
{
    HAL_GPIO_WritePin(FPGA_CS_GPIO_Port, FPGA_CS_Pin, GPIO_PIN_RESET);
}

void fpga_spi_end(void)
{
    HAL_GPIO_WritePin(FPGA_CS_GPIO_Port, FPGA_CS_Pin, GPIO_PIN_SET);
}

volatile bool hasIrq = false;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == FPGA_IRQ_Pin)
    {
       hasIrq = true;
    }
}

uint8_t configRdWr;
uint8_t configOffset;
uint8_t configNumBlocks;
uint8_t configBlockIndex;

uint8_t CardIV[8];
uint8_t CardMaterial[8];
uint8_t CardNonce[8];
uint8_t MechaChallenge1[8], MechaChallenge2[8], MechaChallenge3[8];
        uint8_t CardResponse1[8];
        uint8_t CardResponse2[8];
        uint8_t CardResponse3[8];

        uint8_t MechaNonce[8] = { 0xde, 0xad, 0xc0, 0xde, 0xde, 0xad, 0xc0, 0xde };
void logBuffer(uint8_t* buffer, uint8_t len){
    for(int i=0; i<len; i++) usb_log("%02X ",buffer[i]);
    usb_log("\r\n");
}

int main(void)
{
/* USER CODE BEGIN 1 */

    fake_rtc_init();
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
        HAL_Delay(1000);
    MX_USB_DEVICE_Init();
       HAL_Delay(1000);
    while (1)
    {
        fake_rtc_update();
        //usb_log("Looping\r\n");
        //HAL_Delay(1000);
        if(hasIrq) {
            //usb_log("Data request!\r\n");
            fpga_spi_begin();
            bool commandValid = false;
            uint8_t command = spi_transfer(0x00);
            //usb_log("Got command %02X\r\n",command);
            switch(command) {
                case 0x08: {
                    spi_transfer(to_bcd(rtc.second));
                    spi_transfer(to_bcd(rtc.minute));
                    spi_transfer(to_bcd(rtc.hour));
                    spi_transfer(0x00);
                    spi_transfer(to_bcd(rtc.day));
                    spi_transfer(to_bcd(rtc.month));
                    spi_transfer(to_bcd(rtc.year));
                    commandValid = true;
                    break;
                }

                case 0x36: { // readRegionParams
                    spi_transfer(0x04);
                    spi_transfer(0x00);
                    for(int i = 0;i < 12; i++)
                        spi_transfer(SCPH_90004_NVM_bin[0x180+i]);
                    commandValid = true;
                    break;

                }
                case 0x37: { // read mac
                    for(int i=0; i<8; i++) spi_transfer(SCPH_90004_NVM_bin[0x198+i]);
                    commandValid = true;
                    break;
                }
                case 0x40: { // open config
                    configRdWr = spi_transfer(0);
                    configOffset = spi_transfer(0);
                    configNumBlocks = spi_transfer(0);
                    configBlockIndex = 0;
                    commandValid = true;
                    break;
                }
                case 0x41: { // read config
                    commandValid = true;

                    if(configRdWr || configBlockIndex >= configNumBlocks) {
                        spi_transfer(0x80);
                        for(int i=0; i<15; i++) spi_transfer(0);
                        commandValid = false;
                        break;
                    }

                    if((configOffset == 0 && configBlockIndex >= 4) ||
                    (configOffset == 1 && configBlockIndex >= 2) ||
                    (configOffset == 2 && configBlockIndex >= 7)) {
                        for(int i=0; i<16; i++) spi_transfer(0);
                        break;
                    }

                    int addr = configOffset == 0 ? 0x270 :
                            configOffset == 2 ? 0x200 : 0x2B0;

                    addr += configBlockIndex++ * 16;

                    for(int i=0; i<16; i++) spi_transfer(SCPH_90004_NVM_bin[addr+i]);

                    
                    break;
                }
                case 0x43: { //closeconfig
                    commandValid = true;
                    configRdWr = 0;
                    configOffset =0;
                    configNumBlocks =0;
                    configBlockIndex =0;
                    break;
                }
                case 0x80: //resetAuthContext
                    //usb_log("Reset crypto context!\r\n");
                    meResetCryptoContext();
                    commandValid = true;
                    break;
                case 0x82: {
                    //usb_log("Read CardIV + CardMaterial\n\r");
                    for(int i=0;i<8;i++)
                        CardIV[i] = spi_transfer(0x00);
                    for(int i=0;i<8;i++)
                        CardMaterial[i] = spi_transfer(0x00);
                    //usb_log("CardIV: \r\n");
                    //logBuffer(CardIV, 8);
                    //usb_log("CardMaterial: \r\n");
                    //logBuffer(CardMaterial, 8);
                    meCardCalcUniqueKey(CardIV, CardMaterial);
                    commandValid = true;
                    break;
                case 0x83: {
                    //usb_log("Read Nonce \r\n");
                    for(int i=0;i<8;i++)
                        CardNonce[i] = spi_transfer(0x00);
                    //usb_log("CardNonce: \r\n");
                    //logBuffer(CardNonce, 8);
                    meCardGenerateChallenge(CardIV, CardNonce, MechaNonce, MechaChallenge1, MechaChallenge2, MechaChallenge3);
                    commandValid = true;
                    break;
                }
                case 0x84: {
                    //usb_log("Send Challenge1+halfChallenge2\r\n"); 
                    for(int i=0;i<8;i++){
                        spi_transfer(MechaChallenge1[i]);
                    }
                    for(int i=0;i<4;i++){
                        spi_transfer(MechaChallenge2[i]);
                    }
                    commandValid = true;
                    break;
                }
                case 0x85: {
                    usb_log("sendhalfchall2 + challenge3\r\n");
                    for(int i=4;i<8;i++)
                        spi_transfer(MechaChallenge2[i]);
                    for(int i=0; i<8;i++)
                        spi_transfer(MechaChallenge3[i]);
                    commandValid = true;
                    break;
                }
                case 0x86: {
                    usb_log("Read cardResponse1 + cardresponse2\r\n");
                    for(int i=0;i<8;i++) {
                        CardResponse1[i] =spi_transfer(0x00);
                    }
                    for(int i=0;i<8;i++) {
                        CardResponse2[i] =spi_transfer(0x00);
                    }
                    commandValid = true;
                    break;
                }
                case 0x87: {
                    usb_log("Read cardResponse3\r\n");
                    for(int i=0;i<8;i++) {
                        CardResponse3[i] =spi_transfer(0x00);
                    }
                    commandValid = true;
                    break;
                }
                case 0x88: {
                    if(meCardVerifyResponse(CardResponse1, CardResponse2, CardResponse3)){
                        commandValid = false;
                    }else {/*
                        usb_log("Card unlocked!!!\n");
                        usb_log("CardIV: \r\n");
                        logBuffer(CardIV, 8);
                        usb_log("CardMaterial: \r\n");
                        logBuffer(CardMaterial, 8);
                        usb_log("CardNonce: \r\n");
                        logBuffer(CardNonce, 8);
                        usb_log("MechaChallenge1: \r\n");
                        logBuffer(MechaChallenge1, 8);
                        usb_log("MechaChallenge2: \r\n");
                        logBuffer(MechaChallenge2, 8);
                        usb_log("MechaChallenge3: \r\n");
                        logBuffer(MechaChallenge3, 8);
                        usb_log("CardResponse1: \r\n");
                        logBuffer(CardResponse1, 8);
                        usb_log("CardResponse2:: \r\n");
                        logBuffer(CardResponse2, 8);
                        usb_log("CardResponse3: \r\n");
                        logBuffer(CardResponse3, 8);
                        usb_log("The card is real \n\r");
                        usb_log( "but is the mechacon? ;)\r\n");*/
                        commandValid = true;
                    }
                    
                    break;
                }

                }
            }
            if(commandValid) spi_transfer(0xDE);
            else {
                usb_log("Command invalid! %02X\r\n",command);
                spi_transfer(0xAD);}
            fpga_spi_end();
        
            switch(command) {
                case 0x36: usb_log("Sent region params!\r\n");
                    break;
                case 0x37: usb_log("Sent mac\r\n");
                    break;
                case 0x40: {
                    //usb_log("opencfg %d, %d,%d\r\n",configRdWr,configOffset,configNumBlocks);
                    break;
                }
                case 0x41: {
                    //usb_log("Reading cfg!\r\n");
                    break;
                }
                case 0x80: usb_log("Got 80!\r\n");
            }
            hasIrq = false;
        }/*
        uint8_t CardIV[8] = {0x01, 0x11, 0xA2, 0x00, 0x42, 0x02, 0x13, 0x16 };
        uint8_t CardMaterial[8] = {0x01, 0x02, 0x06, 0x79, 0x00, 0x00, 0x28, 0xF1 };
        uint8_t CardNonce[8] = {0xF9, 0x71, 0xA1, 0x69, 0xC8, 0x19, 0x18, 0x44 };
        

        
        
        usb_log("Challenge1: \r\n");
        for(uint8_t i = 0; i<8;i++)
            usb_log("%02X ",MechaChallenge1[i]);

        usb_log("\r\n");
        usb_log("Challenge2: \r\n");
        for(uint8_t i = 0; i<8;i++)
            usb_log("%02X ",MechaChallenge2[i]);

        usb_log("\r\n");
        usb_log("Challenge3: \r\n");
        for(uint8_t i = 0; i<8;i++)
            usb_log("%02X ",MechaChallenge3[i]);

        usb_log("\r\n");
        
        usb_log("Hello world!\n\r");
      //HAL_Delay(10000);*/
        /*desc 
        1: 0x80 reset context
        2: 0x81 skip
        3: 0x82 read CardIV, CardMaterial
        4: 0x83 read Nonce
        be careful here 
        pol_cal_cmplt() 
        5: 0x84 write Challenge1[7:0], Challenge2[3:0] 
        6: 0x85 write Challenge2[7:4], Challenge3[7:0]
        7: 0x86 read CardResponse1, CardResponse2
        8: 0x87 read CardResponse3
        pol_cal_cmplt() 
        9 :0x88 just report ok? or skip
         
        */}
}



#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
