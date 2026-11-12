
/* USER CODE BEGIN Header */

/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : BLDC Open-Loop Startup Demo
 ******************************************************************************
 */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN PD */

/* Phase A - IR2104 U5 */
#define PHASE_A_IN GPIO_PIN_0
#define PHASE_A_SD GPIO_PIN_1

/* Phase B - IR2104 U1 */
#define PHASE_B_IN GPIO_PIN_2
#define PHASE_B_SD GPIO_PIN_3

/* Phase C - IR2104 U4 */
#define PHASE_C_IN GPIO_PIN_4
#define PHASE_C_SD GPIO_PIN_5

/* All driver control pins */

#define ALL_DRIVER_PINS                                                        \
  (PHASE_A_IN | PHASE_A_SD | PHASE_B_IN | PHASE_B_SD | PHASE_C_IN | PHASE_C_SD)

/* Onboard LED: PC13, active LOW */

#define LED_ON() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)
#define LED_OFF() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)

/* IR2104 shutdown logic */
/* LOW = Enabled, HIGH = Disabled */

#define DRIVER_ENABLE GPIO_PIN_RESET
#define DRIVER_DISABLE GPIO_PIN_SET

/* Startup duration */

#define MOTOR_RUN_TIME 3000

/* USER CODE END PD */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

static void DisableAllDrivers(void);
static void ApplyCommutationStep(uint8_t step);

/**
 * @brief Application entry point
 */

int main(void) {
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  while (1) {
    /*
     * INITIAL SAFE STATE
     *
     * Disable all three gate drivers.
     */

    DisableAllDrivers();

    /*
     * STAGE 1: STARTUP INDICATION
     *
     * Blink onboard LED three times.
     */

    for (uint8_t i = 0; i < 3; i++) {
      LED_ON();
      HAL_Delay(250);

      LED_OFF();
      HAL_Delay(250);
    }

    /*
     * STAGE 2: OPEN-LOOP BLDC STARTUP
     *
     * Execute six-step commutation.
     *
     * First second: 20 ms per step
     * Second second: 15 ms per step
     * Third second: 10 ms per step
     *
     * No speed control or BEMF detection.
     */

    uint32_t startup_start = HAL_GetTick();

    uint8_t bldc_step = 0;

    while ((HAL_GetTick() - startup_start) < MOTOR_RUN_TIME) {
      uint32_t elapsed = HAL_GetTick() - startup_start;

      uint32_t step_delay;

      if (elapsed < 1000) {
        step_delay = 20;
      } else if (elapsed < 2000) {
        step_delay = 15;
      } else {
        step_delay = 10;
      }

      ApplyCommutationStep(bldc_step);

      HAL_Delay(step_delay);

      bldc_step++;

      if (bldc_step >= 6) {
        bldc_step = 0;
      }
    }

    /*
     * STAGE 3: STOP
     *
     * Disable all three gate drivers.
     */

    DisableAllDrivers();

    /*
     * Wait before repeating the startup sequence.
     */

    HAL_Delay(1000);
  }
}

/**
 * @brief Disable all three IR2104 gate drivers
 *
 * SD HIGH = Driver disabled
 */

static void DisableAllDrivers(void) {
  HAL_GPIO_WritePin(GPIOA, PHASE_A_SD | PHASE_B_SD | PHASE_C_SD,
                    DRIVER_DISABLE);
}

/**
 * @brief Apply six-step BLDC commutation
 *
 * High phase: IN HIGH, SD LOW
 * Low phase:  IN LOW,  SD LOW
 * Floating:   SD HIGH
 */

static void ApplyCommutationStep(uint8_t step) {
  /*
   * First disable all three drivers.
   */

  DisableAllDrivers();

  /*
   * Configure phase inputs while drivers are disabled.
   */

  switch (step) {
  case 0:

    /* A HIGH, B LOW, C FLOATING */

    HAL_GPIO_WritePin(GPIOA, PHASE_A_IN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, PHASE_B_IN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, PHASE_C_IN, GPIO_PIN_RESET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(GPIOA, PHASE_A_SD, DRIVER_ENABLE);
    HAL_GPIO_WritePin(GPIOA, PHASE_B_SD, DRIVER_ENABLE);

    break;

  case 1:

    /* A HIGH, B FLOATING, C LOW */

    HAL_GPIO_WritePin(GPIOA, PHASE_A_IN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, PHASE_B_IN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, PHASE_C_IN, GPIO_PIN_RESET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(GPIOA, PHASE_A_SD, DRIVER_ENABLE);
    HAL_GPIO_WritePin(GPIOA, PHASE_C_SD, DRIVER_ENABLE);

    break;

  case 2:

    /* A FLOATING, B HIGH, C LOW */

    HAL_GPIO_WritePin(GPIOA, PHASE_A_IN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, PHASE_B_IN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, PHASE_C_IN, GPIO_PIN_RESET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(GPIOA, PHASE_B_SD, DRIVER_ENABLE);
    HAL_GPIO_WritePin(GPIOA, PHASE_C_SD, DRIVER_ENABLE);

    break;

  case 3:

    /* A LOW, B HIGH, C FLOATING */

    HAL_GPIO_WritePin(GPIOA, PHASE_A_IN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, PHASE_B_IN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, PHASE_C_IN, GPIO_PIN_RESET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(GPIOA, PHASE_A_SD, DRIVER_ENABLE);
    HAL_GPIO_WritePin(GPIOA, PHASE_B_SD, DRIVER_ENABLE);

    break;

  case 4:

    /* A FLOATING, B LOW, C HIGH */

    HAL_GPIO_WritePin(GPIOA, PHASE_A_IN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, PHASE_B_IN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, PHASE_C_IN, GPIO_PIN_SET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(GPIOA, PHASE_B_SD, DRIVER_ENABLE);
    HAL_GPIO_WritePin(GPIOA, PHASE_C_SD, DRIVER_ENABLE);

    break;

  case 5:

    /* A LOW, B FLOATING, C HIGH */

    HAL_GPIO_WritePin(GPIOA, PHASE_A_IN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, PHASE_B_IN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, PHASE_C_IN, GPIO_PIN_SET);

    HAL_Delay(1);

    HAL_GPIO_WritePin(GPIOA, PHASE_A_SD, DRIVER_ENABLE);
    HAL_GPIO_WritePin(GPIOA, PHASE_C_SD, DRIVER_ENABLE);

    break;

  default:

    DisableAllDrivers();

    break;
  }
}

/**
 * @brief System Clock Configuration
 */

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;

  RCC_OscInitStruct.HSIState = RCC_HSI_ON;

  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;

  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    Error_Handler();
  }
}

/**
 * @brief GPIO Initialization Function
 */

static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable GPIO clocks */

  __HAL_RCC_GPIOA_CLK_ENABLE();

  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*
   * Initial output levels
   *
   * IN pins = LOW
   * SD pins = HIGH (drivers disabled)
   * PC13 = HIGH (LED OFF)
   */

  HAL_GPIO_WritePin(GPIOA, PHASE_A_IN | PHASE_B_IN | PHASE_C_IN,
                    GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOA, PHASE_A_SD | PHASE_B_SD | PHASE_C_SD,
                    DRIVER_DISABLE);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  /* Configure PA0 - PA5 */

  GPIO_InitStruct.Pin = ALL_DRIVER_PINS;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure onboard LED */

  GPIO_InitStruct.Pin = GPIO_PIN_13;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
 * @brief Error Handler
 */

void Error_Handler(void) {
  __disable_irq();

  while (1) {
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line) {}

#endif /* USE_FULL_ASSERT */
