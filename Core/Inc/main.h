/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define R_ADC_Pin GPIO_PIN_0
#define R_ADC_GPIO_Port GPIOA
#define Y_ADC_Pin GPIO_PIN_1
#define Y_ADC_GPIO_Port GPIOA
#define B_ADC_Pin GPIO_PIN_2
#define B_ADC_GPIO_Port GPIOA
#define BATT_ADC_Pin GPIO_PIN_3
#define BATT_ADC_GPIO_Port GPIOA
#define CT_ADC_Pin GPIO_PIN_4
#define CT_ADC_GPIO_Port GPIOA
#define BATT_CT_Pin GPIO_PIN_5
#define BATT_CT_GPIO_Port GPIOA
#define CHRG_ON_Pin GPIO_PIN_6
#define CHRG_ON_GPIO_Port GPIOA
#define GSM_PWR_Pin GPIO_PIN_0
#define GSM_PWR_GPIO_Port GPIOB
#define GSM_TX_Pin GPIO_PIN_9
#define GSM_TX_GPIO_Port GPIOA
#define GSM_RX_Pin GPIO_PIN_10
#define GSM_RX_GPIO_Port GPIOA
#define LD4_Pin GPIO_PIN_11
#define LD4_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_12
#define BUZZER_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_15
#define LD1_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_3
#define LD2_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_4
#define LD3_GPIO_Port GPIOB
#define RELAY_DELTA_Pin GPIO_PIN_5
#define RELAY_DELTA_GPIO_Port GPIOB
#define RELAY_OFF_Pin GPIO_PIN_6
#define RELAY_OFF_GPIO_Port GPIOB
#define RELAY_ON_Pin GPIO_PIN_7
#define RELAY_ON_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
